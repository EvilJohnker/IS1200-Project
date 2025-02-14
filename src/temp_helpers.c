#include <stdint.h>
#include <pic32mx.h>
#include <math.h>
#include "temp_helpers.h"
#include "display.h"
#include "i2c.h"
#include "logic.h"
#include "init.h"

/* Function to calculate the average, minimum, and maximum temperature over a given period */
void period_calc(int period, float *avg, float *minp, float *maxp) {
    float sum = 0;  /* Sum of temperatures */
    int i;  /* Loop counter */
    int valid_count = 0;  /* Count of valid temperature readings */

    /* If the period is greater than the current index, adjust the period */
    if (period > tempindex)
        period = tempindex;

    /* Initialize min and max to the first temperature in the period */
    *minp = temps[(tempindex - period + TEMP_BUFFER_SIZE) % TEMP_BUFFER_SIZE];
    *maxp = temps[(tempindex - period + TEMP_BUFFER_SIZE) % TEMP_BUFFER_SIZE];

    /* Loop over the period */
    for (i = 0; i < period; i++) {
        /* Calculate the index, taking into account the circular buffer */
        int index = (tempindex - i + TEMP_BUFFER_SIZE) % TEMP_BUFFER_SIZE;
        float temp = temps[index];  /* Current temperature */

        /* If the temperature is valid (not equal to 1000.0) */
        if (temp != 1000.0f) {
            valid_count++;  /* Increment the count of valid readings */
            sum += temp;  /* Add the temperature to the sum */
            /* If this is the first valid reading, set min and max to this temperature */
            if (valid_count == 1) {
                *minp = temp;
                *maxp = temp;
            } else {
                /* Update min and max if necessary */
                if (temp > *maxp)
                    *maxp = temp;
                if (temp < *minp)
                    *minp = temp;
            }
        }
    }

    /* If there were any valid readings */
    if (valid_count > 0) {
        /* Calculate the average */
        *avg = sum / valid_count;
        /* Convert the temperatures to the desired unit */
        switch(period_unit) {
            case 0:  /* Celsius */
                break;
            case 1:  /* Fahrenheit */
                *avg = celsius_to_farenheit(*avg);
                *maxp = celsius_to_farenheit(*maxp);
                *minp = celsius_to_farenheit(*minp);
                break;
            case 2:  /* Kelvin */
                *avg = celsius_to_kelvin(*avg);
                *maxp = celsius_to_kelvin(*maxp);
                *minp = celsius_to_kelvin(*minp);
                break;
            default:
                break;
        }
    } else {
        /* If there were no valid readings, set all outputs to 0 */
        *minp = 0;
        *maxp = 0;
        *avg = 0;
    }
}

/* Convert 8.8 fixed floating point to floating point number. */
float fixed_to_float(uint16_t fixed) {
    /* Gets the 8 msb i.e. integer part. */
    int8_t integer_part = fixed >> 8;

    /* Gets the 8 lsb i.e. fractional part. */
    uint8_t fractional_part = fixed & 0xFF;

    float result;

    if (fixed & 0x8000) { // If negative
        result = (float)integer_part - ((float)fractional_part / 256.0f);
    } else {  // If positive
        result = (float)integer_part + ((float)fractional_part / 256.0f);
    }

    return result;
}

/* Handles user selection in the main menu. */
void user_selection() {
    float celsius;

    do {
        /* If push button 1 is pressed exit the loop. */
        if ((PORTF >> 1) & 1) {
            menu_selection = 0;
            display_initial();
            break;
        }
        switch (menu_selection) {
            case 0:
                celsius = read_temperature();
                display_temperature(celsius, "C");
                break;
            case 1:
                celsius = read_temperature();
                display_temperature(celsius_to_farenheit(celsius), "F");
                break;
            case 2:
                celsius = read_temperature();
                display_temperature(celsius_to_kelvin(celsius), "K");
                break;
            default:
                break;
        }
    } while (!(menu_selection == 3));

    /* Go to the period menu if user selected it in the main menu. 
    Otherwise go back to the main menu. */
    if (menu_selection == 3)
        period_menu();
    else
        main_menu();
}

/* Gets the temperature from the sensor and returns the value
as a floating point number. */
float read_temperature() {
    uint16_t temp;

    do {
        i2c_start();
    } while (!i2c_send(TEMP_SENSOR_ADDRESS << 1));  // write mode

    /* Which register where we want to read from. */
    i2c_send(TEMP_SENSOR_REG_TEMP);

    do {
        i2c_start();
    } while (!i2c_send((TEMP_SENSOR_ADDRESS << 1) | 1));  // read mode

    /* Temperature value is received in separate data bytes
    that are put together to the variable temp. */
    temp = i2c_recv() << 8;
    i2c_ack();
    temp |= i2c_recv();
    i2c_nack();
    i2c_stop();

    float temperature_as_float = fixed_to_float(temp);
    return temperature_as_float;
}

/* Convert floating point number to string. */
void float_to_string(float f) {
    /* if number is negative temp sign is '-'. */
    char *temp_sign = (f < 0) ? "-" : "";
    float temp = fabsf(f);

    int int_part = temp;
    float frac = temp - int_part;
    int frac_part = (int)(frac * 100);  // * 100, beacuse we want two decimals.

    snprintf(string_buffer, sizeof(string_buffer), "%s%d.%02d", temp_sign, int_part, frac_part);
}

/* Convert celsiusto farenheit. */
float celsius_to_farenheit(float celsius) {
    return (celsius * 9/5) + 32;
}

/* Convert celsius to kelvin. */
float celsius_to_kelvin(float celsius) {
    return celsius + 273.15;
}

/* Returns the four push button states. */
int get_btns() {
    int btn1 = (PORTF << 2) & 8;
    return ((PORTD >> 5) | btn1) & 0xf;
}

/* Determines which if C, F, or K will be displayed for the average, min, and max. */
void get_info() {
    int btns;

    do {
        btns = get_btns();
		switch (btns) {
			case 2:
				sleep(1000000);
				period_unit++;
				period_unit %= 3;
				display_info();
				break;
			case 4:
				sleep(1000000);
				period_unit--;
				if (period_unit < 0) period_unit = 2;
				display_info();
				break;
			default:
				break;
		}
    } while (!(btns == 8));

    menu_selection = 0;
    display_initial();
}

/* Concatinates user selected unit to the string buffer. */
void period_unit_char() {
		switch (period_unit) {
		case 0:
			strcat(string_buffer, " C,");
			break;
		case 1: 
			strcat(string_buffer, " F,");
			break;
		case 2: 
			strcat(string_buffer, " K,");
			break;	
		default: 
			break;
	}
}