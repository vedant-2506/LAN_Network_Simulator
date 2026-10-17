#ifndef MS2503_FUN_H
#define MS2503_FUN_H

#include "MS2503_param.h"

#ifdef __cplusplus
extern "C" {
#endif

int RandomNumberGenerator(void);                     // Function to generate a random number
int initRandomSeed(unsigned long  seed);            // Function to initialize the random number generator with a seed

void ArrayReset(Port *port);                          // Function to reset the array of ports
void FullReset(int n_host);                            // Function to reset the host information
void ResetData(int host_id);                           // Function to reset the data for a specific host
void ResetHost(int host_id);                           // Function to reset the information for a specific host
int  Probability( int percentage);                   // Function to calculate the probability based on the number of hosts

void GenerateBytes(unsigned char packet[BUFFER_SIZE], int start_index, int end_index);                      // Function to generate random bytes for data transmission between sender and receiver hosts

void GenerateHost(int n_host);                       // Function to generate a host based on the number of hosts 
void GenerateData(int host_id);                      // Function to generate data for a specific host

void ConsumeData(int host_id);                       // Function to consume data for a specific host
void CopyData(Port  *source, Port *destination);          // Function to copy data from source port to destination port for a specific host
void SendData(int host_id);                          // Function to send data for a specific host

void Connect(Port *source, Port *destination , const char *source_name , const char *destination_name);                 // Function to connect source port to destination port for a specific host

void IPstrip(int host_id);                          // Function to strip the IP header from a packet for a specific host
void ArrayReset(Port *port);                          // Function to reset the array of ports
void FullReset(int n_host);                            // Function to reset the host information
void ResetData(int host_id);                           // Function to reset the data for a specific host
void ResetHost(int host_id);                           // Function to reset the information for a specific host
void ShowData(Port *port, int direction, const char *port_name);  // Function to show the data in a specific port
int  Probability( int percentage);                   // Function to calculate the probability based on the number of hosts

void GenerateBytes(unsigned char packet[BUFFER_SIZE], 
                    int start_index,
					int end_index);                      // Function to generate random bytes for data transmission between sender and receiver hosts

void PrintLine(const char *message, int host_id, int message_type);  // Function to print a message line for a specific host and message type

void SendOverLAN(int n_hosts);      // Function to simulate sending data over a LAN for a specific number of hosts
void RunNW_Simulation(void);       // Function to run the network simulation


#ifdef __cplusplus
}
#endif

#endif
