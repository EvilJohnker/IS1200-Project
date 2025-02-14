#include "display.h"
#include "logic.h"
#include "init.h"

/* Starts program from here. */
int main(void) {
    init();
    display_initial();
    main_menu();

    return 0;
}