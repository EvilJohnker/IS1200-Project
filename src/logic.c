#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <pic32mx.h>
#include "logic.h"
#include "display.h"
#include "temp_helpers.h"
#include "init.h"

void main_menu() {
    menu_selection = 0;
    period_time = 0;
    int pressed_btn;

    /* Loop until Push Button 3 is pressed to confirm selection. */
    do {
        pressed_btn = get_btns();
        sleep(10000);

        if (pressed_btn != 0) {
            switch (pressed_btn) {
                case 2:
                    menu_selection = (menu_selection + 1) % (sizeof(menu_items) / sizeof(menu_items[0]));
                    display_initial();
                    break;
                case 4:
                    menu_selection = (menu_selection - 1 + sizeof(menu_items) / sizeof(menu_items[0])) % (sizeof(menu_items) / sizeof(menu_items[0]));
                    display_initial();
                    break;
                default:
                    break;
            }
        }
    } while (!(pressed_btn == 1));

    sleep(100000);
    user_selection();
}

void period_menu() {
    int pressed_btn;
    sleep(1000000);
    bool toggle = false;
    period_unit = 0;

    display_string(0, "Period:");
    display_string(1, "0001");
    display_string(2, "Slow Mode");
    display_string(3, "");
    display_update();

    do {
        display_update();
        pressed_btn = get_btns();
        sleep(100000);

        /* Only check switch statement if a button was pressed, otherwise
        unnecessary. */
        if (pressed_btn != 0) {
            switch (pressed_btn) {
                case 2:
                    /* If in slow mode the sleep will trigger creating a longer delay
                    for the decrementing of the period time. */
                    if (!toggle) sleep(1000000);
                    period_time--;

                    /* Loop back around to 2000 after going less than 1. */
                    if (period_time < 1) period_time = 2000;
                    snprintf(period_buffer, sizeof(period_buffer), "%04d", period_time);
                    display_string(1, period_buffer);
                    sleep(10000);
                    break;
                case 4:
                    /* If in slow mode the sleep will trigger creating a longer delay
                    for the decrementing of the period time. */
                    if (!toggle) sleep(1000000);
                    period_time++;

                    /* Loop back around to 1 after going greater than 2000. */
                    if (period_time > 2000) period_time = 1;
                    snprintf(period_buffer, sizeof(period_buffer), "%04d", period_time);
                    display_string(1, period_buffer);
                    sleep(10000);
                    break;
                case 8:
                    sleep(500000);
                    toggle = !toggle;
                    if (toggle) {
                        display_string(2, "Fast Mode");
                        display_update();
                    } else {
                        display_string(2, "Slow Mode");
                        display_update();
                        sleep(1000000);
                    }
                    sleep(100000);
                    break;
                default:
                    break;
            }
        }
    } while (!(pressed_btn == 1));

    display_info();
    get_info();
    main_menu();
}

/* Timer ISR which reads temperature data every second and stores
the value in the temps array and is used for the period calculations. */
void ISR() {
    float temp = read_temperature();
    tempindex++;
    temps[tempindex] = temp;
    if (tempindex == 2000)
        tempindex = 0;

    /* Clear the interrupt. */
    IFSCLR(0) = (1 << 12);
}