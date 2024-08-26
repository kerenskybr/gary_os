#ifndef CLASSIC_KEYBOARD_H
#define CLASSIC_KEYBOARD_H

#define PS2_PORT 0x64
#define PS2_COMMAND_ENABLE_FIRST_PORT 0xAE

#define CLASSIC_KEYBOARD_KEY_RELEASE 0x80
#define ISR_KEYBOARD_INTERRUPT 0x21
// https://wiki.osdev.org/%228042%22_PS/2_Controller
#define KEYBOARD_INPUT_PORT 0x60

struct keyboard* classic_init();

#endif