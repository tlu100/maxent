#include "word.h"
#include <string>

word::word() {
    //ctor
}
word::word(std::string slovo) {
            this->slovo = slovo;
            //this->pred = pred;
            //this->nasl = nasl;
            this->isnum = false;
}
word::~word() {
    //dtor
}
