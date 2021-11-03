#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using std::ofstream;
using namespace std;

#define DEST_PORT 4000
#define DEST_IP "127.0.0.1"
//cout<<"==>"<<<<"\n";
int main() {


    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
    cout<<"==>"<<"WSAStartup Success"<<"\n";
    //Get Socket.


    int sockfd;
    struct sockaddr_in dest_addr; // will hold the destination addr
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // do some error checking!
    if(sockfd == -1){
        perror("Error in socket");
    }
    cout<<"==>"<<"Socket Assignment Success"<<"\n";

    dest_addr.sin_family = AF_INET; // host byte order
    dest_addr.sin_port = htons(DEST_PORT); // short, network byte order
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    memset(&(dest_addr.sin_zero), '\0', 8); // zero the rest of the struct

    //Connect

    int dest_socket = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
    if(dest_socket == -1){
        perror("Error in connection");
    }
    cout<<"==>"<<"Client is connected to Server"<<"\n";



    //Send Message


    char *msg = "GET g.txt 192.168.1.68:3001 HTTP/1.1\r\n\r\n";
    int len, bytes_sent;
    len = strlen(msg);
    bytes_sent = send(sockfd, msg, len, 0);
    if(bytes_sent == -1){
        perror("bytes_sent error");
    }
    cout<<"==>"<<"message is sent to Server"<<"\n";
    //Receive Message
    char buf[2096];
    int bytesReceived = recv(sockfd, buf, 2096, 0);
    if (bytesReceived > 0)
    {
        // Echo response to console
        cout << "SERVER> " << string(buf, 0, bytesReceived);
        cout<<"==>"<<"Response Received Successfully"<<"\n";
    }

    return 0;
}
