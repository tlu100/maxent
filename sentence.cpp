#include "sentence.h"

sentence::sentence()
{
    //ctor
}

sentence::~sentence()
{
    //dtor
}

void sentence::pridej(word* slovo) {
    this->words.push_back(slovo);
}
