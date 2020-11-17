#include <iostream>
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
// #pragma comment(lib, "Ws2_32.lib")

int main(){
////////////////////////////////////////////////////////////////
//socket流程
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"Error at WSAStartup()\n");
        return 1;
    }
    //socket()
    /*
    SOCKET WSAAPI socket(
        int af,//AF_INET IPv4
        int type,//SOCK_STREAM一種套接字類型，可通過OOB數據傳輸機制提供順序的，可靠的，雙向的，基於連接的字節流。此套接字類型將傳輸控制協議（TCP）用於Internet地址系列（AF_INET或AF_INET6）。
        int protocol//IPPROTO_TCP
    );
    */
    // Create a listening socket
    SOCKET server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    //bind()
    struct sockaddr_in saServer;
    // Set up the sockaddr structure
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = htonl(INADDR_ANY);//
    saServer.sin_port = htons(5150);
    struct sockaddr_in clientInfo;
    int addrlen = sizeof(clientInfo);
    // Bind the listening socket using the information in the sockaddr structure
    // Bind Socket with sockaddr
    int bind_result = bind(server,(SOCKADDR*) &saServer, sizeof(saServer) );
    if (bind_result == SOCKET_ERROR) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(server);
        WSACleanup();
        return 1;
    }
    else
        wprintf(L"bind returned success\n");
    //listen()
    // Listen for incoming connection requests on the created socket
    if (listen(server, 1) == SOCKET_ERROR)
        wprintf(L"listen function failed with error: %d\n", WSAGetLastError());
    wprintf(L"Listening on socket...\n");
    //accept()
    SOCKET Client;
     // Accept the connection.
    Client = accept(server, (struct sockaddr*)&clientInfo, &addrlen);
    if (Client == INVALID_SOCKET) {
        wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
        closesocket(server);
        WSACleanup();
        return 1;
    } else
        wprintf(L"Client connected.\n");
    //receive()
    char Menu[] = "-- Menu --\n1.Read all existing message\n2.Write a new message\nplease type '1' or '2' to select the option:\n";
    char option_1[] = "ALL MESSAGE :\n";
    char option_2[] = "Type your new Message here :\n";
    char recvbuf[1024];
    int recvbuflen = 1024;
    char database[10000];
    memset (database,NULL,sizeof(database));
    int d_length = 0;
    char empty = NULL;
    // Send an initial buffer
    // Receive until the peer closes the connection
    iResult = send(Client, Menu, strlen(Menu), 0);
    do {
        // iResult = shutdown(Client, SD_SEND);
        iResult = recv(Client, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ){
            printf("Bytes received: %d\n", iResult);
            if(recvbuf[0] == '1'){
                if(d_length == 0){
                    empty = '1';//empty
                    iResult = send(Client, &empty, 1, 0);
                }
                else{
                    empty = '0';//not empty
                    iResult = send(Client, &empty, 1, 0);
                    iResult = send(Client, database, strlen(database), 0);
                }
            }
            else if(recvbuf[0] == '2'){
                strncpy(&database[d_length], &recvbuf[1], recvbuflen-1);
                d_length = strlen(database);
                database[d_length]='\n';
                d_length = strlen(database);
            }
        }
        else if ( iResult <0)
            printf("recv failed: %d\n", WSAGetLastError());

        memset (recvbuf,NULL,sizeof(recvbuf));
        iResult = send(Client, Menu, strlen(Menu), 0);
    } while( iResult > 0 );

    //close()
    int close = closesocket(server);
    if (close == SOCKET_ERROR) {
        wprintf(L"closesocket function failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
////////////////////////////////////////////////////////////////
    return 0;
}


