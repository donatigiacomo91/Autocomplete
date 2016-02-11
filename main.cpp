#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

#include "TST.h"

using namespace std;

int main() {

    std::vector<std::string> strings;
    strings.reserve(25482);
    strings.push_back("");

    // read from dictionary
    std::ifstream file("../dictwords.txt");
    std::string str;
    long total_char = 0;
    long total_word = 0;
    while (std::getline(file, str)) {
        strings.push_back(str);
        total_char += str.length();
        total_word++;
    }
    float medium_len = (float) total_char / (float) total_word;

    cout << "number of character is : " << total_char << endl;
    cout << "number of word is : " << total_word << endl;
    cout << "medium word length is : " << medium_len << endl << endl;

    // convert to char*
    std::vector<char*> cstrings;
    for(size_t i = 0; i < strings.size(); ++i)
        cstrings.push_back(const_cast<char*>(strings[i].c_str()));

    // build the tree
    tst::Tree<char,int,int> tree(cstrings);

    cout << "TST have size: " << tree.size() << " byte" << endl;
    cout << "TST have " << tree.node_count(1) / (float) total_char << " nodes per character" << endl;

    tree.compact(1);
    cout << "after compact TST have " << (float) tree.node_count(1) / (float) total_char << " nodes per character" << endl;

    return 0;
}