//
// Created by Abdelrahman Nour on 11/9/2021.
//
#include <bits/stdc++.h>

using std::ofstream;
using namespace std;


vector<vector<string>>  parse_input_file(string path){

    //cout<<"eh1 "<<"\n";
    //cout<<path<<"\n";
    vector<vector<string>> commands;

    std::ifstream input( path , std::ios::in );
    string str;
    while(getline(input,str)){
        cout<<str<<"\n";
        vector<string> line_inf;
        //method - filepath+file - hostname - port
        char * cstr = new char [str.length()+1];
        std::strcpy (cstr, str.c_str());
//        cout<<cstr<<"\n";
        vector<char *> words;
        char *word = strtok(cstr, " ");
        while (word != NULL)
        {
//            cout<<"eeeeh\n";
            words.push_back(word);
            word = strtok(NULL, " ");
        }
//        cout<<words.size()<<"\n";
//        cout<<words[0]<<"\n";
//        cout<<words[1]<<"\n";
//        cout<<words[2]<<"\n";
//        cout<<words[3]<<"\n";
        for(int i = 0 ; i < words.size() ; i++){
//            cout<<words[i]<<" ";
            string g = string(words[i]);
            line_inf.push_back(g);
            //cout<<line_inf[i]<<"\n";
        }
        //cout<<"\n";
        commands.push_back(line_inf);
    }
    return commands;
}