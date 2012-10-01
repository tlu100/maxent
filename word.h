#ifndef WORD_H
#define WORD_H

#include <string>

class word
{
    public:
        std::string slovo;
        //word* pred;
        //word* nasl;
        bool isnum;

        word(std::string slovo);

        word();

        ~word();
    protected:
    private:
};

#endif // WORD_H
