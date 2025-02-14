#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>
#include <stdbool.h>

void i2c_idle(void);
bool i2c_send(uint8_t data);
uint8_t i2c_recv(void);
void i2c_ack(void);
void i2c_nack(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_stop(void);

#endif