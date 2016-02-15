#include <iostream>
#include <vector>
#include <fstream>

#include "TST.h"
#include "TST_P.h"

using namespace std;

int main() {

    std::vector<std::string> strings;
    strings.reserve(50000);

    // read from dictionary
    std::ifstream file("../dicttest.txt");
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
    std::vector<const char*> cstrings;
    cstrings.reserve(50000);
    for(size_t i = 0; i < strings.size(); ++i)
        cstrings.push_back(const_cast<char*>(strings[i].c_str()));

    // build the tree
    tst_p::Tree<char,int> tree(cstrings);

    // print statistics
    cout << "TST have size: " << tree.size(tree.getRoot()) << " byte" << endl;
    cout << "TST have " << tree.node_count(tree.getRoot()) << " nodes" << endl;

    cout << cstrings[0] << " have index: " << tree.search(cstrings[0]) << endl;

    tree.compress(tree.getRoot());
    // print statistics
    cout << "after compression TST have size: " << tree.size(tree.getRoot()) << " byte" << endl;
    cout << "after compression TST have " << tree.node_count(tree.getRoot()) << " nodes" << endl;

    cout << cstrings[3] << " have index: " << tree.search(cstrings[3]) << endl;

    return 0;
}