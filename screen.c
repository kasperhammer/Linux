#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <unistd.h>

#define I2C_BUS "/dev/i2c-2"  // I2C bus device file
#define I2C_ADDR 0x3e  // I2C address of the LCD screen
#define I2C_ADDR2 0x62  // I2C address of the LCD backlight

// Function to write a byte to a specific register of the I2C device
void i2c_write_byte(int file, unsigned char reg, unsigned char value) {
    unsigned char buffer[2];
    buffer[0] = reg;  // First byte: register address
    buffer[1] = value;  // Second byte: value to be written
    write(file, buffer, 2);  // Write the buffer to the I2C device
}

// Function to write a string to the LCD screen
void write_string_to_lcd(int file, const char *str) {
    size_t len = strlen(str);  // Get the length of the input string
    
    unsigned int lineControl = 0;  // Variable to control line switch
    for (size_t i = 0; i < len; i++) {
        if(i >= 16){  // If the string length exceeds 16 characters
            if(lineControl == 0){  // If it's the first line
                i2c_write_byte(file, 0x00, 0xC0);  // Move cursor to the beginning of the second line
                lineControl = 1;  // Update lineControl to indicate second line
            }
        } else {  // If the string length is within 16 characters
            lineControl = 0;  // Reset lineControl
        }
        i2c_write_byte(file, 0x40, str[i]);  // Write the character to the LCD screen
    }
}

// Function to initialize the LCD screen
void SetScreen(char *inputString){
    int file;
    char *filename = I2C_BUS;
    file = open(filename, O_RDWR);  // Open the I2C device file
    ioctl(file, I2C_SLAVE, I2C_ADDR);  // Set the I2C device to slave mode with the LCD address

    // Initialize LCD screen settings
    i2c_write_byte(file, 0x00, 0x28);  // Function set: 2 line mode and 5x8
    i2c_write_byte(file, 0x00, 0x0D);  // Display on/off control: Display on, cursor on, blink off
    i2c_write_byte(file, 0x00, 0x01);  // Clear display
    i2c_write_byte(file, 0x00, 0x06);  // Entry mode set
    i2c_write_byte(file, 0x00, 0x02);  // Return home
    
    const char *input_string = "No Input Given";  // Default string if no input is provided

    size_t len = strlen(inputString);  // Get the length of the input string
    if(len != 0){  // If input string is provided
        input_string = inputString;  // Update input_string
    }
  
    write_string_to_lcd(file, input_string);  // Write input_string to LCD screen

    close(file);  // Close the I2C device file
}

// Function to set the color of the LCD backlight
void SetColor(int color) {
    int file;
    char *filename = I2C_BUS;
    file = open(filename, O_RDWR);  // Open the I2C device file
    ioctl(file, I2C_SLAVE, I2C_ADDR2);  // Set the I2C device to slave mode with the backlight address

    // Turn sleep mode off
    i2c_write_byte(file, 0x00, 0x00);

    // Set backlight color based on the input parameter
    switch (color) {
        case 0:  // Blue background
            i2c_write_byte(file, 0x08, 0x01);
            break;
        case 1:  // Green background
            i2c_write_byte(file, 0x08, 0x04);
            break;
        case 2:  // Red background
            i2c_write_byte(file, 0x08, 0x10);
            break;
        default:  // No background color
            i2c_write_byte(file, 0x08, 0x15);
            break;
    }

    close(file);  // Close the I2C device file
}

// Global variable for the I2C device file
int file;

// Function to initialize the screen settings
void InitScreen(){
    char *filename = I2C_BUS;
    file = open(filename, O_RDWR);  // Open the I2C device file
    ioctl(file, I2C_SLAVE, I2C_ADDR);  // Set the I2C device to slave mode with the LCD address

    // Initialize LCD screen settings
    i2c_write_byte(file, 0x00, 0x28);  // Function set: 2 line mode and 5x8
    i2c_write_byte(file, 0x00, 0x0D);  // Display on/off control: Display on, cursor on, blink off
    i2c_write_byte(file, 0x00, 0x0C);  // Display clear
    i2c_write_byte(file, 0x00, 0x01);  // Entry mode set
    i2c_write_byte(file, 0x00, 0x02);  // Return home
}

// Function to write a string to the top line of the LCD screen
void WriteTopLine(char *str){
    i2c_write_byte(file, 0x00, 0x80);  // Move cursor to the beginning of the first line
    size_t len = strlen(str);  // Get the length of the input string
    for (size_t i = 0; i < len; i++) {
        i2c_write_byte(file, 0x40, str[i]);  // Write each character of the string to the LCD screen
    }
}

// Function to write a string to the bottom line of the LCD screen
void WriteBottomLine(char *str){
    i2c_write_byte(file, 0x00, 0xC0);  // Move cursor to the beginning of the second line
    for (int i = 0; i < 16; i++) {
        i2c_write_byte(file, 0x40, ' ');  // Write spaces to clear the line
    }
    i2c_write_byte(file, 0x00, 0xC0);  // Move cursor to the beginning of the second line
    size_t len = strlen(str);  // Get the length of the input string
    for (size_t i = 0; i < len; i++) {
        i2c_write_byte(file, 0x40, str[i]);  // Write each character of the string to the LCD screen
    }
}
