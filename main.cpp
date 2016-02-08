#include <iostream>
#include <algorithm>
#include <vector>

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

    std::vector<const char*> dic;
    std::string w0 = "";
    std::string w1 = "abc";
    std::string w2 = "abz";
    std::string w3 = "az";
    dic.push_back(w0.c_str());
    dic.push_back(w1.c_str());
    dic.push_back(w2.c_str());
    dic.push_back(w3.c_str());

    tst::Tree<char,int,int> tree(dic);


    return 0;
}