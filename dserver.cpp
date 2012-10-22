/*********************************************************************
*
* dserver.cpp
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
* dserver PortNumber
* where PortNumber is the port where dserver should listen for
* packets from dclient
*
*********************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "winsock.h"
#include <math.h>
using namespace std;

// Function prototype
void DatagramServer(short nPort);

// Helper macro for displaying errors
#define PRINTERROR(s)   \
               fprintf(stderr,"\n%: %d\n", s, WSAGetLastError())

#define MAX_COMM_PACKET sizeof(packet)

void main(int argc, char **argv)
{
  cout << "Press <ENTER> to start the server." << endl;
  cin.get();

  WORD wVersionRequested = MAKEWORD(1,1);
  WSADATA wsaData;
  int nRet;
  short nPort;

  // Check for port argument
  if (argc != 2)
  {
    fprintf(stderr,"\nSyntax: dserver PortNumber\n");
    return;
  }

  nPort = atoi(argv[1]);

  // Initialize WinSock and check version
  nRet = WSAStartup(wVersionRequested, &wsaData);
  if (wsaData.wVersion != wVersionRequested)
  {
    fprintf(stderr,"\n Wrong version\n");
    return;
  }

  DatagramServer(nPort);

  // Release WinSock
  WSACleanup();
}

void DatagramServer(short nPort)
{
  // Create a UDP/IP datagram socket
  SOCKET theSocket;
  theSocket = socket(AF_INET,           // Address family
                    SOCK_DGRAM,        // Socket type
                    IPPROTO_UDP);      // Protocol
  if (theSocket == INVALID_SOCKET)
  {
    PRINTERROR("socket()");
    return;
  }

  // Fill in the address structure
  SOCKADDR_IN saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr.s_addr = INADDR_ANY; // Let WinSock assign address
  saServer.sin_port = htons(nPort);          // Use port passed from user

  int nRet;

  // bind the name to the socket
  nRet = bind(theSocket,                          // Socket descriptor
                         (LPSOCKADDR)&saServer,  // Address to bind to
                         sizeof(struct sockaddr) // Size of address
                         );
  if (nRet == SOCKET_ERROR)
  {
    PRINTERROR("bind()");
    closesocket(theSocket);
    return;
  }

  int nLen;
  nLen = sizeof(SOCKADDR);
  char szBuf[256];

  nRet = gethostname(szBuf, sizeof(szBuf));
  if (nRet == SOCKET_ERROR)
  {
    PRINTERROR("gethostname()");
    closesocket(theSocket);
    return;
  }

  // Show the server name and port number
  printf("\nServer named %s waiting on port %d\n", szBuf, nPort);

  // Wait for data from the client
  SOCKADDR_IN saClient;

  char inbuff[150];

  while(1)
  {
    nRet = recvfrom(theSocket,                              // Bound socket
                     inbuff,                                 // Receive buffer
                     sizeof(inbuff),                 // Size of buffer in bytes
                     0,                                              // Flags
                     (LPSOCKADDR)&saClient,  // Buffer to receive client address
                     &nLen);                                 // Length of client address buffer

    printf("\nValue: %s",inbuff);

    // Send data back to the client
    /* time=time+.001;
    for(int i=0;i<3;i++)
    {
      Packet1.phantomPos[i]=sin(time)+i;
      Packet1.velocity[i]=cos(time)+i;
    }
    char outbuff[150];
    sprintf(outbuff, "%f %f %f %f %f %f",Packet1.phantomPos[0],Packet1.phantomPos[1],Packet1.phantomPos[2],Packet1.velocity[0],Packet1.velocity[1],Packet1.velocity[2]);
    sendto(theSocket,                                               // Bound socket
              outbuff,                                                     // Send buffer
              strlen(outbuff),                                     // Length of data to be sent
              0,                                                           // Flags
              (LPSOCKADDR)&saClient,                       // Address to send data to
              nLen);                                                       // Length of address */

  }
  closesocket(theSocket);
  return;
}


