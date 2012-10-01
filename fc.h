#ifndef FC_H
#define FC_H

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

double log2(double x);

bool isNum(std::string value);
bool isNum(char c);

std::string getword(std::istream& i);

#endif // FC_H
