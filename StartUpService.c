#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "screen.h"
#include "IpGet.h"
#include <time.h>
#include <pthread.h>
#include <syslog.h>
#include "mqtt.h"
#include <mosquitto.h>

#define THREADS 2  // Number of threads used in the program
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct mosquitto *mosq1;
char *hugeString;  // Stores combined information (time and temperature)
char *timeString;  // Stores current time information
char *temp;  // Stores temperature information
char *ipString;  // Stores local IP address

// Function to print binary representation of an integer
int BitPrint(int bits) {
    for (int i = 15; i >= 0; i--) {
        if (bits & (1 << i)) {
            printf("1");
        } else {
            printf("0");
        }
        // Print a space between every 4 bits for readability
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

// Function to combine temperature and time information and display on the screen
void InterProlateAndPrint() {
    sprintf(hugeString,"%s %s", temp, timeString);  // Combine temperature and time strings
    WriteTopLine(hugeString);  // Display the combined string on the top line of the screen
    WriteBottomLine(ipString);  // Display the IP address on the bottom line of the screen
}

// Function to continuously get current time and update the timeString
void *getTime(void *arg) {
        pthread_mutex_lock(&mutex);
    int boolean = 0;
    char *tempTime;
    while (boolean == 0) {
        tempTime = GetTime(0);  // Get current time
        free(timeString);  // Free memory of previous timeString
        timeString = tempTime;  // Update timeString with current time
        InterProlateAndPrint();  // Display updated information on the screen
        pthread_mutex_unlock(&mutex);
        usleep(500000);  // Wait for 0.5 seconds
        pthread_mutex_lock(&mutex);
        tempTime = GetTime(1);  // Get current time in another format
        free(timeString);  // Free memory of previous timeString
        timeString = tempTime;  // Update timeString with current time
        InterProlateAndPrint();  // Display updated information on the screen
        pthread_mutex_unlock(&mutex);
        usleep(500000);  // Wait for 0.5 seconds
    }


    pthread_exit(NULL);  // Exit the thread
}

// Function to continuously get temperature information and update the temp variable
void *getTemp(void *arg) {

    int boolean = 0;
    char *tempTemp;
    while (boolean == 0) {
        pthread_mutex_lock(&mutex);
        tempTemp = GetTemp();  // Get current temperature
        free(temp);  // Free memory of previous temp
        temp = tempTemp;  // Update temp with current temperature
        syslog(LOG_INFO,temp);
        InterProlateAndPrint();  // Display updated information on the screen
        pthread_mutex_unlock(&mutex);
       
      
        usleep(10000000);  // Wait for 10 seconds
    }

    pthread_exit(NULL);  // Exit the thread
}

void *publishTemp(void *arg){
    
 int boolean = 0;
 while (boolean == 0) {
        pthread_mutex_lock(&mutex);
        publish_message(mosq1,temp);
        pthread_mutex_unlock(&mutex);
        usleep(30000000);  // Wait for 10 seconds
    }

}

int main(void) {
    pthread_t timeThread, tempThread, publishthread;  // Thread variables

    // Initialize
    init();  // Initialize system
    InitScreen();  // Initialize screen display

    hugeString = (char *)malloc(200 * sizeof(char));  // Allocate memory for hugeString
    timeString = (char *)malloc(100 * sizeof(char));  // Allocate memory for timeString
    temp = (char *)malloc(100 * sizeof(char));  // Allocate memory for temp
    ipString = (char *)malloc(100 * sizeof(char));  // Allocate memory for ipString
    sleep(10);
    ipString = GetLocalIp();  // Get local IP address

    openlog("TemperatureLog",LOG_PID,LOG_USER);
    mosq1 = GetMqtt();
    InitMqtt(mosq1);
    // Create time thread
    pthread_create(&timeThread, NULL, getTime, NULL);
    // Create temperature thread
    pthread_create(&tempThread, NULL, getTemp, NULL);

     pthread_create(&publishthread, NULL, publishTemp, NULL);

    // Wait for threads to finish
    pthread_join(timeThread, NULL);
    pthread_join(tempThread, NULL);
    pthread_join(publishTemp, NULL);
    // Free allocated memory
    free(hugeString);
    free(timeString);
    free(temp);
    free(ipString);

    DestroyMQTT(mosq1);

    return 0;
}
