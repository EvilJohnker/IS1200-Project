#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"


void i2c_idle() {
    /* Wait until Master transmit is not in progress and
    until SEN = 0; RSEN = 0; PEN = 0; RCEN = 0; ACKEN = 0. */
    while (I2C1CON & 0x1F || I2C1STAT & (1 << 14));
}

bool i2c_send(uint8_t data) {
    i2c_idle();

    /* Put the data being sent in the transmit register. */
    I2C1TRN = data;
    
    i2c_idle();
    
    /* Returns true if ACK received from slave,
    return false if NACK received from slave. */
    return !(I2C1STAT & (1 << 15));
}

uint8_t i2c_recv() {
    i2c_idle();

    /* Set receive enable bit to 1. This enables
    receive mode for I2C. */
    I2C1CONSET = 1 << 3;

    i2c_idle();

    /* Disable software or receive clock stretching. */
    I2C1STATCLR = 1 << 6;

    /* Returns the received data byte. */
    return I2C1RCV;
}

void i2c_ack() {
    i2c_idle();

    /* Send ACK during acknowledge. */
    I2C1CONCLR = 1 << 5;

    /* Initiates acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
    I2C1CONSET = 1 << 4;
}

void i2c_nack() {
    i2c_idle();

    /* Send NACK during acknowledge. */
    I2C1CONSET = 1 << 5;

    /* Initiates acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
    I2C1CONSET = 1 << 4;
}

void i2c_start() {
    i2c_idle();

    /* Initiates start condition on SDA and SCL pins. */
    I2C1CONSET = 0x1;

    i2c_idle();
}

void i2c_restart() {
    i2c_idle();

    /* Initiates repeated start condition on SDA and SCL pins. */
    I2C1CONSET = 1 << 1;

    i2c_idle();
}

void i2c_stop() {
    i2c_idle();

    /* Initiates stop condition on SDA and SCL pins. */
    I2C1CONSET = 1 << 2;
    i2c_idle();
}