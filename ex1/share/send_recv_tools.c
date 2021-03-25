#include <winsock2.h>
#include <stdio.h>
#include "common.h"
int RecieveMsg(SOCKET socket, char* Buffer, int* MessageLen, SOCKADDR_IN* SenderAddr) {
    int AddrSize =sizeof(SOCKADDR_IN);
    *MessageLen = recvfrom(socket, Buffer, MAX_BUFFER_SIZE, 0, (SOCKADDR*)SenderAddr, &AddrSize);

    if (*MessageLen == SOCKET_ERROR) {
        fprintf(stderr,"-ERROR- recvfrom failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    if (*MessageLen < MAX_BUFFER_SIZE) {
        Buffer[*MessageLen] = '\0';
    }
    else {
        fprintf(stderr, "-ERROR- buffer is too small!\n");
        return FAIL;
    }
    return 0;
}

int SendMsg(SOCKET socket, char* BufferToSend, int BytesToSend, SOCKADDR_IN* RecvAddr) {
    int AddrSize = sizeof(*RecvAddr);
    const char* CurPlacePtr = BufferToSend;
    int BytesTransferred;
    int RemainingBytesToSend = BytesToSend;

    while (RemainingBytesToSend > 0)
    {
        BytesTransferred = sendto(socket, BufferToSend, BytesToSend, 0, (SOCKADDR*)RecvAddr, AddrSize);
        if (BytesTransferred == SOCKET_ERROR)
        {
            fprintf(stderr, "-ERROR- sendto() failed, error %d\n", WSAGetLastError());
            return FAIL;
        }

        RemainingBytesToSend -= BytesTransferred;
        CurPlacePtr += BytesTransferred; 
    }
    return 0;
}