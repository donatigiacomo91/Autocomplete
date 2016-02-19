#include <iostream>
#include <vector>
#include <fstream>
#include <random>

#include "Top_K.h"

using namespace std;

int main() {

//    std::vector<std::string> strings;
//    strings.reserve(50000);
//
//    // read from dictionary
//    std::ifstream file("../dictwords.txt");
//    std::string str;
//    long total_char = 0;
//    long total_word = 0;
//    while (std::getline(file, str)) {
//        strings.push_back(str);
//        total_char += str.length();
//        total_word++;
//    }
//    float medium_len = (float) total_char / (float) total_word;
//
//    cout << "number of character is : " << total_char << endl;
//    cout << "number of word is : " << total_word << endl;
//    cout << "medium word length is : " << medium_len << endl << endl;
//
//    // convert to char*
//    std::vector<const char*> cstrings;
//    cstrings.reserve(50000);
//    for(size_t i = 0; i < strings.size(); ++i)
//        cstrings.push_back(const_cast<char*>(strings[i].c_str()));
//
//    // build the tree
//    tst_p::Tree<char,int> tree(cstrings);
//    tree.compress(tree.getRoot());
//
//    std::array<int,2> range = tree.prefix("nut");
//    cout << range[0] << "," << range[1] << endl;
//    for(int i=range[0]; i <= range[1]; i++)
//        cout << strings[i] << endl;

//    // print statistics
//    cout << "TST have size: " << tree.size(tree.getRoot()) << " byte" << endl;
//    cout << "TST have " << tree.node_count(tree.getRoot()) << " nodes" << endl;
//
//    cout << cstrings[0] << " have index: " << tree.search(cstrings[0]) << endl;
//
//    tree.compress(tree.getRoot());
//    // print statistics
//    cout << "after compression TST have size: " << tree.size(tree.getRoot()) << " byte" << endl;
//    cout << "after compression TST have " << tree.node_count(tree.getRoot()) << " nodes" << endl;
//
//    cout << cstrings[3] << " have index: " << tree.search(cstrings[3]) << endl;

//    std::ofstream out("../ditcwords_sc.txt");
//    std::mt19937 rng;
//    rng.seed(std::random_device()());
//    std::uniform_int_distribution<std::mt19937::result_type> dist(1,1000); // distribution in range [1, 1000]
//    for (int i=1; i<=25481; i++) {
//        out << dist(rng) << endl;
//    }

    // read from dictionary
    std::ifstream file("../dictwords_sc.txt");
    std::string str;
    std::vector<int> scores;
    scores.reserve(25481);
    while (std::getline(file, str)) {
        int num = std::stoi(str);
        scores.push_back(num);
    }

    top_k::K_Heap heap(scores);
    std::vector<int> res = heap.get(5,0,100);
    for(auto i=0;i<res.size();i++)
        cout << res[i] << endl;


    return 0;
}