#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

// Function to retrieve the local IP address of the system
char *GetLocalIp() {
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);  // Create a socket

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);  // Retrieve the IP address associated with the interface

    close(fd);  // Close the socket

    char *str;  // Pointer to store the IP address string
    str = (char *) malloc(20);  // Allocate memory for the string
    sprintf(str,"%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));  // Convert IP address to string format
    
    return str;  // Return the IP address string
}
