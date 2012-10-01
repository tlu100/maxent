#include "fc.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

std::string getword(std::istream& i) {
    std::string out;
    while (i.peek() != ' ' && i.peek() != '"' && i.peek() != '(' && i.peek() != ')' && i.peek() != ',' && i.peek() != '.' && i.peek() != '!' && i.peek() != ':' && i.peek() != ';' && i.peek() != '?' && i.peek() != '\n')
        out+=i.get();
    if (out == "") {
        if (i.peek() == '.' || i.peek() == ',' || i.peek() != '!' || i.peek() != '?' || i.peek() != ':' || i.peek() != ';' || i.peek() != '"' || i.peek() != ')' || i.peek() != '(')
            out = i.get();
        else if (i.peek() == ' ') {
            while (i.peek() == ' ') i.ignore();
            out = ' ';
        }
    }
    return out;
}

bool isNum(std::string value) {
    for (size_t i = 0; i < value.length(); i++)
        if (value[i] > 47 && value[i] < 58) continue;
            else return false;
    return true;
}

bool isNum(char c) {
    return (c > 47 && c < 58);
}

double log2(double x) {
    return log(x) / log(2);
}
