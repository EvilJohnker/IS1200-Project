#ifndef _INIT_H
#define _INIT_H

#define NUM_MENU_ITEMS 4
#define TEMP_BUFFER_SIZE 2000

/* Temperature Sensor Address */
#define TEMP_SENSOR_ADDRESS 0x48

extern float temps[TEMP_BUFFER_SIZE];
extern int tempindex;

extern char* menu_items[NUM_MENU_ITEMS];
extern int menu_selection;
extern int period_time;
extern int period_unit;

float min, max, avg;

char string_buffer[32];
char period_buffer[32];
char text_buffer[4][16];

typedef enum {
    TEMP_SENSOR_REG_TEMP,
    TEMP_SENSOR_REG_CONF,
    TEMP_SENSOR_REG_HYST,
    TEMP_SENSOR_REG_LIMIT
} TempSensorReg;

void init(void);

#endif