#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include <maxent/maxentmodel.hpp>

#include "../fc.h"
#include "../word.h"
#include "../sentence.h"


using namespace maxent;
//using namespace std;

MaxentModel m;

std::vector<std::string> context;

//std::ifstream in("/home/marek/wmt12/test/test.de");
std::ifstream in("/home/marek/wmt12/test/1.de");

int main()
{
    // nacteni ulozeneho modelu
    m.load("../new_model");
    // pocet uspechu v predikci
    int u = 0;
    // pocet slov
    int v = 0;
    // suma exponentu pro perplexitu
    double sum = 0;
    int pv = 0;

    // nacitani do konce souboru

    while (in.good()) {
        sentence* s = new sentence();
        while (in.peek() != '\n' && !in.eof()) {
            word* slovo = new word(getword(in));

            s->pridej(slovo);
        }
        while (in.peek() == '\n') in.ignore();

        for (size_t j=0; j<s->words.size(); j++) {
            word* w = s->words[j];
            if (j>0 && j < s->words.size()-1) {
                if (w->slovo == "," && (isNum(s->words[j-1]->slovo) || s->words[j-1]->isnum) && isNum(s->words[j+1]->slovo)) {
                    s->words[j-1]->slovo += "," + s->words[j+1]->slovo;
                    s->words.erase(s->words.begin()+j, s->words.begin()+j+2);
                    j--;
                }
                w = s->words[j];
                if (w->slovo == " " && (isNum(s->words[j-1]->slovo) || s->words[j-1]->isnum) && isNum(s->words[j+1]->slovo)) {
                    s->words[j-1]->slovo += " " + s->words[j+1]->slovo;
                    s->words[j-1]->isnum = true;
                    s->words.erase(s->words.begin()+j, s->words.begin()+j+2);
                    j--;
                }
                w = s->words[j];
                if (w->slovo == " ") s->words.erase(s->words.begin()+j);
            }
        }

        for (size_t j=0; j<s->words.size(); j++) {
            word* w = s->words[j];
            if (s->words.size() > 1) context.push_back(s->words[s->words.size()-2]->slovo);
            if (j>1) {
                context.push_back(s->words[j-2]->slovo);
                context.push_back(s->words[j-1]->slovo);
            }
            // nacteni pravdepodobnosti a kandidatu z modelu
            std::vector<pair<std::string, double> > probs;
            m.eval_all(context, probs);

            // pokud jsme se trefili, pricteme bod
            // TODO: vyresit SINGLETONY
            if (s->words[j]->slovo == probs[0].first) u++;

            // pricteni logaritmu k sume perplexity
            bool nalezeno = false;
            for (int p = 0; p < probs.size(); p++) {
                if (probs[p].first == s->words[j]->slovo) {
                 sum += log2(probs[p].second);
                 nalezeno = true;
                 break;
                }
            }
            if (!nalezeno) {
                sum += log2(probs[probs.size()-1].second/2.0);
            }

            //sum += log2(probs[0].second);
            v++;
            context.clear();
        }

        // vynechani prazdnych radku
        while (in.peek() == '\n') in.ignore();
        context.clear();
        pv++;
    }

    std::cout << "Pocet vet: " << pv << std::endl;
    std::cout << "Pocet predikci: " << v << std::endl;
    std::cout << "Presnost predikce: " << ((double)u/v) * 100 << "%" << std::endl;
    std::cout << "Perplexita: " << pow(2,-(double)(sum/v));

    return 0;
}
