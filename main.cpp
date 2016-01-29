#include <iostream>
#include <algorithm>
#include <vector>

#include "Trie.h"

using namespace std;

int main() {
    BasicTrie trie;
    trie.insert("pippo", 99);
    trie.insert("pluto", 44);

    unsigned int res = trie.find("pip");

    cout << "Hello, World!" << res << endl;
    return 0;
}