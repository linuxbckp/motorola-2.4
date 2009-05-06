/*
 *
 * Apple Newton keyboard driver for Linux
 * by Justin Cormack <j.cormack@doc.ic.ac.uk>
 *
 */


#include <linux/slab.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>

MODULE_AUTHOR("Justin Cormack <j.cormack@doc.ic.ac.uk>");

#define NKBD_KEY	0x7f
#define NKBD_PRESS	0x80

static unsigned char nkbd_keycode[128] = {	 
  KEY_A, KEY_S, KEY_D, KEY_F, KEY_H, KEY_G, KEY_Z, KEY_X,
  KEY_C, KEY_V, 0, KEY_B, KEY_Q, KEY_W, KEY_E, KEY_R,
  KEY_Y, KEY_T, KEY_1, KEY_2, KEY_3, KEY_4, KEY_6, KEY_5,
  KEY_EQUAL, KEY_9, KEY_7, KEY_MINUS, KEY_8, KEY_0, KEY_RIGHTBRACE, KEY_O,
  KEY_U, KEY_LEFTBRACE, KEY_I, KEY_P, KEY_ENTER, KEY_L, KEY_J, KEY_APOSTROPHE,
  KEY_K, KEY_SEMICOLON, KEY_BACKSLASH, KEY_COMMA, KEY_SLASH, KEY_N, KEY_M, KEY_DOT,
  KEY_TAB, KEY_SPACE, KEY_GRAVE, KEY_BACKSPACE, 0, 0, 0, KEY_LEFTMETA,
  KEY_LEFTSHIFT, KEY_CAPSLOCK, KEY_LEFTALT, KEY_LEFTCTRL, KEY_RIGHTSHIFT, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_UP, 0
};

static char *nkbd_name = "Newton Keyboard";

struct nkbd {
  unsigned char keycode[128];
  struct input_dev dev;
  struct serio *serio;
};

void nkbd_interrupt(struct serio *serio, unsigned char data, unsigned int flags)
{
  struct nkbd *nkbd = serio->private;

  /* invalid scan codes are probably the init sequence, so we ignore them */
  if (nkbd->keycode[data & NKBD_KEY])
    input_report_key(&nkbd->dev, nkbd->keycode[data & NKBD_KEY], data & NKBD_PRESS);
  else if (data == 0xe7) /* end of init sequence */ 
    printk(KERN_INFO "input%d: %s on serio%d\n", nkbd->dev.number, nkbd_name, serio->number);

}

void nkbd_connect(struct serio *serio, struct serio_dev *dev)
{
	struct nkbd *nkbd;
	int i;

	if (serio->type != (SERIO_RS232 | SERIO_NEWTON))
	  return;

	if (!(nkbd = kmalloc(sizeof(struct nkbd), GFP_KERNEL)))
	  return;

	memset(nkbd, 0, sizeof(struct nkbd));
	
	nkbd->dev.evbit[0] = BIT(EV_KEY) | BIT(EV_REP);

	nkbd->serio = serio;

	nkbd->dev.keycode = nkbd->keycode;
	nkbd->dev.private = nkbd;

	serio->private = nkbd;

	if (serio_open(serio, dev)) {
		kfree(nkbd);
		return;
	}

	memcpy(nkbd->keycode, nkbd_keycode, sizeof(nkbd->keycode));
	for (i = 0; i < 128; i++)
		set_bit(nkbd->keycode[i], nkbd->dev.keybit);
	clear_bit(0, nkbd->dev.keybit);

	nkbd->dev.name = nkbd_name;
	nkbd->dev.idbus = BUS_RS232;
	nkbd->dev.idvendor = SERIO_NEWTON;
	nkbd->dev.idproduct = 0x0001;
	nkbd->dev.idversion = 0x0100;

	input_register_device(&nkbd->dev);

	printk(KERN_INFO "input%d: %s on serio%d\n", nkbd->dev.number, nkbd_name, serio->number);
}

void nkbd_disconnect(struct serio *serio)
{
	struct nkbd *nkbd = serio->private;
	input_unregister_device(&nkbd->dev);
	serio_close(serio);
	kfree(nkbd);
}

struct serio_dev nkbd_dev = {
	interrupt:	nkbd_interrupt,
	connect:	nkbd_connect,
	disconnect:	nkbd_disconnect
};

int __init nkbd_init(void)
{
	serio_register_device(&nkbd_dev);
	return 0;
}

void __exit nkbd_exit(void)
{
	serio_unregister_device(&nkbd_dev);
}

module_init(nkbd_init);
module_exit(nkbd_exit);
