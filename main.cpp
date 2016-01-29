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

    cout << "Hello, World!" << endl;
    return 0;
}