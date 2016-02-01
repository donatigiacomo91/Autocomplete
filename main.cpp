#include <iostream>
#include <algorithm>
#include <vector>

#include "Trie.h"

using namespace std;

int main() {

    BasicTrie<char,unsigned int> trie;
    trie.insert("ab",2,1);
    trie.insert("bab",3,2);
    trie.insert("bca",3,3);
    trie.insert("cab",3,4);
    trie.insert("cac",3,5);
    trie.insert("cbac",4,6);
    trie.insert("cbba",4,7);

    array<unsigned int,2> result = trie.prefix("b",1);

    cout << "Hello, World!" << endl;
    return 0;
}