#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <set>

std::vector<std::string> podzielString(const std::string& str, char separator) {
    std::vector<std::string> tokeny;
    std::istringstream strumien(str);
    std::string token;

    while (std::getline(strumien, token, separator)) {
        tokeny.push_back(token);
    }

    return tokeny;
}

std::string usunZnaki(const std::string& str, const std::set<char>& znakiDoUsuniecia) {
    std::string wynik;

    for (char znak : str) {
        if (znakiDoUsuniecia.count(znak) == 0) {
            wynik += znak;
        }
    }

    return wynik;
}

void czytajLinie(std::vector<std::vector<int>>& graf, std::string linia) {
    std::vector<std::string> podzielonyString = podzielString(linia, ')');

    for (const auto& wspolrzedna : podzielonyString) {
        std::set<char> znakiDoUsuniecia = { '(', ' ', ',', ')' };
        std::string wspolrzedne = usunZnaki(wspolrzedna, znakiDoUsuniecia);

        if (wspolrzedne.size() == 2) {
            int start = std::stoi(wspolrzedne.substr(0, 1));
            int koniec = std::stoi(wspolrzedne.substr(1));

            std::vector<int> krawedz = { start, koniec };
            graf.push_back(krawedz);
        }
    }
}

void czytajPlik(std::string sciezkaPliku, std::vector<std::vector<int>>& graf) {
    std::ifstream plikWejsciowy(sciezkaPliku);

    if (!plikWejsciowy.is_open()) {
        std::cout << "Nie mo¿na otworzyæ pliku: " << sciezkaPliku << std::endl;
        return;
    }

    std::string linia;

    while (std::getline(plikWejsciowy, linia)) {
        czytajLinie(graf, linia);
    }

    plikWejsciowy.close();
}

int main() {
    std::vector<std::vector<int>> graf;

    std::string plikWejsciowy = "cos.txt";
    czytajPlik(plikWejsciowy, graf);

    for (const auto& wektor : graf) {
        std::cout << "(";
        for (int i = 0; i < wektor.size(); i++) {
            int wartosc = wektor[i];
            std::cout << wartosc;
            if (i < wektor.size() - 1) {
                std::cout << ",";
            }
        }
        std::cout << ")" << std::endl;
    }

    return 0;
}
