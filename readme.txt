# Project Title
Reading ambient temperature from a Temperature Sensor.

# Project Description
This project is designed to be able to read the temperature value from the
temperature sensor and display the value, in celsius, farenheit, and kelvin, on the display.
The program can also give the average, max, and min temperature over a user specified
time period between 1-2000 seconds.

# How to compile and run the program

## Compilation:
1. Make sure to be in the mcb32tools environment by
   writing the command: '. /opt/mcb32tools/environment' in the terminal.

2. Make sure to give the usb connection proper priveleges by
   running the command: 'sudo chmod 777 /tty/USB0'. (For Linux users)

3. Compile the code with 'make'

4. Install the code with 'make install'


## Running the program:

### Usage/User Interface
This program uses the following push buttons:
    - Push Button 4 (The left most button): Used for moving up in the menus. Also when displaying
                                            Average, min, and max this button is used to switch
                                            between C, F, and K units and which is displayed. 

    - Push Button 3 (The second left most button): Used for moving down in the menus.

    - Push Button 2 (The second right most button): Used for confirming the selection and
                                                    going to the choice. Also when displaying
                                                    Average, min, and max this button is used to switch
                                                    between C, F, and K units and which is displayed, but
                                                    in the other way as button 4.

    - Push Button 1 (The right most button): Used for going back to the main menu. However,
                                             in the period menu when selecting the period, it
                                             is used for toggling fast/slow mode for the period.

In the main menu intially the specific option of the user can
be made by using push buttons 4-2, moving up, down, and confirming the selection.
To go back to the main menu for selecting otheroptions push button 1 is used.

When choosing one of the options 'Display C, F, or K, the only button which can be used in this
page is button 1 which takes you back to the main menu.

In the period menu where the time period is selected Push button 4 and 3 are used for selecting the
time period between 1-2000 seconds. Push button 1 is used to toggle between fast and slow mode. This
increases the speed at which the time period increments and decrements on the display for faster
usage. Push button 2 is then used for confirming the selection.

When displaying the average, max, and min temperature. Push buttons 4 and 3 can be used for switching
between different units to display this information. Again button 1 is used to go back to the main menu.