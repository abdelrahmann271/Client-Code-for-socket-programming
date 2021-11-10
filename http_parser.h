//
// Created by Abdelrahman Nour on 11/4/2021.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using std::ofstream;
using namespace std;


class HTTP{
    string status="";
    string method = "";
    unordered_map<string,string> headers;
    string body="";
    string filename;

public:
    HTTP(){

    }
    void set_method(string method_to_send){
        method = method_to_send;
    }
    void set_filename(string file){
        filename = file;
    }

    void set_status(int status_code){
        status = to_string(status_code);
    }
    void add_header(string header_name , string header_value){
        headers[header_name] = header_value;
    }
    void  add_body(string message){
        body = message;
    }
    string build(){
        string http = "";
        http+=method+" "+filename+" "+"HTTP/1.1\r\n";
        for (auto i = headers.begin(); i != headers.end(); i++){
            //cout<<i->first<<": "<<i->second<<"\r\n";
            http+=i->first+": "+i->second+"\r\n";
        }
        http+="\r\n";
        http+=body;
        //cout<<http<<" henaaa\n";
        return http;
    }


};

int size_of_message(char* buffer){
    int g = 0;
    while(buffer[g] != '\0'){
        g++;
    }
    return g;
}
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

vector<char *> get_lines(char *buffer){
    string s = buffer;
    vector<char *> lines;
    char *line = strtok(buffer, "\r\n");
    //lines.push_back(line);
    while (line != NULL)
    {
        lines.push_back(line);
        line = strtok(NULL, "\r\n");

    }
    return lines;
}

vector<char *> get_words(char * line){
    vector<char *> words;
    char *word = strtok(line, " ");
    //words.push_back(word);
    while (word != NULL)
    {
        words.push_back(word);
        word = strtok(NULL, " ");

    }
    return words;
}

void parse_http(char *message ,
                unordered_map<string,string> &headers ,
                string &http_version ,
                string &status_code ,
                string &body ,
                int size_of_message){

//    cout<<size_of_message<<"\n";
    int i =0;
    int end_of_headers;
    for( ; i < size_of_message ; i++){
        if(message[i] == '\r' && message[i+1] == '\n' && message[i+2] == '\r' && message[i+3] == '\n'){
            end_of_headers = i-1;
            break;
        }
    }
    int k = 0 ;
    char msg[end_of_headers+1];
    while(k!=end_of_headers+1){
        msg[k] = message[k];
        k++;
    }
    msg[k]='\0';
    int j = i+4;
//    int j = i+4;
//    while(message[j] != '\0' ){
//        body+=message[j];
//        j++;
//    }
    vector<char *> lines = get_lines(msg);
    vector<char *> words = get_words(lines[0]);

    http_version  = words[0];
    status_code = words[1];

    char *word;
    for(int i = 1 ; i < lines.size() ; i++){
        vector<char *> words;
        word= strtok(lines[i], ": ");
        while (word != NULL)
        {
            words.push_back(word);
            word = strtok(NULL, ": ");
        }
        headers[words[0]] = words[1];
    }
    int cnt = ( headers.find("Content-Length") == headers.end() )? 0 : stoi(headers["Content-Length"]);
    //cout<<headers["Content-Length"]<<" content length\n";
    int y = 0;
    while(y<cnt){
        body+=message[j];
        j++;
        y++;
    }


}