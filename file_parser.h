//
// Created by Abdelrahman Nour on 11/9/2021.
//
#include <bits/stdc++.h>

using std::ofstream;
using namespace std;


vector<vector<string>>  parse_input_file(string path){

    vector<vector<string>> commands;
    std::ifstream input( path , std::ios::in );
    string str;
    while(getline(input,str)){
        vector<string> line_inf;
        //method - filepath+file - hostname - port
        char * cstr = new char [str.length()+1];
        std::strcpy (cstr, str.c_str());
        vector<char *> words;
        char *word = strtok(cstr, " ");
        while (word != NULL)
        {
            words.push_back(word);
            word = strtok(NULL, " ");
        }
        for(int i = 0 ; i < words.size() ; i++){
            string g = string(words[i]);
            line_inf.push_back(g);
        }
        commands.push_back(line_inf);
    }
    return commands;
}