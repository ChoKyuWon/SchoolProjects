#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/input-polldev.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>

#define WIINUNCHUK_JOY_FUZZ 4
#define WIINUNCHUK_JOY_FLAT 8

struct wiinunchuk_device {
	struct input_polled_dev *polled_input;
	struct i2c_client *i2c_client;
	int state;
};

static void wiinunchuk_poll(struct input_polled_dev *polled_input) {
	struct wiinunchuk_device *wiinunchuk = polled_input->private;
	struct i2c_client *i2c = wiinunchuk->i2c_client;
	uint8_t b[6];
	static uint8_t cmd_byte = 0;
	struct i2c_msg cmd_msg =
		{ .addr = i2c->addr, .len = 1, .buf = &cmd_byte };
	struct i2c_msg data_msg =
		{ .addr = i2c->addr, .flags = I2C_M_RD, .len = 6, .buf = b };
	int jx, jy, ax, ay, az;
	bool c, z;
	int e;

	switch (wiinunchuk->state) {
	case 0:
		/* insert your code here */
		e = i2c_transfer(i2c->adapter, &cmd_msg, 1);
		wiinunchuk->state = 1;
		break;

	case 1:
		/* insert your code here */
		e = i2c_transfer(i2c->adapter, &data_msg, 1);

		jx = b[0];
		jy = b[1];
		ax = (((u_int16_t)b[2]) << 2 | ((b[5] >> 2) & 0b11));
		ay = (((u_int16_t)b[3]) << 2 | ((b[5] >> 4) & 0b11));
		az = (((u_int16_t)b[4]) << 2 | ((b[5] >> 6) & 0b11));
		z = !(b[5] & 1);
		c = !(b[5] & 2);

		printk("wiinunchuk: j=%.3i,%.3i a=%.3x,%.3x,%.3x %c%c\n",
				jx, jy, ax, jy, az, c?'C':'c', z?'Z':'z');
		input_report_abs(polled_input->input, ABS_X, jx);
		input_report_abs(polled_input->input, ABS_Y, jy);
		input_report_abs(polled_input->input, ABS_RX, ax);
		input_report_abs(polled_input->input, ABS_RY, ay);
		input_report_abs(polled_input->input, ABS_RZ, az);
		input_report_key(polled_input->input, BTN_C, c);
		input_report_key(polled_input->input, BTN_Z, z);
		input_sync(polled_input->input);
		wiinunchuk->state = 0;
		break;

	default:
		wiinunchuk->state = 0;
	}
}

static void wiinunchuk_open(struct input_polled_dev *polled_input) {
	struct wiinunchuk_device *wiinunchuk = polled_input->private;
	struct i2c_client *i2c = wiinunchuk->i2c_client;
	static uint8_t data1[2] = { 0xf0, 0x55 };
	static uint8_t data2[2] = { 0xfb, 0x00 };
	struct i2c_msg msg1 = { .addr = i2c->addr, .len = 2, .buf = data1 };
	struct i2c_msg msg2 = { .addr = i2c->addr, .len = 2, .buf = data2 };
	int e;

	e = i2c_transfer(i2c->adapter, &msg1, 1);
	e = i2c_transfer(i2c->adapter, &msg2, 1);
}

static int wiinunchuk_probe(struct i2c_client *client, 
	const struct i2c_device_id *id) {
		struct wiinunchuk_device *wiinunchuk;
		struct input_polled_dev *polled_input;
		struct input_dev *input;
		int rc;

	wiinunchuk = devm_kzalloc(&client->dev, sizeof(struct wiinunchuk_device), GFP_KERNEL);
	if (!wiinunchuk)
		return -ENOMEM;

	polled_input = input_allocate_polled_device();
	if (!polled_input) {
		rc = -ENOMEM;
		goto err_alloc;
	}

	wiinunchuk->i2c_client = client;
	wiinunchuk->polled_input = polled_input;
	
	polled_input->private = wiinunchuk;
	polled_input->poll = wiinunchuk_poll;
	polled_input->poll_interval = 50;
	polled_input->open = wiinunchuk_open;

	input = polled_input->input;
	input->name = "Nintendo WiiNunchuk";
	input->id.bustype = BUS_I2C;
	input->dev.parent = &client->dev;

	set_bit(EV_ABS, input->evbit);
	set_bit(ABS_X, input->absbit);
	set_bit(ABS_Y, input->absbit);
	set_bit(ABS_RX, input->absbit);
	set_bit(ABS_RY, input->absbit);
	set_bit(ABS_RZ, input->absbit);

	set_bit(EV_KEY, input->evbit);
	set_bit(BTN_C, input->keybit);
	set_bit(BTN_Z, input->keybit);

	input_set_abs_params(input, ABS_X, 30, 220, 
			WIINUNCHUK_JOY_FUZZ, WIINUNCHUK_JOY_FLAT);
	input_set_abs_params(input, ABS_Y, 40, 200, 
			WIINUNCHUK_JOY_FUZZ, WIINUNCHUK_JOY_FLAT);
	input_set_abs_params(input, ABS_RX, 0, 0x3ff, 
			WIINUNCHUK_JOY_FUZZ, WIINUNCHUK_JOY_FLAT);
	input_set_abs_params(input, ABS_RY, 0, 0x3ff, 
			WIINUNCHUK_JOY_FUZZ, WIINUNCHUK_JOY_FLAT);
	input_set_abs_params(input, ABS_RZ, 0, 0x3ff, 
			WIINUNCHUK_JOY_FUZZ, WIINUNCHUK_JOY_FLAT);

	rc = input_register_polled_device(wiinunchuk->polled_input);
	if (rc) {
		printk("wiinunchuk: Failed to register input device\n");
		goto err_register;
	}

	i2c_set_clientdata(client, wiinunchuk);
	return 0;

err_register:
	input_free_polled_device(polled_input);
err_alloc:
	devm_kfree(&client->dev, wiinunchuk);
	printk("wiinunchuk: err_alloc... \n");
	return rc;
}

static int wiinunchuk_remove(struct i2c_client *client) {
	struct wiinunchuk_device *wiinunchuk = i2c_get_clientdata(client);

	i2c_set_clientdata(client, NULL);
	input_unregister_polled_device(wiinunchuk->polled_input);
	input_free_polled_device(wiinunchuk->polled_input);
	devm_kfree(&client->dev, wiinunchuk);
	return 0;
}

static const struct i2c_device_id wiinunchuk_id[] = {
	/* insert your code here */
	{"nunchuk",0},
	{}
};
MODULE_DEVICE_TABLE(i2c, wiinunchuk_id);

#ifdef CONFIG_OF
static const struct of_device_id wiinunchuk_dt_ids[] = {
	/* insert your code here */
	{.compatible = "nintendo, nunchuk",},
	{}
};
MODULE_DEVICE_TABLE(of, wiinunchuk_dt_ids);
#endif

static struct i2c_driver wiinunchuk_driver = {
	/* insert your code here */
	.probe		= wiinunchuk_probe,
	.remove		= wiinunchuk_remove,
	.id_table	= wiinunchuk_id,
	.driver ={
		.name	= "nunchuk",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(wiinunchuk_dt_ids),
	},
};

static int __init wiinunchuk_init(void) {
	/* insert your code here */
	int e;
	e = i2c_add_driver(&wiinunchuk_driver);
	if (e != 0) {
		printk("wiinunchuck: fail to add driver : %d\n", e);
	}
	else{
		printk("wiinunchuck: success to add driver (err: %d)\n", e);
	}
	return e;
}

static void __exit wiinunchuk_exit(void) {
	/* insert your code here */
	i2c_del_driver(&wiinunchuk_driver);
	printk("wiinunchuck: success to delete driver\n");
}

module_init(wiinunchuk_init);
module_exit(wiinunchuk_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("WiiNunchuk Driver");
MODULE_AUTHOR("ChoKyuWon");