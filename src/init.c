#include <stdint.h>
#include <pic32mx.h>
#include "logic.h"
#include "i2c.h"
#include "display.h"
#include "init.h"

/* Initialized variables to be used in other source files. */
int menu_selection = 0;
int period_time = 1;
int period_unit = 0;
int tempindex = 0;

float temps[TEMP_BUFFER_SIZE] = { 1000.0 };

char* menu_items[NUM_MENU_ITEMS] = {
    "Display C",
    "Display F",
    "Display K",
    "Record Temp"
};

/* Initializes all registers and interfaces. */
void init() {
    SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
    OSCCONSET = 0x080000;
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */

    /* Configure all Analog-to-Digital input pins to digital pins */
    AD1PCFG = 0xffff;

    /* Disable Open Drain configuration for Register E */
    ODCE = 0x0;

    /* Output pins for display signals */
    /*
    RF1 = Push Button 1
    RF4 = OLED data/command select
    RF5 = OLED VBAT enable
    */
    PORTF = 0xFFFF;

    /* RG9 = OLED reset */
    PORTG = (1 << 9);

    /* Disable Open Drain configuration for Registers F and G */
    ODCF = 0x0;
    ODCG = 0x0;

    /* Set as Outputs */
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Push Buttons 2-4. */
    TRISDSET = (0x7 << 5);

    /* Set as input (this is for Push Button 1) */
    TRISFSET = (1 << 1);

    /* Initial clear of SPI Register */
    SPI2CON = 0;

    /* Set Baud Rate of the SPI to 4 MHz */
    SPI2BRG = 4;

    /* Clear SPIROV (Receive Overflow Flag bit)*/
    SPI2STATCLR &= ~0x40;

    /* Enable master mode for SPI and set clock polarity to idle state for clock is high level. MSTEN = 1; CKP = 1. */
    SPI2CONSET = 0x60;

    /* SPI Peripheral On bit. Turn on SPI */
    SPI2CONSET = 0x8000;

    /* Initial clear of control register */
    I2C1CON = 0x0;

    /* According to datasheet for the temperature sensor TCN75S the maximum frequency
    is 400 kHz so Baud rate should be less. Divide PB clock from 40 MHz down to acceptable
    frequency. */
    I2C1BRG = 0x0C2;

    /* Clear status register for I2C. */
    I2C1STAT = 0x0;

    /* SIDL = 1. Discontinue module operation when device enters idle mode. */
    I2C1CONSET = 1 << 13;

    /* ON = 1. Enable the I2C module. */
    I2C1CONSET = 1 << 15;

    /* Clear the receive register. */
    I2C1RCV = 0x0;

    /* Initial clear of the control registers. */
    T2CON = 0x0;
    T3CON = 0x0;

    /* Enable T32 = 1, for a 32-bit timer. This is for 
    better accuracy of 1 second. */
    T2CONSET = (1 << 3);
    
    /* Timer starts counting from 0. */
    TMR2 = 0x0;

    /* Count to this value in the period register. */
    PR2 = 156249;

    /* Set prescale to 1:256 and enable the timer. */
    T2CONSET = 0x8070;

    /* We configure interrupt for Timer3 bacause when an
    interrupt event occurs, the odd number timer generates the event. */
    IPCSET(3) = (0x4 << 2);
    IFSCLR(0) = (1 << 12);
    IECSET(0) = (1 << 12);

    /* This is used to set config register in the temp sensor to 0. */
    do {
        i2c_start();
    } while (!i2c_send(TEMP_SENSOR_ADDRESS << 1));

    i2c_send(TEMP_SENSOR_REG_CONF);
    i2c_send(0x0);
    i2c_stop();

    /* initialize display. */
    display_init();
    
    /* Enable interrupts globally. */
    enable_interrupt();
}