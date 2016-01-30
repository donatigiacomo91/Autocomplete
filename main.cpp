#include <iostream>
#include <algorithm>
#include <vector>

#include "Trie.h"

using namespace std;

int main() {

    BasicTrie trie;
    trie.insert("ab",1);
    trie.insert("bab",2);
    trie.insert("bca",3);
    trie.insert("cab",4);
    trie.insert("cac",5);
    trie.insert("cbac",6);
    trie.insert("cbba",7);

    array<unsigned int,2> result = trie.prefix("cbac");

    cout << "Hello, World!" << endl;
    return 0;
}