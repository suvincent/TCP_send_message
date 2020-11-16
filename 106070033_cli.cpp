#include <iostream>
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

int main(){
    char input;
    std::cout << "-- Menu --" << std::endl;
    std::cout << "1.Read all existing message" << std::endl;
    std::cout << "2.Write a new message" << std::endl;
    std::cout << "please type '1' or '2' to select the option:"<< std::endl;

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"Error at WSAStartup()\n");
        return 1;
    }

    SOCKET Client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    //bind()
    struct sockaddr_in saServer;
    // Set up the sockaddr structure
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    saServer.sin_port = htons(5150);

    // Connect to server.
    iResult = connect(Client, (SOCKADDR *) & saServer, sizeof (saServer));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
        iResult = closesocket(Client);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    wprintf(L"Connected to server.\n");

    char SendBuf[1024];
    char SendData[1024];
    memset (SendData,NULL,1024);
    memset (SendBuf,NULL,1024);
    int BufLen = 0;
    char newbuf[] = "";
    char recvbuf[1024];
    int recvbuflen = 1024;
    bool flag = false;
    while(std::cin >> input){
        if(input == '1'){//read all
            if(flag){
                std::cout << "ALL MESSAGE" << std::endl;
                SendData[0] = input ;
                iResult = send(Client, SendData, 1, 0);
                if (iResult == SOCKET_ERROR) {
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(Client);
                    WSACleanup();
                    return 1;
                }
                iResult = recv(Client, recvbuf, recvbuflen, 0);
                if(recvbuf[0] != '\0'){
                    recvbuf[strlen(recvbuf)-1] = '\0';
                    std::cout << recvbuf << std::endl;
                }
            }
            // input = NULL;
            memset (SendData,NULL,1024);
            memset (SendBuf,NULL,1024);
        }
        else if(input == '2'){//write
            // Connect to server.
            std::cout << "Type your new Message here :"<<std::endl;
            std::cin >> SendBuf;
            SendData[0] = input;
            strcat( SendData,  SendBuf);
            BufLen = (int)strlen(SendData); 
            // std::cout <<SendData<<std::endl;
            
            iResult = send(Client, SendData, (int) strlen(SendData), 0);
            if(!flag)flag = true;
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(Client);
                WSACleanup();
                return 1;
            }
            memset (SendData,NULL,1024);
            memset (SendBuf,NULL,1024);
            // input = NULL;
            printf("Bytes Sent: %ld\n", iResult);
        }
        else{
            std::cout << "wrong input command! plz type again" << std::endl;
        }
        std::cout << "-- Menu --" << std::endl;
        std::cout << "1.Read all existing message" << std::endl;
        std::cout << "2.Write a new message" << std::endl;
        std::cout << "please type '1' or '2' to select the option:"<< std::endl;
    }
    
    return 0;
}