#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include <maxent/maxentmodel.hpp>

#include "fc.h"
#include "word.h"
#include "sentence.h"

using namespace maxent;
//using namespace std;

MaxentModel m;

std::ifstream in("/home/maltik/wmt12/dev/1.de");

std::vector<std::string> context;

std::vector<sentence*> vety;

int main () {

    // nacteni do vet a slov
    while (in.good()) {
        sentence* veta = new sentence();
        // predpoklada radek = veta
        while (in.peek() != '\n' && !in.eof()) {
            word* slovo = new word(getword(in));

            veta->pridej(slovo);
        }
        while (in.peek() == '\n') in.ignore();
        vety.push_back(veta);
    }

    // TODO: rozdeleni souveti na vety - pokud chceme posledni slovo vety

    // spojeni cisel, odstraneni mezer
    for (size_t i=0; i<vety.size(); i++) {
        sentence* s = vety[i];
        for (size_t j=0; j<s->words.size(); j++) {
            word* w = s->words[j];
            // spojeni cisel s desetinnou carkou
            if (j>0 && j < s->words.size()-1) {
                if (w->slovo == "," && (isNum(s->words[j-1]->slovo) || s->words[j-1]->isnum) && isNum(s->words[j+1]->slovo)) {
                    s->words[j-1]->slovo += "," + s->words[j+1]->slovo;
                    s->words.erase(s->words.begin()+j, s->words.begin()+j+2);
                    j--;
                }
                w = s->words[j];
                // spojeni cisel s mezerou oddelujici rad
                if (w->slovo == " " && (isNum(s->words[j-1]->slovo) || s->words[j-1]->isnum) && isNum(s->words[j+1]->slovo)) {
                    s->words[j-1]->slovo += " " + s->words[j+1]->slovo;
                    s->words[j-1]->isnum = true;
                    s->words.erase(s->words.begin()+j, s->words.begin()+j+2);
                    j--;
                }
                // TODO: dalsi moznosti, radove cislovky, ..............
                w = s->words[j];
                if (w->slovo == " ") s->words.erase(s->words.begin()+j);
            }
        }
    }

    // spocita cetnosti
    std::map<std::string, size_t> pocty;
    for (size_t i=0; i<vety.size(); i++) {
        sentence* s = vety[i];
        for (size_t j=0; j<s->words.size(); j++) {
            word* w = s->words[j];
            std::map<std::string, size_t>::iterator it;
            it = pocty.find(w->slovo);
            if (it == pocty.end())
                pocty.insert(pair<std::string, size_t>(w->slovo, 1));
            else
                it->second++;
        }
    }

    /* std::map<std::string, size_t>::iterator it;
    for (it = pocty.begin(); it != pocty.end(); it++)
        std::cout << it->first << "   " << it->second << std::endl;
        */

    m.begin_add_event();
    // posilani dat maxentu
    std::vector<std::string> context;
    for (size_t i=0; i<vety.size(); i++) {
        sentence* s = vety[i];
        for (size_t j=0; j<s->words.size(); j++) {
            word* w = s->words[j];
            std::map<std::string, size_t>::iterator it;
            it = pocty.find(w->slovo);
            // nahrazeni slov cetnosti 1 singletonem
            if (it->second == 1) w->slovo = "SINGLETON";
            if (s->words.size() > 1) context.push_back(s->words[s->words.size()-2]->slovo);
            if (j>1) {
                context.push_back(s->words[j-2]->slovo);
                context.push_back(s->words[j-1]->slovo);
                std::cout << "w2 " << s->words[j-2]->slovo << " w1 " << s->words[j-1]->slovo << " w " << s->words[j]->slovo << std::endl;
            }
            m.add_event(context, w->slovo, 1);
            context.clear();


        }
    }

    m.end_add_event();

    // natrenovani a ulozeni modelu
    m.train(1, "lbfgs");
    m.save("new_model");

    return 0;
}

