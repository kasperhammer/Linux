#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>        
#include <byteswap.h>

/* The MCP9808 Digital temperature sensor IC from Microchip is a i2c/SMBus compatible
 * device. To understand this code you need access to MCP9808 Datasheet from Microchip
 *
 * The device is implemented on Seeed Studios Grove I2C high accuracy Temperature Sensor
 *
 * Links and supplemental information can be found on the documentation link in this
 * file's header.
 *
 * The MCP9808 contains several registers:
 *
 * Register addresses:
 *  0x00: RFU    - (Reserved for future use) Read only
 *  0x01: CONFIG - Configuration register
 *  0x02: Tupper - Alert temperature Upper boundary trip register
 *  0x03: Tlower - Alert temperature Lower boundary trip register
 *  0x04: Tcrit  - Critical temperature trip register
 *  0x05: Ta     - Temperature register
 *  0x06:        - Manufacturer ID register (Reads 0x54)
 *  0x07:        - Device ID/Revision register
 *  0x08:        - Resolution register
 *  0x09-0x0F    - Reserved....
 */

#define CONFIG_REG      0x01
#define TUPPER_REG      0x02
#define TLOWER_REG      0x03
#define TCRIT_REG       0x04
#define TA_REG          0x05
#define MANID_REG       0x06
#define DEVID_REG       0x07
#define RES_REG         0x08
#define BIT_MASK_HEX    0x0FFF

// Device specific information. (Perhaps better as command options or config file)
#define MPC9808_BUS     "/dev/i2c-2"
#define MPC9808_ADR     0x18

// Function to initialize I2C communication
int i2c_init(char *bus, unsigned int address) {
    int file;
    file = open(bus, O_RDWR);  // Open the I2C bus
    ioctl(file, I2C_SLAVE, address);  // Set the device address
    return(file);  // Return the file descriptor
}

// Function to initialize the MCP9808 sensor and retrieve temperature
int init(){
    char revision; // To store MCP9808 revision ID - Perhaps useful in the future
    int file;
    char buf[20];
    int32_t reg32;
    uint16_t *const reg16poi = (uint16_t *)&reg32; // Address reg32 wordwise
    uint8_t *const reg8poi = (uint8_t *)&reg32;    // Address reg32 bytewise
    file = i2c_init(MPC9808_BUS, MPC9808_ADR);  // Initialize I2C communication with MCP9808 sensor

    // Read manufacturer ID
    reg32 = i2c_smbus_read_word_data(file, MANID_REG);  // Read the manufacturer ID register
    if (reg32 < 0) {
        fprintf(stderr, "ERROR: Read failed on i2c bus register %d - %s\n", MANID_REG, strerror(errno));
        exit(1);
    }
    if (bswap_16(reg16poi[0]) != 0x0054) { // Check manufacturer ID - Big endian 5400 and not 0054
        fprintf(stderr, "Manufacturer ID wrong is 0x%x should be 0x54\n", __bswap_16(reg16poi[0]));
        exit(1);
    }
    // Read device ID and revision
    reg32 = i2c_smbus_read_word_data(file, DEVID_REG);  // Read the device ID and revision register
    if (reg32 < 0) {
        fprintf(stderr, "ERROR: Read failed on i2c bus register %d - %s\n", DEVID_REG, strerror(errno));
        exit(1);
    }
    if (reg8poi[0] != 0x04) { // Check device ID - Big endian 0400 and not 0004
        fprintf(stderr, "Manufacturer ID OK but device ID wrong is 0x%x should be 0x4\n", reg8poi[0]);
        exit(1);
    }
    revision = reg8poi[1];

    /* DEBUG */
    printf("All good :-)\n");

    // Read temperature from MCP9808 sensor
    reg32 = i2c_smbus_read_word_data(file, TA_REG);  // Read the temperature register
    // Check if the reading was successful
    if (reg32 < 0) {
        fprintf(stderr, "ERROR: Reading failed\n");
        exit(1);
    }

    // Extract temperature value from reg32
    int temperature_raw = bswap_16(reg16poi[0]); // Temperature value in raw format

    // Convert raw value to Celsius
    double temperature_celsius = (temperature_raw & BIT_MASK_HEX) / 16.0; // Convert raw value to Celsius
    printf("Temperature: %.2f°C\n", temperature_celsius);

    return 0;
}

// Function to retrieve the temperature as a string
char* GetTemp(){
    int file;
    int32_t reg32;
    uint16_t *const reg16poi = (uint16_t *)&reg32; // Address reg32 wordwise
    uint8_t *const reg8poi = (uint8_t *)&reg32;    // Address reg32 bytewise
    file = i2c_init(MPC9808_BUS, MPC9808_ADR);  // Initialize I2C communication with MCP9808 sensor
    char *tempString = (char *)malloc(100 * sizeof(char));  // Allocate memory for temperature string

    // Read temperature from MCP9808 sensor
    reg32 = i2c_smbus_read_word_data(file, TA_REG);  // Read the temperature register
    // Check if the reading was successful
    if (reg32 < 0) {
        fprintf(stderr, "ERROR: Reading failed\n");
        exit(1);
    }

    // Extract temperature value from reg32
    int temperature_raw = bswap_16(reg16poi[0]); // Temperature value in raw format

    // Convert raw value to Celsius
    double temperature_celsius = (temperature_raw & BIT_MASK_HEX) / 16.0; // Convert raw value to Celsius
    // Format temperature as a string
    sprintf(tempString,"Temp %.1fC",temperature_celsius);

    return tempString;  // Return the temperature string
}
