#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

#include "TST.h"
#include "TST_P.h"

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
    std::vector<const char*> cstrings;
    for(size_t i = 0; i < strings.size(); ++i)
        cstrings.push_back(const_cast<char*>(strings[i].c_str()));

    // build the tree
    tst_p::Tree<char,int> tree(cstrings);

    // print statistics
    cout << "TST have size: " << tree.size(tree.getRoot()) << " byte" << endl;
//    cout << "TST have " << tree.node_count(1) << " nodes" << endl;

    tree.compress(tree.getRoot());
    // print statistics
    cout << "after compression TST have size: " << tree.size(tree.getRoot()) << " byte" << endl;
//    cout << "TST have " << tree.node_count(1) << " nodes" << endl;
//    cout << "compressed node are: " << tree.compressed_count() << endl;

    return 0;
}