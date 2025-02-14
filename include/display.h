#ifndef _DISPLAY_H
#define _DISPLAY_H

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

#include <stdint.h>

void sleep(int val);
uint8_t spi_send_recv(uint8_t data);

void display_init(void);
void display_string(int line, char *s);
void display_update(void);
void display_initial(void);
void display_info(void);
void display_temperature(float temp, const char *unit);

#endif