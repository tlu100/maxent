#ifndef SENTENCE_H
#define SENTENCE_H

#include <vector>
#include "word.h"

class sentence
{
    public:
        std::vector<word*> words;
        void pridej(word* slovo);
        sentence();
        ~sentence();
    protected:
    private:
};

#endif // SENTENCE_H
