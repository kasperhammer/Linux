#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to retrieve the current hour
int getHour() {
    time_t rawtime;  // Variable to store the raw time
    struct tm *timeinfo;  // Structure to store time information

    time(&rawtime);  // Get the current time
    timeinfo = localtime(&rawtime);  // Convert the raw time to local time

    return timeinfo->tm_hour;  // Return the hour component of the current time
}

// Function to retrieve the current minute
int getMin() {
    time_t rawtime;  // Variable to store the raw time
    struct tm *timeinfo;  // Structure to store time information

    time(&rawtime);  // Get the current time
    timeinfo = localtime(&rawtime);  // Convert the raw time to local time

    return timeinfo->tm_min;  // Return the minute component of the current time
}

// Function to get the current time as a string
char* GetTime(int collon) {
    char *timeString = (char *)malloc(100 * sizeof(char));  // Allocate memory for the time string
    int timeHour = getHour();  // Get the current hour
    int timeMin = getMin();  // Get the current minute
    
    // Use sprintf to format the string based on the 'collon' parameter
    if (collon == 0) {
        sprintf(timeString, "%02d:%02d", timeHour, timeMin);  // Format the time string with colons
    } else {
        sprintf(timeString, "%02d %02d", timeHour, timeMin);  // Format the time string with spaces
    }
  
    return timeString;  // Return the formatted time string
}
