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
    vector<BasicTrieNode*> getTrieVector();
    void insert(string word, unsigned int index);
    unsigned int find(string word);
    //array<unsigned int,2> prefix(string word);
};

BasicTrie::BasicTrie() {
    BasicTrieNode* root = new BasicTrieNode();
    trie.push_back(root);
}

vector<BasicTrieNode*> BasicTrie::getTrieVector() {
    return trie;
}

// 0 -> fail
// >0 -> index of the word in the dictionary
unsigned int BasicTrie::find(string word) {
    int len = word.length();
    unsigned int result_index;
    BasicTrieNode* node = trie.at(0);

    for (int j = 0; j < len; j++) {
        char c = word[j];

        // leaf node
        if (node->letters.size() == 0) {
            return 0;
        }
        // binary search
        vector<char>::iterator pos = lower_bound(node->letters.begin(), node->letters.end(),c);
        result_index = pos - node->letters.begin();
        // check index out of range
        if (result_index > node->letters.size()-1) {
            return 0;
        }
        // get char and check the match
        char result = node->letters.at(result_index);
        if (result != c) {
            return 0;
        }
        // jump to child
        node = trie.at(node->children.at(result_index));
    }

    return node->index;
}

// index in the dictionary must start from one (zero excluded)
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
