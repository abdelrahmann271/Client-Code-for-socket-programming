#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "http_parser.h"
#include "file_parser.h"
using std::ofstream;
using namespace std;

#define DEST_PORT 4000
#define DEST_IP "127.0.0.1"
#define DEFAULT_BUFLEN 9000

//cout<<"==>"<<<<"\n";

int main() {

    //Initialization

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

    vector<vector<string>> commands = parse_input_file("input.txt");



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


    //Executing commands..
    cout<<commands.size()<<"\n";
    //For every command do the following ..
    for(int i = 0 ; i < commands.size() ; i++){
        cout<<"========================================================================================\n";
        string method = commands[i][0];

        string file_name = commands[i][1];


        HTTP request;
        string body;

        if(method == "client-get"){

            request.set_method("GET");
            request.set_filename(file_name);
            string whole_request = request.build();


            //Get the file name with extension to write it.
            int lstInd;
            for (int i = 0; i < file_name.size(); i++) {
                if (file_name[i] == '/') {
                    lstInd = i;
                }
            }
            string path = file_name.substr(0, lstInd);
            string file = file_name.substr(lstInd + 1, file_name.size() - lstInd);
            cout << path << " " << file;


            char msg[DEFAULT_BUFLEN];
            int i =0;
            cout<<"\n"<<whole_request.size()<<"\n";
            for( ; i < whole_request.size(); i++){  // handle this
                msg[i] = whole_request[i];
            }

            int len, bytes_sent;
            len = strlen(msg);
            cout<<"len"<<i<<"\n";
            int recvbuflen = DEFAULT_BUFLEN;
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

            cout<<"==>"<<"message is sent to Server"<<"\n";

            //Receive message

//            char buf[DEFAULT_BUFLEN]={0};
//            int bytesReceived = recv(sockfd, buf, DEFAULT_BUFLEN, 0);
//            cout<<bytesReceived;
            char buffer[DEFAULT_BUFLEN];
            int bReceived = -1;
            HTTP recvieved;
            unordered_map<string,string> headers;
            string method;
            string file_name;
            string http_version;
            string body = "";
            string status="";
            cout<<"==>"<<"Waiting on receive"<<"\n";
            if ((bReceived = recv(sockfd,  buffer, DEFAULT_BUFLEN, MSG_PEEK)) > 0) {
                cout<<"==>"<<"Size of buffer"<<"\n";
                cout<<size_of_message(  buffer)<<"\n";;
                string http_version;
                parse_http(  buffer, headers,  http_version, status,body,bReceived);
                //int totalBytes = size_of_message(  buffer);
                int totalBytes = bReceived;
                recv(sockfd, buffer, totalBytes, 0);
                cout<<"==>"<<"Buffer Sent"<<"\n";
                for(int i = 0 ; i < totalBytes ; i++){
                    cout<<buffer[i];
                }
                int bodyLength = std::atoi(headers["Content-Length"].c_str());
                cout<<"==>"<<"Content Length"<<"\n";
                cout<<bodyLength<<"\n";
                cout<<"==>"<<"Received"<<"\n";
                cout<<body.size()<<"\n";
                bodyLength-=body.size();
                while (bodyLength > 0) {
                    int read = recv(sockfd, buffer, std::min(DEFAULT_BUFLEN, bodyLength), 0);
                    for (int i = 0; i < read; i++)
                    {
                        body +=  buffer[i];
                    }
                    bodyLength -= DEFAULT_BUFLEN;
                }
            }
            else if(bReceived == 0){
                int iResult = shutdown(sockfd, 2);
                break;
            }


            if (bReceived > 0)
            {
                cout<<"==>"<<"The messgage"<<"\n";
                for(int i = 0 ; i < bReceived ; i++){
                    cout<<buffer[i];
                }

//                unordered_map<string,string> headers;
//                string http_version;
//                string body;
//                string status_code;
//                parse_http(buf, headers,http_version,status_code,body,size_of_message(buf));

                ofstream MyFile;
                string createFile = "/Users/Abdelrahman Nour/CLionProjects/client/" + file;

                cout<<"\n"<<createFile<<"\n";
                MyFile.open(createFile.c_str(), std::ofstream::binary | std::ofstream::out);
                if(!MyFile){
                    cout<<"error file\n";
                    //Create the file..
                }
                else{
                    cout<<body.size();
                    MyFile << body;
                    // Close the file
                    MyFile.close();
                    cout<<"==>"<<"Response Received Successfully"<<"\n";
                }
            }
        }
        else if(method == "client-post"){

            string file_to_post = "/Users/Abdelrahman Nour/CLionProjects/client/" + file_name; // From Command
            cout<<file_name<<"\n";
            //string file_to_post =  "yoyo.jpg"; // From Command
            //char buffer_of_file[DEFAULT_BUFLEN]; /// tooooooooooooooooooooooooooooo
            std::ifstream input;
            input.open(file_to_post,ios::in | ios::binary);
            if(!input){
                cout << "==>" << "File Not Found .. \n";
            }
            else{
                HTTP request;
                request.set_method("POST");
                request.set_filename(file_name);

                string body = "";
                char ch;
                string line = "";
                int size_of_file_to_send = 0;
                //Reading the file into buffer
                while (1) {
                    input >>  std::noskipws  >> ch;
                    body+=ch;
                    size_of_file_to_send++;
                    if (input.eof())
                        break;
                    if(size_of_file_to_send==2000){
                        //create a new buffer
                    }
                }
                request.add_header("Content-Length", to_string(size_of_file_to_send));
                //cout<<to_string(size_of_file_to_send)<<"\n";
                request.add_body(body);
                string whole_request = request.build();
                //cout<<whole_request.size()<<"\n";

                int cnt = 0;
                while(cnt<whole_request.size()){
                    char buffer_of_file[DEFAULT_BUFLEN];
                    int i =0;
                    for( ; (i < DEFAULT_BUFLEN) && (cnt < whole_request.size() ); i++){  // handle this
                        buffer_of_file[i] = whole_request[cnt];
                        cnt++;
                    }
                    int iResult;
                    iResult = send(sockfd, buffer_of_file, i, 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("send failed: %d\n", WSAGetLastError());
                        closesocket(sockfd);
                        WSACleanup();
                        return 1;
                    }
                    printf("Bytes Sent: %ld\n", iResult);
                }


                char buf[DEFAULT_BUFLEN]={0};
                int bytesReceived = recv(sockfd, buf, DEFAULT_BUFLEN, 0);
                if (bytesReceived > 0)
                {
                    cout<<"==>"<<"The messgage"<<"\n";
                    for(int i = 0 ; i < bytesReceived ; i++){
                        cout<<buf[i];
                    }
//                    unordered_map<string,string> headers;
//                    string http_version;
//                    string body;
//                    string status_code;
//                    parse_http(buf, headers,http_version,status_code,body,size_of_message(buf));
                }

            }
            //}
        }
        else{
            cout<<"==>"<<"Method Not Supported or Error"<<"\n";
        }
    }


    //Send Message




// shutdown the connection for sending since no more data will be sent
// the client can still use the ConnectSocket for receiving data

//    bytes_sent = send(sockfd, msg,i, 0);
//    cout<<bytes_sent<<"\n";
//    if(bytes_sent == -1){
//        perror("bytes_sent error");
//    }


    int iResult = shutdown(sockfd, 2);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    cout<<"\nSafe";
    return 0;
}
