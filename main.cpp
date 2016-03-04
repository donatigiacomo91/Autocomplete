#include <iostream>
#include <vector>
#include <fstream>
#include <random>

#include "Lib/Autocomplete.h"

using namespace std;

// CODE THAT GENERATE RANDOM SCORES
void gen_random_score() {
    std::ofstream out("../Data/dictwords_sc.txt");
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,1000); // distribution in range [1, 1000]
    for (int i=1; i<=25481; i++) {
        out << dist(rng) << endl;
    }
}

// CODE THAT GENERATE LCP
void lcp() {

    std::ifstream in("../Data/dictwords_lc.txt");
    std::ofstream out("../Data/dictwords_lcp.txt");

    std::string str1, str2;

    std::getline(in, str1);
    while (std::getline(in, str2)) {
        auto i = 0;
        while (i < str1.length() && i < str2.length() && str1[i]==str2[i])
            i++;
        out << i << std::endl;
        str1 = str2;
    }
}

int main() {

//    std::string str;
//    std::vector<std::string> strings;
//    strings.reserve(25482);
//
//    // read word from dictionary
//    std::ifstream words("../Data/dictwords_lc.txt");
//    long total_char = 0;
//    long total_word = 0;
//    while (std::getline(words, str)) {
//        strings.push_back(str);
//        total_char += str.length();
//        total_word++;
//    }
//    float medium_len = (float) total_char / (float) total_word;
//
//    cout << "number of character is : " << total_char << endl;
//    cout << "number of word is : " << total_word << endl;
//    cout << "medium word length is : " << medium_len << endl << endl;
//
//    // convert to char*
//    std::vector<const char*> cstrings;
//    cstrings.reserve(25482);
//    for(size_t i = 0; i < strings.size(); ++i)
//        cstrings.push_back(const_cast<char*>(strings[i].c_str()));
//
//
//    // read scores
//    std::ifstream words_score("../Data/dictwords_sc.txt");
//    std::vector<int> scores;
//    scores.reserve(25482);
//    while (std::getline(words_score, str)) {
//        int num = std::stoi(str);
//        scores.push_back(num);
//    }
//
//    // create the autocomplete system for the dictionary
//    atl::Autocomplete<char,int,long> system(cstrings,scores);
//    // get the top result for the specified prefix
//    std::vector<long> res = system.topK("abo",5);
//    for(auto i=0; i<res.size(); i++)
//        cout << scores[res[i]] << endl;

    return 0;
}