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
std::ifstream in("/home/marek/wmt12/dev/1.de");

int main()
{
    // nacteni ulozeneho modelu
    m.load("../new_model3");
    // pocet slov ve vete
    int i;
    // nactene slovo z dat
    std::string w;
    // pocet uspechu v predikci
    int u = 0;
    // pocet slov
    int v = 0;
    // suma exponentu pro perplexitu
    double sum = 0;
    int pv = 0;

    // nacitani do konce souboru
    while (in.good()) {
        i = 0;
        // nacitani po vetach
        while (in.peek() != '\n' && !in.eof()) {
            if (i > 0) context.push_back(w);
            if (i > 2) context.erase(context.begin()+1);
            // nacteni slova
            w = getword(in);
            // indikator cisla
            bool num = false;
            // slouceni cisel s mezerou mezi rady
            while ((isNum(w) || num) && in.peek() == ' ') {
                in.ignore();
                if (isNum(in.peek())) {
                    w += " ";
                    num = true;
                    while (isNum(in.peek())) w += in.get();
                }
            }
            // slouceni cisla s desetinnou carkou
            if ((isNum(w) || num) && in.peek() == ',') {
                in.ignore();
                if (isNum(in.peek())) {
                    w += ",";
                    while (isNum(in.peek())) w+=in.get();
                } else in.putback(',');
            }

            // TODO: dalsi moznosti, radove cislovky apod.

            // vynechani mezer
            while (w == " ") w = getword(in);

            // nacteni pravdepodobnosti a kandidatu z modelu
            std::vector<pair<std::string, double> > probs;
            m.eval_all(context, probs);

            // pokud jsme se trefili, pricteme bod
            // TODO: vyresit SINGLETONY
            if (w == probs[0].first /*|| probs[0].first == "SINGLETON"*/) u++;

            // pricteni logaritmu k sume perplexity
            bool nalezeno = false;
            for (int p = 0; p < probs.size(); p++) {
                if (probs[p].first == w) {
                 sum += log2(probs[p].second);
                 nalezeno = true;
                 break;
                }
            }
            if (!nalezeno) {
                sum += log2(probs[probs.size()-1].second/2.0);
            }
            v++;
            i++;

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
