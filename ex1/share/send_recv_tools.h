#ifndef	SEND_RECV_TOOLS_H
#define	SEND_RECV_TOOLS_H
#include <winsock2.h>

int RecieveMsg(SOCKET socket, char* Buffer, int* MessageLen, SOCKADDR_IN* SenderAddr);
int SendMsg(SOCKET socket, char* BufferToSend, int BytesToSend, SOCKADDR_IN* RecvAddr);

#endif