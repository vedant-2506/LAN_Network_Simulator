#ifndef MS2503_PARAM_H
#define MS2503_PARAM_H


#define MAX_HOSTES 26                   // Maximum number of hosts
#define MIN_HOSTS 3                     // Minimum number of hosts 
#define BUFFER_SIZE 256                 // Buffer size for data transmission
#define RANDOM_BUFFER_COUNT 8            // Number of random buffers for data transmission
#define MIN_DATA_BYTES 13               // Minimum number of data bytes for transmission
#define MAX_DATA_BYTES 20                // Maximum number of data bytes for transmission
#define PACKET_HEADER_BYTES 6           // Number of bytes in the packet header  
#define PROBABILITY_RANGE 20000         // Range of probability values

#define RANG_MULTIPLIR 1664525UL       // Multiplier for random number generation
#define RANG_INCREMENT 1013904223UL    // Increment for random number generation
#define RANG_MODULUS 4294967296UL      // Modulus for random number generation

typedef struct
{
    unsigned char d[2][BUFFER_SIZE];      // d[0] is output, d[1] is input
} Port;


typedef struct
{
    int host_id;                         // Unique identifier for the host
    int network_address;                  // Network address of the host
    int machine_address;                  // Machine address of the host
    char smac;                          // Source MAC address of the host
}HostInfo;


#endif 