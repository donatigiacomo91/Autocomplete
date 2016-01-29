#include <iostream>
#include <algorithm>
#include <vector>

#include "Trie.h"

using namespace std;

int main() {
    BasicTrie trie;
    trie.insert("pi",10);
    trie.insert("pa",20);
    trie.insert("pim",30);

    unsigned int res = trie.find("pz");

    cout << "Hello, World!" << res << endl;
    return 0;
}