/*
 * bee01_battery.c - Maxim 8903 USB/Adapter Charger Driver
 *
 * Copyright (C) 2011 Samsung Electronics
 * Copyright (C) 2011-2013 Freescale Semiconductor, Inc.
 * Copyright (C) 2013 
 * Based on max8903_charger.c
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/power_supply.h>
#include <linux/platform_device.h>
#include <linux/power/sabresd_battery.h>
#include <linux/sort.h>


#define	BATTERY_UPDATE_INTERVAL	5 /*seconds*/
#define LOW_VOLT_THRESHOLD	2800000
#define HIGH_VOLT_THRESHOLD	4200000
#define ADC_SAMPLE_COUNT	6

struct max8903_data {
	struct max8903_pdata *pdata;
	struct device *dev;
	struct power_supply psy;
	struct power_supply usb;
	bool fault;
	bool usb_in;
	bool ta_in;
	bool chg_state;
	struct delayed_work work;
	unsigned int interval;
	unsigned short thermal_raw;
	int voltage_uV;
	int current_uA;
	int battery_status;
	int charger_online;
	int charger_voltage_uV;
	int real_capacity;
	int percent;
	int old_percent;
	int usb_charger_online;
	int first_delay_count;
	struct power_supply bat;
	struct power_supply	detect_usb;
	struct mutex work_lock;
};

typedef struct {
	u32 voltage;
	u32 percent;
} battery_capacity , *pbattery_capacity;
static int cpu_type_flag;
static int offset_discharger;
static int offset_charger;
static int offset_usb_charger;

static enum power_supply_property max8903_charger_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static enum power_supply_property max8903_battery_props[] = {
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_CAPACITY_LEVEL,
	POWER_SUPPLY_PROP_TEMP,
};

static int max8903_suspend_flag = 0;

static void max8903_charger_update_status(struct max8903_data *data)
{
	if (data->usb_in || data->ta_in) {
		if (data->ta_in)
		data->charger_online = 1;

		if (data->usb_in)
		data->usb_charger_online = 1;
		} else {
				data->charger_online = 0;
				data->usb_charger_online = 0;
		  }
	if (data->charger_online == 0 && data->usb_charger_online == 0) {
			data->battery_status = POWER_SUPPLY_STATUS_DISCHARGING;
	} else {
		if (gpio_get_value(data->pdata->chg) == 0) {
			data->battery_status = POWER_SUPPLY_STATUS_CHARGING;
		} else if (data->ta_in && gpio_get_value(data->pdata->chg) == 1) {
		  if (!data->pdata->feature_flag) {
			if (data->percent >= 99)
				data->battery_status = POWER_SUPPLY_STATUS_FULL;
			else
				data->battery_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
			} else {
				data->battery_status = POWER_SUPPLY_STATUS_FULL;
			}
		} else if (data->usb_in && gpio_get_value(data->pdata->chg) == 1) {
		  if (!data->pdata->feature_flag) {
			if (data->percent >= 99)
				data->battery_status = POWER_SUPPLY_STATUS_FULL;
			else
				data->battery_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
			} else {
				data->battery_status = POWER_SUPPLY_STATUS_FULL;
			}
		}
	}
	pr_debug("chg: %d\n", gpio_get_value(data->pdata->chg));
	pr_debug("dc: %d\n", gpio_get_value(data->pdata->dok));
	pr_debug("flt: %d\n", gpio_get_value(data->pdata->flt));
}

#ifdef CONFIG_BATTERY_MAX17048
extern int max17048_get_vcell_external(void);
extern int max17048_get_soc_external(void);
extern int max17048_get_temp_external(void);
#endif

static void max8903_battery_update_status(struct max8903_data *data)
{
	int temp = 0;
	static int temp_last;
	bool changed_flag;
	changed_flag = false;
	mutex_lock(&data->work_lock);
	if (!data->pdata->feature_flag) {
#ifdef CONFIG_BATTERY_MAX17048
		if(max8903_suspend_flag != 1)
		{
			temp = max17048_get_vcell_external() * 1000;
		}
		else
		{
			temp = data->voltage_uV;
		}
#else
		temp = 4000000;
#endif
		if (temp_last == 0) {
			data->voltage_uV = temp;
			temp_last = temp;
		}
		if (data->charger_online == 0 && temp_last != 0) {
			temp_last = temp;
			data->voltage_uV = temp;
		}
		if (data->charger_online == 1 || data->usb_charger_online == 1) {
			data->voltage_uV = temp;
			temp_last = temp;
		}
#ifdef CONFIG_BATTERY_MAX17048
		if(max8903_suspend_flag != 1)
		{
			data->percent = max17048_get_soc_external();
		}
		
		if(data->percent > 100)
		{
			data->percent = 100;
		}
#else
		data->percent = 100;
#endif
		if (data->percent != data->old_percent) {
			data->old_percent = data->percent;
			changed_flag = true;
		}
		if (changed_flag) {
			changed_flag = false;
			power_supply_changed(&data->bat);
		}
		 /*
		  *because boot time gap between led framwork and charger
		  *framwork,when system boots with charger attatched,
		  *charger led framwork loses the first charger online event,
		  *add once extra power_supply_changed can fix this issure
		  */
		if (data->first_delay_count < 200) {
			data->first_delay_count = data->first_delay_count + 1 ;
			power_supply_changed(&data->bat);
		}
	}
	mutex_unlock(&data->work_lock);
}

static int max8903_battery_get_property(struct power_supply *bat,
				       enum power_supply_property psp,
				       union power_supply_propval *val)
{
	struct max8903_data *di = container_of(bat,
			struct max8903_data, bat);
	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		val->intval = POWER_SUPPLY_STATUS_UNKNOWN;
			if (gpio_get_value(di->pdata->chg) == 0) {
				di->battery_status = POWER_SUPPLY_STATUS_CHARGING;
			} else if (di->ta_in && gpio_get_value(di->pdata->chg) == 1) {
				if (!di->pdata->feature_flag) {
					if (di->percent >= 99)
						di->battery_status = POWER_SUPPLY_STATUS_FULL;
					else
						di->battery_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
					} else {
							di->battery_status = POWER_SUPPLY_STATUS_FULL;
					}
			 } else if (di->usb_in && gpio_get_value(di->pdata->chg) == 1) {
				if (!di->pdata->feature_flag) {
					if (di->percent >= 99)
					    di->battery_status = POWER_SUPPLY_STATUS_FULL;
					else
						di->battery_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
					} else {
						di->battery_status = POWER_SUPPLY_STATUS_FULL;
					}
			}
		val->intval = di->battery_status;
		return 0;
	default:
		break;
	}

	switch (psp) {
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = di->voltage_uV;
		break;
	case POWER_SUPPLY_PROP_CHARGE_NOW:
		val->intval = 0;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
		val->intval = HIGH_VOLT_THRESHOLD;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
		val->intval = LOW_VOLT_THRESHOLD;
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = 1;
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		val->intval = di->percent < 0 ? 0 :
				(di->percent > 100 ? 100 : di->percent);
		break;
	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = POWER_SUPPLY_HEALTH_GOOD;
		if (di->fault)
			val->intval = POWER_SUPPLY_HEALTH_UNSPEC_FAILURE;
		break;
	case POWER_SUPPLY_PROP_CAPACITY_LEVEL:
		if (di->battery_status == POWER_SUPPLY_STATUS_FULL)
			val->intval = POWER_SUPPLY_CAPACITY_LEVEL_FULL;
		else if (di->percent <= 15)
			val->intval = POWER_SUPPLY_CAPACITY_LEVEL_LOW;
		else
			val->intval = POWER_SUPPLY_CAPACITY_LEVEL_NORMAL;
		break;
	case POWER_SUPPLY_PROP_TEMP:
		val->intval = max17048_get_temp_external() * 10;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
static int max8903_get_property(struct power_supply *psy,
		enum power_supply_property psp,
		union power_supply_propval *val)
{
	struct max8903_data *data = container_of(psy,
			struct max8903_data, psy);

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = 0;
		if (data->ta_in)
			val->intval = 1;
		data->charger_online = val->intval;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}
static int max8903_get_usb_property(struct power_supply *usb,
		enum power_supply_property psp,
		union power_supply_propval *val)
{
	struct max8903_data *data = container_of(usb,
			struct max8903_data, usb);

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = 0;
		if (data->usb_in)
			val->intval = 1;
		data->usb_charger_online = val->intval;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}
static irqreturn_t max8903_dcin(int irq, void *_data)
{
	struct max8903_data *data = _data;
	struct max8903_pdata *pdata = data->pdata;
	bool ta_in;

	ta_in = gpio_get_value(pdata->dok) ? false : true;

	if (ta_in == data->ta_in)
		return IRQ_HANDLED;

	data->ta_in = ta_in;
	pr_info("TA(DC-IN) Charger %s.\n", ta_in ?
			"Connected" : "Disconnected");
	max8903_charger_update_status(data);
	max8903_battery_update_status(data);
	power_supply_changed(&data->psy);
	power_supply_changed(&data->bat);
	return IRQ_HANDLED;
}
static irqreturn_t max8903_usbin(int irq, void *_data)
{
	struct max8903_data *data = _data;
	struct max8903_pdata *pdata = data->pdata;
	bool usb_in;
	usb_in = gpio_get_value(pdata->uok) ? false : true;
	if (usb_in == data->usb_in)
		return IRQ_HANDLED;

	data->usb_in = usb_in;
	max8903_charger_update_status(data);
	max8903_battery_update_status(data);
	pr_info("USB Charger %s.\n", usb_in ?
			"Connected" : "Disconnected");
	power_supply_changed(&data->bat);
	power_supply_changed(&data->usb);
	return IRQ_HANDLED;
}

static irqreturn_t max8903_fault(int irq, void *_data)
{
	struct max8903_data *data = _data;
	struct max8903_pdata *pdata = data->pdata;
	bool fault;

	fault = gpio_get_value(pdata->flt) ? false : true;

	if (fault == data->fault)
		return IRQ_HANDLED;

	data->fault = fault;

	if (fault)
		dev_err(data->dev, "Charger suffers a fault and stops.\n");
	else
		dev_err(data->dev, "Charger recovered from a fault.\n");
	max8903_charger_update_status(data);
	max8903_battery_update_status(data);
	power_supply_changed(&data->psy);
	power_supply_changed(&data->bat);
	power_supply_changed(&data->usb);
	return IRQ_HANDLED;
}

static irqreturn_t max8903_chg(int irq, void *_data)
{
	struct max8903_data *data = _data;
	struct max8903_pdata *pdata = data->pdata;
	int chg_state;

	chg_state = gpio_get_value(pdata->chg) ? false : true;

	if (chg_state == data->chg_state)
		return IRQ_HANDLED;

	data->chg_state = chg_state;
	max8903_charger_update_status(data);
	max8903_battery_update_status(data);
	power_supply_changed(&data->psy);
	power_supply_changed(&data->bat);
	power_supply_changed(&data->usb);
	return IRQ_HANDLED;
}

static void max8903_battery_work(struct work_struct *work)
{
	struct max8903_data *data;
	data = container_of(work, struct max8903_data, work.work);
	data->interval = HZ * BATTERY_UPDATE_INTERVAL;

	max8903_charger_update_status(data);
	max8903_battery_update_status(data);
	pr_debug("battery voltage: %4d mV\n" , data->voltage_uV);
	pr_debug("charger online status: %d\n" , data->charger_online);
	pr_debug("battery status : %d\n" , data->battery_status);
	pr_debug("battery capacity percent: %3d\n" , data->percent);
	pr_debug("data->usb_in: %x , data->ta_in: %x \n" , data->usb_in, data->ta_in);
	/* reschedule for the next time */
	schedule_delayed_work(&data->work, data->interval);
}

static ssize_t max8903_voltage_offset_discharger_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "read offset_discharger:%04d\n",
		offset_discharger);
}

static ssize_t max8903_voltage_offset_discharger_store(struct device *dev,
			     struct device_attribute *attr, const char *buf,
			     size_t count)
{
	offset_discharger = simple_strtoul(buf, NULL, 10);
	pr_info("read offset_discharger:%04d\n", offset_discharger);
	return count;
}

static ssize_t max8903_voltage_offset_charger_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "read offset_charger:%04d\n",
		offset_charger);
}

static ssize_t max8903_voltage_offset_charger_store(struct device *dev,
			     struct device_attribute *attr, const char *buf,
			     size_t count)
{
	offset_charger = simple_strtoul(buf, NULL, 10);
	pr_info("read offset_charger:%04d\n", offset_charger);
	return count;
}

static ssize_t max8903_voltage_offset_usb_charger_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "read offset_usb_charger:%04d\n",
		offset_usb_charger);
}

static ssize_t max8903_voltage_offset_usb_charger_store(struct device *dev,
			     struct device_attribute *attr, const char *buf,
			     size_t count)
{
	offset_usb_charger = simple_strtoul(buf, NULL, 10);
	pr_info("read offset_charger:%04d\n", offset_usb_charger);
	return count;
}

static struct device_attribute max8903_discharger_dev_attr = {
	.attr = {
		 .name = "max8903_ctl_offset_discharger",
		 .mode = S_IRUSR | S_IWUSR,
		 },
	.show = max8903_voltage_offset_discharger_show,
	.store = max8903_voltage_offset_discharger_store,
};

static struct device_attribute max8903_charger_dev_attr = {
	.attr = {
		 .name = "max8903_ctl_offset_charger",
		 .mode = S_IRUSR | S_IWUSR,
		 },
	.show = max8903_voltage_offset_charger_show,
	.store = max8903_voltage_offset_charger_store,
};

static struct device_attribute max8903_usb_charger_dev_attr = {
	.attr = {
		 .name = "max8903_ctl_offset_usb_charger",
		 .mode = S_IRUSR | S_IWUSR,
		 },
	.show = max8903_voltage_offset_usb_charger_show,
	.store = max8903_voltage_offset_usb_charger_store,
};

static __devinit int max8903_probe(struct platform_device *pdev)
{
	struct max8903_data *data;
	struct device *dev = &pdev->dev;
	struct max8903_pdata *pdata = pdev->dev.platform_data;
	int ret = 0;
	int gpio = 0;
	int ta_in = 0;
	int usb_in = 0;
	int retval;
	cpu_type_flag = 0;
	if (cpu_is_mx6q())
		cpu_type_flag = 1;
	if (cpu_is_mx6dl())
		cpu_type_flag = 0;
	data = kzalloc(sizeof(struct max8903_data), GFP_KERNEL);
	if (data == NULL) {
		dev_err(dev, "Cannot allocate memory.\n");
		return -ENOMEM;
	}
	data->first_delay_count = 0;
	data->pdata = pdata;
	data->dev = dev;
	platform_set_drvdata(pdev, data);
	data->usb_in = 0;
	data->ta_in = 0;
	if (pdata->dc_valid == false && pdata->usb_valid == false) {
		dev_err(dev, "No valid power sources.\n");
		printk(KERN_INFO "No valid power sources.\n");
		ret = -EINVAL;
		goto err;
	}
	if (pdata->dc_valid) {
		if (pdata->dok && gpio_is_valid(pdata->dok)) {
			gpio = pdata->dok; /* PULL_UPed Interrupt */
//			/* set DOK gpio input */
//			ret = gpio_request(gpio, "max8903-DOK");
//			if (ret) {
//				printk(KERN_ERR"request max8903-DOK error!!\n");
//				goto err;
//			} else {
//				gpio_direction_input(gpio);
//			}
			ta_in = gpio_get_value(gpio) ? 0 : 1;
		} else if (pdata->dok && gpio_is_valid(pdata->dok) && pdata->dcm_always_high) {
			ta_in = pdata->dok; /* PULL_UPed Interrupt */
			ta_in = gpio_get_value(gpio) ? 0 : 1;
		} else {
			dev_err(dev, "When DC is wired, DOK and DCM should"
					" be wired as well."
					" or set dcm always high\n");
			ret = -EINVAL;
			goto err;
		}
	}
	if (pdata->usb_valid) {
		if (pdata->uok && gpio_is_valid(pdata->uok)) {
			gpio = pdata->uok;
//			/* set UOK gpio input */
//			ret = gpio_request(gpio, "max8903-UOK");
//			if (ret) {
//				printk(KERN_ERR"request max8903-UOK error!!\n");
//				goto err;
//			} else {
//				gpio_direction_input(gpio);
//			}
			usb_in = gpio_get_value(gpio) ? 0 : 1;
		} else {
			dev_err(dev, "When USB is wired, UOK should be wired."
					"as well.\n");
			ret = -EINVAL;
			goto err;
		}
	}
	if (pdata->chg) {
		if (!gpio_is_valid(pdata->chg)) {
			dev_err(dev, "Invalid pin: chg.\n");
			ret = -EINVAL;
			goto err;
		}
		/* set CHG gpio input */
		ret = gpio_request(pdata->chg, "max8903-CHG");
		if (ret) {
			printk(KERN_ERR"request max8903-CHG error!!\n");
			goto err;
		} else {
			gpio_direction_input(pdata->chg);
		}
	}
	if (pdata->flt) {
		if (!gpio_is_valid(pdata->flt)) {
			dev_err(dev, "Invalid pin: flt.\n");
			ret = -EINVAL;
			goto err;
		}
		/* set FLT gpio input */
		ret = gpio_request(pdata->flt, "max8903-FLT");
		if (ret) {
			printk(KERN_ERR"request max8903-FLT error!!\n");
			goto err;
		} else {
			gpio_direction_input(pdata->flt);
		}
	}
	if (pdata->usus) {
		if (!gpio_is_valid(pdata->usus)) {
			dev_err(dev, "Invalid pin: usus.\n");
			ret = -EINVAL;
			goto err;
		}
	}
	mutex_init(&data->work_lock);
	data->fault = false;
	data->ta_in = ta_in;
	data->usb_in = usb_in;
	data->psy.name = "max8903-ac";
	data->psy.type = POWER_SUPPLY_TYPE_MAINS;
	data->psy.get_property = max8903_get_property;
	data->psy.properties = max8903_charger_props;
	data->psy.num_properties = ARRAY_SIZE(max8903_charger_props);
	ret = power_supply_register(dev, &data->psy);
	if (ret) {
		dev_err(dev, "failed: power supply register.\n");
		goto err_psy;
	}
	data->usb.name = "max8903-usb";
	data->usb.type = POWER_SUPPLY_TYPE_USB;
	data->usb.get_property = max8903_get_usb_property;
	data->usb.properties = max8903_charger_props;
	data->usb.num_properties = ARRAY_SIZE(max8903_charger_props);
	ret = power_supply_register(dev, &data->usb);
	if (ret) {
		dev_err(dev, "failed: power supply register.\n");
		goto err_psy;
	}
	data->bat.name = "max8903-charger";
	data->bat.type = POWER_SUPPLY_TYPE_BATTERY;
	data->bat.properties = max8903_battery_props;
	data->bat.num_properties = ARRAY_SIZE(max8903_battery_props);
	data->bat.get_property = max8903_battery_get_property;
	data->bat.use_for_apm = 1;
	retval = power_supply_register(&pdev->dev, &data->bat);
	if (retval) {
		dev_err(data->dev, "failed to register battery\n");
		goto battery_failed;
	}
	INIT_DELAYED_WORK(&data->work, max8903_battery_work);
	schedule_delayed_work(&data->work, data->interval);
	if (pdata->dc_valid) {
		ret = request_threaded_irq(gpio_to_irq(pdata->dok),
				NULL, max8903_dcin,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"MAX8903 DC IN", data);
		if (ret) {
			dev_err(dev, "Cannot request irq %d for DC (%d)\n",
					gpio_to_irq(pdata->dok), ret);
			goto err_usb_irq;
		}
	}

	if (pdata->usb_valid) {
		ret = request_threaded_irq(gpio_to_irq(pdata->uok),
				NULL, max8903_usbin,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"MAX8903 USB IN", data);
		if (ret) {
			dev_err(dev, "Cannot request irq %d for USB (%d)\n",
					gpio_to_irq(pdata->uok), ret);
			goto err_dc_irq;
		}
	}

	if (pdata->flt) {
		ret = request_threaded_irq(gpio_to_irq(pdata->flt),
				NULL, max8903_fault,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"MAX8903 Fault", data);
		if (ret) {
			dev_err(dev, "Cannot request irq %d for Fault (%d)\n",
					gpio_to_irq(pdata->flt), ret);
			goto err_flt_irq;
		}
	}

	if (pdata->chg) {
		ret = request_threaded_irq(gpio_to_irq(pdata->chg),
				NULL, max8903_chg,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"MAX8903 Status", data);
		if (ret) {
			dev_err(dev, "Cannot request irq %d for Status (%d)\n",
					gpio_to_irq(pdata->flt), ret);
			goto err_chg_irq;
		}
	}
	ret = device_create_file(&pdev->dev, &max8903_discharger_dev_attr);
	if (ret)
		dev_err(&pdev->dev, "create device file failed!\n");
	ret = device_create_file(&pdev->dev, &max8903_charger_dev_attr);
	if (ret)
		dev_err(&pdev->dev, "create device file failed!\n");
	ret = device_create_file(&pdev->dev, &max8903_usb_charger_dev_attr);
	if (ret)
		dev_err(&pdev->dev, "create device file failed!\n");
	if (cpu_type_flag == 1) {
			offset_discharger = 1694;
			offset_charger = 1900;
			offset_usb_charger = 1685;
	}
	if (cpu_type_flag == 0) {
			offset_discharger = 1464;
			offset_charger = 1485;
			offset_usb_charger = 1285;
	}
	max8903_charger_update_status(data);
	max8903_battery_update_status(data);
	return 0;
err_psy:
	power_supply_unregister(&data->psy);
battery_failed:
	power_supply_unregister(&data->bat);
err_usb_irq:
	if (pdata->usb_valid)
		free_irq(gpio_to_irq(pdata->uok), data);
	cancel_delayed_work(&data->work);
err_dc_irq:
	if (pdata->dc_valid)
		free_irq(gpio_to_irq(pdata->dok), data);
	cancel_delayed_work(&data->work);
err_flt_irq:
	if (pdata->usb_valid)
		free_irq(gpio_to_irq(pdata->uok), data);
	cancel_delayed_work(&data->work);
err_chg_irq:
	if (pdata->dc_valid)
		free_irq(gpio_to_irq(pdata->dok), data);
	cancel_delayed_work(&data->work);
err:
	if (pdata->uok)
		gpio_free(pdata->uok);
	if (pdata->dok)
		gpio_free(pdata->dok);
	if (pdata->flt)
		gpio_free(pdata->flt);
	if (pdata->chg)
		gpio_free(pdata->chg);
	kfree(data);
	return ret;
}

static __devexit int max8903_remove(struct platform_device *pdev)
{
	struct max8903_data *data = platform_get_drvdata(pdev);
	if (data) {
		struct max8903_pdata *pdata = data->pdata;
		if (pdata->flt)
			free_irq(gpio_to_irq(pdata->flt), data);
		if (pdata->usb_valid)
			free_irq(gpio_to_irq(pdata->uok), data);
		if (pdata->dc_valid)
			free_irq(gpio_to_irq(pdata->dok), data);
		if (pdata->dc_valid)
			free_irq(gpio_to_irq(pdata->chg), data);
		cancel_delayed_work_sync(&data->work);
		power_supply_unregister(&data->psy);
		power_supply_unregister(&data->bat);
		kfree(data);
	}
	return 0;
}

static int max8903_suspend(struct platform_device *pdev,
				  pm_message_t state)
{
	struct max8903_data *data = platform_get_drvdata(pdev);
	int irq;
	
	max8903_suspend_flag = 1;
	if (data) {
		struct max8903_pdata *pdata = data->pdata;
		if (pdata) {
			if (pdata->dc_valid) {
				irq = gpio_to_irq(pdata->dok);
				enable_irq_wake(irq);
			}
			if (pdata->usb_valid) {
				irq = gpio_to_irq(pdata->uok);
				enable_irq_wake(irq);
				}
			cancel_delayed_work(&data->work);
		}
	}
	return 0;
}

static int max8903_resume(struct platform_device *pdev)
{
	struct max8903_data *data = platform_get_drvdata(pdev);
	bool ta_in;
	bool usb_in;
	int irq;

	max8903_suspend_flag = 0;

	if (data) {
		struct max8903_pdata *pdata = data->pdata;
		if (pdata) {
			ta_in = gpio_get_value(pdata->dok) ? false : true;
			usb_in = gpio_get_value(pdata->uok) ? false : true;
			if (ta_in != data->ta_in) {
				data->ta_in = ta_in;
				pr_info("TA(DC-IN) Charger %s.\n", ta_in ?
				"Connected" : "Disconnected");
				max8903_charger_update_status(data);
				power_supply_changed(&data->psy);
			}
			if (usb_in != data->usb_in) {
				data->usb_in = usb_in;
				pr_info("USB Charger %s.\n", usb_in ?
				"Connected" : "Disconnected");
				max8903_charger_update_status(data);
				power_supply_changed(&data->usb);
			}

			data->chg_state = gpio_get_value(pdata->chg) ? false : true;

#ifdef CONFIG_BATTERY_MAX17048
			data->percent = max17048_get_soc_external();
			if(data->percent > 100)
			{
				data->percent = 100;
			}
			if (data->percent != data->old_percent) {
				power_supply_changed(&data->bat);
			}
#endif

			if (pdata->dc_valid) {
				irq = gpio_to_irq(pdata->dok);
				disable_irq_wake(irq);
			}
			if (pdata->usb_valid) {
				irq = gpio_to_irq(pdata->uok);
				disable_irq_wake(irq);
			}
			schedule_delayed_work(&data->work, BATTERY_UPDATE_INTERVAL);
		}
	}
	return 0;

}

static struct platform_driver max8903_driver = {
	.probe	= max8903_probe,
	.remove	= __devexit_p(max8903_remove),
	.suspend = max8903_suspend,
	.resume = max8903_resume,
	.driver = {
		.name	= "max8903-charger",
		.owner	= THIS_MODULE,
	},
};

static int __init max8903_init(void)
{
	return platform_driver_register(&max8903_driver);
}
module_init(max8903_init);

static void __exit max8903_exit(void)
{
	platform_driver_unregister(&max8903_driver);
}
module_exit(max8903_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("Sabresd Battery Driver");
MODULE_ALIAS("sabresd_battery");
