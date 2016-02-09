#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

#include "TST.h"

using namespace std;

void gen_random(char *s, const int len) {
    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

int main() {

//    BasicTrie<char,unsigned int> trie;
//    char str[11];
//
//    long dic_size = 0;
//    for(int i=1; i < 1000; i++) {
//        int len = rand() % 8 + 3;
//        dic_size = dic_size + len;
//        gen_random(str,len);
//        trie.insert(str,len,i);
//    }
//    // convert to bit
//    dic_size = dic_size*8;
//
//    long trie_size = 0;
//    vector<BasicTrieNode<char,unsigned int>*> vec = trie.getTrieVector();
//    for (int j = 0; j < vec.size(); ++j) {
//        trie_size = trie_size + vec[j]->get_size();
//    }
//
//
//    cout << dic_size << endl << trie_size << endl << vec.size() << endl;

    std::vector<std::string> strings;
    strings.reserve(25482);
    strings.push_back("");

    std::ifstream file("../dictwords.txt");
    std::string str;
    while (std::getline(file, str)) {
        strings.push_back(str);
    }

    std::vector<char*> cstrings;

    for(size_t i = 0; i < strings.size(); ++i)
        cstrings.push_back(const_cast<char*>(strings[i].c_str()));

    tst::Tree<char,int,int> tree(cstrings);

    cout << tree.search("Staten") << endl;

    return 0;
}