#include "example.hpp"
#include <string>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <iostream>

int write(const char* x, int z){
    std::printf(x);
    return z;
}

void print(const char* x){
    std::printf(x);
}

void showtimme(const char* x, const char* y){
    std::printf("Id does nothing");

}

int main(){
    Shell shell;
    shell.setStringCompare(std::strcmp);
    shell.setCommand_print_Callback(print);
    shell.setCommand_showtime_Callback(showtimme);
    shell.setWrite(write);

    std::string s;
    while(std::getline(std::cin, s)){
        shell.execute(s.c_str(), s.length());
    }
}