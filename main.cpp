#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "http_parser.h"
using std::ofstream;
using namespace std;

#define DEST_PORT 4000
#define DEST_IP "127.0.0.1"
#define DEFAULT_BUFLEN 8196
//cout<<"==>"<<<<"\n";
int main() {


    //std::ifstream input( "eye.png", std::ios::binary );



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


    //Read from File
    //then execute commands..
    //For Every Command do the following ..

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

    string method = "POST"; //or "GET" from command
    string filename = "/test"; // from command

    //Send Message
    HTTP request;
    request.set_method("POST");
    request.set_filename(filename);
    string body;


    if(method == "GET"){

//        whole_request = head;
    }
    else{ // POST
        string file_to_post =  "hello.html"; // From Command
        std::ifstream input2( file_to_post , std::ios::binary );

        string str;
        while(getline(input2,str))
            body+=str;
//        for(int i = 0 ; i < body.size() ; i++){
//            cout<<body[i];
//        }
//        cout<<"\n"<<body.size()<<'\n';
//        whole_request = head+body;
        request.add_header("Content-Length", to_string(body.size()));
        request.add_header("Content-Type","text/html"); // From Command
        request.add_body(body);
    }
    string whole_request = request.build();
    char msg[10000];
    int i =0;
    for( ; i < whole_request.size(); i++){

//        if(whole_request[i]=='\0'){
//            //cout<<"\n7asal\n";
//            msg[i] = msg[i-1];
//        }
//        else{
            msg[i] = whole_request[i];
//        }
        //cout<<msg[i];
    }
//    char *msg = "POST /test HTTP/1.1\r\n\r\n";

    int len, bytes_sent;
    len = strlen(msg);
    cout<<"\nlen"<<i<<"\n";
    int recvbuflen = DEFAULT_BUFLEN;
    //const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
// Send an initial buffer
    iResult = send(sockfd, msg, (int) strlen(msg), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("Bytes Sent: %ld\n", iResult);
    iResult = send(sockfd, msg, (int) strlen(msg), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("Bytes Sent: %ld\n", iResult);
// shutdown the connection for sending since no more data will be sent
// the client can still use the ConnectSocket for receiving data

//    bytes_sent = send(sockfd, msg,i, 0);
//    cout<<bytes_sent<<"\n";
//    if(bytes_sent == -1){
//        perror("bytes_sent error");
//    }
    cout<<"==>"<<"message is sent to Server"<<"\n";
    //Receive Message
    char buf[DEFAULT_BUFLEN]={0};

    int bytesReceived = recv(sockfd, buf, DEFAULT_BUFLEN, 0);
    if (bytesReceived > 0)
    {
        cout<<"==>"<<"The messgage"<<"\n";
        cout<<buf<<"\n";
        unordered_map<string,string> headers;
        string http_version;
        string body;
        string status_code;
        parse_http(buf, headers,http_version,status_code,body,size_of_message(buf));
//        cout<<buf<<"\n";
//        cout<<"==>"<<"http_version\n";
//        cout<<http_version<<"\n";
//        cout<<"==>"<<"status code\n";
//        cout<<status_code<<"\n";
//        cout<<"==>"<<"body\n";
//        cout<<body<<"\n";
        if(method == "GET"){
            string file_name = "yoyo.html";
            // std::ofstream output( file_name, std::ios::binary );
            std::ofstream output;
            output.open(file_name);
            output<<body;
            cout<<"==>"<<"Response Received Successfully"<<"\n";
        }

    }

//    iResult = shutdown(sockfd, 2);
//    if (iResult == SOCKET_ERROR) {
//        printf("shutdown failed: %d\n", WSAGetLastError());
//        closesocket(sockfd);
//        WSACleanup();
//        return 1;
//    }
    cout<<"\nSsafe";
    return 0;
}
