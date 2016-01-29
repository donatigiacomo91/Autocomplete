//
// Created by Giacomo Donati on 27/01/16.
//

#include <string>
#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef AUTOCOMPLETE_TRIE_H
#define AUTOCOMPLETE_TRIE_H

// alpha -> alphabet size

class BasicTrieNode {
public:

    // index in the dictionary of the word that end in the node
    unsigned int index;

    // index of the children (this must be alphabetically sorted to allow a binary search)
    vector<unsigned int> children;
    vector<char> letters;

    BasicTrieNode();

};

BasicTrieNode::BasicTrieNode(){
    index = 0;
}

class BasicTrie {
    vector<BasicTrieNode*> trie;
public:
    BasicTrie();
    void insert(string word, unsigned int index);
    unsigned int find(string word);
    //array<unsigned int,2> prefix(string word);
};

BasicTrie::BasicTrie() {
    BasicTrieNode* root = new BasicTrieNode();
    trie.push_back(root);
}

unsigned int BasicTrie::find(string word) {
    return 0;
}

//void BasicTrie::insert(string word, unsigned int index) {
//
//    int len = word.length();
//    BasicTrieNode* node = trie[0];
//    BasicTrieNode* child = nullptr;
//
//    for (int j = 0; j < len; j++) {
//        char c = word[j];
//
//        // linear in alpha
//        // here we need i binary search
//        int size = node->children.size();
//        int pos;
//        for (pos = 0; pos < size; pos++) {
//            child = trie.at(node->children.at(pos));
//            if(c <= child->letter) {
//                break;
//            }
//        }
//
//        if (child == nullptr) {
//
//            child = new BasicTrieNode(c);
//            trie.push_back(child);
//            node->children.push_back(trie.size()-1);
//
//        } else if (c != child->letter) {
//            child = new BasicTrieNode(c);
//            trie.push_back(child);
//            node->children.insert(node->children.begin()+pos, trie.size()-1);
//
//        }
//        node = child;
//        child = nullptr;
//
//    }
//    node->index = index;
//
//}

void BasicTrie::insert(string word, unsigned int index) {

    int len = word.length();
    unsigned int result_index;
    BasicTrieNode* node = trie.at(0);
    BasicTrieNode* next_node;


    for (int j = 0; j < len; j++) {
        char c = word[j];

        if (node->letters.size() > 0) {
            vector<char>::iterator pos = lower_bound(node->letters.begin(), node->letters.end(),c);

            result_index = pos - node->letters.begin();
            char result = node->letters.at(result_index);

            if (result != c) {
                next_node = new BasicTrieNode();
                trie.push_back(next_node);
                unsigned int next_node_index = (unsigned int) trie.size()-1;
                node->children.insert(node->children.begin()+result_index, next_node_index);
                node->letters.insert(node->letters.begin()+result_index, c);
            }
        } else {
            next_node = new BasicTrieNode();
            trie.push_back(next_node);
            unsigned int next_node_index = (unsigned int) trie.size()-1;
            node->children.push_back(next_node_index);
            node->letters.push_back(c);
            result_index = 0;
        }

        node = trie.at(node->children.at(result_index));

    }
    node->index = index;

}


#endif //AUTOCOMPLETE_TRIE_H
