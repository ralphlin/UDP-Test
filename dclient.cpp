/*********************************************************************
*
* dclient.cpp
*
* Written May, 2009
* By Ralph Lin
* University of Washington
* Neurobotics Lab
*
* Description:
* This program is used to test UDP socket communications to 
* communicate between EMG computer (Windows) to robot controller
* (RT Ubuntu) computer for RSS demo.
*
* It consists of two paired programs, dserver.cpp and dclient.cpp
* which receives and send messages with no error checking.
*
* Syntax: 
* dclient ServerName PortNumber
* where ServerName is host name running dserver and PortNumber is 
* the port where to use for communications
*
*********************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "WinSock.h"
#include <math.h>
using namespace std;

// Function prototype
void DatagramClient(char *szServer, short nPort);

// Helper macro for displaying errors
#define PRINTERROR(s)   \
               fprintf(stderr,"\n%: %d\n", s, WSAGetLastError())

#define MAX_COMM_PACKET sizeof(packet)

void main(int argc, char **argv)
{
  cout << "Press <ENTER> to start comms" << endl;
  cin.get();

  WORD wVersionRequested = MAKEWORD(1,1);
  WSADATA wsaData;
  int nRet;
  short nPort;

  // Check for the host and port arguments
  if (argc != 3)
  {
    fprintf(stderr,"\nSyntax: dclient ServerName PortNumber\n");
    return;
  }

  nPort = atoi(argv[2]);

  // Initialize WinSock and check the version
  nRet = WSAStartup(wVersionRequested, &wsaData);
  if (wsaData.wVersion != wVersionRequested)
  {
    fprintf(stderr,"\n Wrong version\n");
    return;
  }

  DatagramClient(argv[1], nPort);

  // Release WinSock
  WSACleanup();
}


void DatagramClient(char *szServer, short nPort)
{
  printf("\nDatagram Client sending to server: %s on port: %d",
                         szServer, nPort);

  // Find the server
  LPHOSTENT lpHostEntry;
  lpHostEntry = gethostbyname(szServer);
  if (lpHostEntry == NULL)
  {
     PRINTERROR("gethostbyname()");
     return;
  }

  // Create a UDP/IP datagram socket
  SOCKET  theSocket;

  theSocket = socket(AF_INET,                     // Address family
                    SOCK_DGRAM,          // Socket type
                    IPPROTO_UDP);        // Protocol
  if (theSocket == INVALID_SOCKET)
  {
    PRINTERROR("socket()");
    return;
  }

  // Fill in the address structure for the server
  SOCKADDR_IN saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);   // ^ Server's address
  saServer.sin_port = htons(nPort);       // Port number from command line

  // Send data to the server
  int nRet;
  char outbuff[150];

  int testOutput=1;

  while(1)
  {
    sprintf(outbuff, "%d",testOutput);
    printf("%d\n",testOutput);
    testOutput++;

    nRet = sendto(theSocket,                                // Socket
                             outbuff,                                      // Data buffer
                             strlen(outbuff),                      // Length of data
                             0,                                            // Flags
                             (LPSOCKADDR)&saServer,        // Server address
                             sizeof(struct sockaddr)); // Length of address
    if (nRet == SOCKET_ERROR)
    {
           PRINTERROR("sendto()");
           closesocket(theSocket);
           return;
    }

    // Wait for the reply
    /* int nFromLen;
    char inbuff[150];
    nFromLen = sizeof(struct sockaddr);
    recvfrom(theSocket,                                             // Socket
                    inbuff,                                                        // Receive buffer
                    sizeof(inbuff),                                        // Length of receive buffer
                    0,                                                             // Flags
                    (LPSOCKADDR)&saServer,                 // Buffer to receive sender's address
                    &nFromLen);                                    // Length of address buffer
    if (nRet == SOCKET_ERROR)
    {
      PRINTERROR("recvfrom()");
      closesocket(theSocket);
      return;
    }

    char outputText[150];
    sprintf(outputText, "received: %s", inbuff);
    printf("%s\n",outputText); */
  }

  closesocket(theSocket);
  return;
}

