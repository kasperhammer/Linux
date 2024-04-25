# StartUpService.c Documentation
## Purpose
The purpose of the StartUpService.c file is to implement a program that starts up during system boot and runs in the background as a daemon. The program sets up and manages threads to collect and update time and temperature information, which is then displayed on a screen. Additionally, the temperature information is published to an MQTT broker.

## Functions
1. `int BitPrint(int bits)`
   This function prints a binary representation of an integer to the console. It is used for debugging or diagnostics.

2. `void InterpolateAndPrint()`
   This function combines temperature and time information and displays the combined string on a screen. It continuously updates the screen with the latest information.

3. `void *getTime(void *arg)`
   This thread function continuously retrieves the current time and updates a global variable with the time information. Then, it calls `InterpolateAndPrint()` to update the screen.

4. `void *getTemp(void *arg)`
   This thread function continuously retrieves the current temperature and updates a global variable with the temperature information. Then, it calls `InterpolateAndPrint()` to update the screen.

5. `void *publishTemp(void *arg)`
   This thread function continuously publishes the temperature information to an MQTT broker.

6. `int main(void)`
   This function is the entry point of the program. It initializes the system, creates threads for time, temperature, and MQTT publishing, and then exits the program.

## Daemon Implementation
To run StartUpService.c as a daemon during system boot, follow these steps:

Compile the code: Compile your StartUpService.c file into an executable file. For example, you can use the GCC command:
   ```bash
   gcc StartUpService.c -o StartUpService -lpthread
   ```
   Copy the executable file to the bin directory: Copy the compiled executable file (StartUpService) to /usr/local/bin or another appropriate location.
   ```bash
   sudo cp StartUpService /usr/local/bin
   ```
   Create a systemd service file: Create a systemd service file (e.g., startup.service) in the /etc/systemd/system/ directory with the following content:
   ```plaintext
   [Unit]
   Description=Startup Service
   After=network.target

   [Service]
   Type=simple
   ExecStart=/usr/local/bin/StartUpService

   [Install]
   WantedBy=multi-user.target
   ```
   Reload systemd daemons: Reload systemd daemons to update the service configurations.
   ```bash
   sudo systemctl daemon-reload
   ```
   Enable and start the service: Enable and start the new service.
   ```bash
   sudo systemctl enable startup.service
   sudo systemctl start startup.service
   ```

Now, StartUpService will run as a daemon during system boot and continue to run in the background, collecting time and temperature information, updating the screen, and publishing temperature information to the MQTT broker.

## Notes
Ensure you have the necessary permissions to run StartUpService as a daemon, and all dependencies are met.
You can adjust the service configuration as needed, such as specifying a different executable path or adding additional startup parameters.

# IpGet.c Documentation
## Purpose
The purpose of the IpGet.c file is to contain functions for retrieving the local IP address from the system. This is useful when connecting to a local network device or when needing to know the local IP address for other purposes.

## Functions
1. `char *GetLocalIp()`
   This function returns a string representing the local IP address of the system. The function creates a socket, retrieves the IP address from the desired network interface, and then formats the IP address as a string.

## Implementation
To use IpGet.c in your project, follow these steps:

Include the header file: Add the following line at the top of your source file to include IpGet.h:
   ```c
   #include "IpGet.h"
   ```
   Call the function: Use the `GetLocalIp()` function in your code where you need to retrieve the local IP address. Example:
   ```c
   char *ipAddress = GetLocalIp();
   printf("Local IP Address: %s\n", ipAddress);
   free(ipAddress); // Remember to free the memory after use
   ```
   Compile the source files: When compiling your project, make sure IpGet.c is included in the command.

## Notes
The `GetLocalIp()` function may return NULL if an error occurs while retrieving the IP address. It is recommended to check for the NULL value when using the function to avoid unwanted errors.
Ensure to free the allocated memory using `free()` after using the returned string to avoid memory leaks.

# mqtt.c Documentation
## Purpose
The purpose of the mqtt.c file is to contain functions for connecting to an MQTT broker, sending and receiving MQTT messages, and handling MQTT connection events.

## Functions
1. `void publish_message(struct mosquitto *mosq, const char *payload)`
   This function is used to publish a message to an MQTT broker. It takes a pointer to a mosquitto structure and a string as input and then publishes the message to the specified topic.

2. `void on_connect1(struct mosquitto *mosq, void *obj, int result)`
   This function serves as a callback function called when connecting to an MQTT broker. It can be used to perform additional actions after a successful connection.

3. `void on_message1(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)`
   This function serves as a callback function called when a new message is received from an MQTT broker. It receives a pointer to a mosquitto_message structure containing information about the received message and can be used to handle the message.

4. `struct mosquitto* GetMqtt()`
   This function initializes a new mosquitto structure and returns a pointer to the initialized structure. It is necessary to call this function before using other MQTT-related functions.

5. `int DestroyMQTT(struct mosquitto *mosq1)`
   This function releases resources allocated to a mosquitto structure. It is important to call this function when the MQTT object is no longer in use to avoid memory leaks.

6. `int InitMqtt(struct mosquitto *mosq1)`
   This function initializes the MQTT connection by connecting to an MQTT broker with the specified IP address and port number.

## Implementation
To use mqtt.c in your project, follow these steps:

Include the header file: Add the following line at the top of your source file to include mqtt.h:
   ```c
   #include "mqtt.h"
   ```
   Initialize MQTT: Use the `GetMqtt()` function to initialize an MQTT structure and the `InitMqtt()` function to connect to an MQTT broker.
   ```c
   struct mosquitto *mosq1 = GetMqtt();
   InitMqtt(mosq1);
   ```
   Use MQTT functions: Use the various MQTT functions such as `publish_message()` to interact with the MQTT broker.
   ```c
   publish_message(mosq1, "Hello, MQTT!");
   ```
   End MQTT connection: When finished using MQTT, release the resources by calling `DestroyMQTT()`.
   ```c
   DestroyMQTT(mosq1);
   ```

## Notes
Before using MQTT functions, ensure you have included mosquitto.h and configured your project to

 link with the Mosquitto library.
It is recommended to handle any errors that may occur during MQTT operations to ensure a robust application.

# screen.c Documentation
## Purpose
The purpose of the screen.c file is to implement functions for controlling an LCD screen via the I2C protocol. This file contains functions for writing text on the screen and setting the background color of the screen backlight.

## Functions
1. `void i2c_write_byte(int file, unsigned char reg, unsigned char value)`
   This function writes a byte to a specific register address on an I2C device. It is used to send commands and data to the LCD screen.

2. `void write_string_to_lcd(int file, const char *str)`
   This function writes a string to the LCD screen. If the string is longer than 16 characters, it automatically switches to the next line.

3. `void SetScreen(char *inputString)`
   This function initializes the LCD screen and displays a given string on the screen. It is used to start the screen and display an initial message.

4. `void SetColor(int color)`
   This function sets the color of the LCD screen backlight based on the received color parameter. Color options include blue, green, red, or no color.

5. `void InitScreen()`
   This function initializes the settings of the LCD screen, making it ready for use. It sets up the basic configurations of the screen such as two-line mode, blink control, etc.

6. `void WriteTopLine(char *str)`
   This function writes a string to the top line of the LCD screen.

7. `void WriteBottomLine(char *str)`
   This function writes a string to the bottom line of the LCD screen.

## Implementation
To use the functions in screen.c, follow these steps:

Include the header file: In your main file or the relevant program, include the screen.h header file.
   ```c
   #include "screen.h"
   ```
   Initialize the screen: Call the `InitScreen()` function to initialize the LCD screen.
   ```c
   InitScreen();
   ```
   Write text on the screen: Use the `write_string_to_lcd()` function to write text on the screen.
   ```c
   write_string_to_lcd(file, "Hello, World!");
   ```
   Set the background color: Use the `SetColor()` function to set the background color of the screen backlight.
   ```c
   SetColor(0); // Blue background
   ```
   Display on the screen: Use `WriteTopLine()` and `WriteBottomLine()` to write text on the top and bottom lines of the screen, respectively.
   ```c
   WriteTopLine("Temperature:");
   WriteBottomLine("25°C");
   ```

## Notes
Ensure you have the necessary permissions to use the I2C bus, and the screen is correctly connected to your device.
The functions in screen.c can be customized as needed, such as changing the screen's configurations or adding additional display options.

# temp.c Documentation
## Purpose
The purpose of the temp.c file is to implement functions to interact with an MCP9808 digital temperature sensor via the I2C protocol. This file contains functions for sensor initialization and reading the current temperature.

## Functions
1. `int i2c_init(char *bus, unsigned int address)`
   This function initializes communication with an I2C device by opening the corresponding I2C bus and setting the device's address. It returns a file descriptor used to communicate with the device.

2. `int init()`
   This function initializes the MCP9808 temperature sensor unit and returns the current temperature as a floating-point value in Celsius.

3. `char* GetTemp()`
   This function returns the current temperature as a string.

## Implementation
To use the functions in temp.c, follow these steps:

Include necessary header files: In your main file or the relevant program, include the temp.h header file.
   ```c
   #include "temp.h"
   ```
   Initialize the sensor: Call the `init()` function to initialize the MCP9808 sensor and get the current temperature.
   ```c
   double temperature = init();
   ```
   Read the temperature: Use the `GetTemp()` function to get the current temperature as a string.
   ```c
   char *temperatureString = GetTemp();
   ```

## Notes
Ensure you have the necessary permissions to use the I2C bus, and the sensor is correctly connected to your device.
The functions in temp.c can be customized as needed, such as changing the sensor's configurations or adding additional functionality.

# TimeGet.c Documentation
## Purpose
The purpose of the TimeGet.c file is to implement functions to retrieve the current time in hour and minute format. This is useful in situations where the application needs to work with timestamp data.

## Functions
1. `int getHour()`
   This function returns the current hour as an integer value.

2. `int getMin()`
   This function returns the current minute as an integer value.

3. `char* GetTime(int colon)`
   This function returns the current time in the format "HH:MM" or "HH MM" depending on the value of the colon parameter. The returned string contains the time as a character string.

## Implementation
To use the functions in TimeGet.c, follow these steps:

Include necessary header files: In your main file or the relevant program, include the TimeGet.h header file.
   ```c
   #include "TimeGet.h"
   ```
   Get hour and minute: Use the `getHour()` and `getMin()` functions to retrieve the current hour and minute.
   ```c
   int currentHour = getHour();
   int currentMin = getMin();
   ```
   Get time as string: Use the `GetTime()` function to get the current time as a string.
   ```c
   char *timeString = GetTime(1); // Returns time in the format "HH MM"
   ```

## Notes
The functions in TimeGet.c return timestamps based on the local time zone.
The `GetTime()` function allows choosing between two formats for the timestamp by specifying the value of the colon parameter (0 for "HH:MM" and 1 for "HH MM").
