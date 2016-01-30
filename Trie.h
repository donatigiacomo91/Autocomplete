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
unsigned int dictionary_len = UINT32_MAX;

class BasicTrieNode {
public:

    // index in the dictionary of the word that end in the node
    unsigned int index;

    // index of the children (this must be alphabetically sorted to allow a binary search)
    vector<unsigned int> children;
    vector<char> letters;

    unsigned int most_left = dictionary_len;
    unsigned int most_right = 0;

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
    array<unsigned int,2> prefix(string word);
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

array<unsigned int,2> BasicTrie::prefix(string word) {
    int len = word.length();
    unsigned int result_index;
    BasicTrieNode* node = trie.at(0);

    for (int j = 0; j < len; j++) {
        char c = word[j];

        // leaf node
        if (node->letters.size() == 0) {
            return array<unsigned int,2> {0,0};
        }
        // binary search
        vector<char>::iterator pos = lower_bound(node->letters.begin(), node->letters.end(),c);
        result_index = pos - node->letters.begin();
        // check index out of range
        if (result_index > node->letters.size()-1) {
            return array<unsigned int,2> {0,0};
        }
        // get char and check the match
        char result = node->letters.at(result_index);
        if (result != c) {
            return array<unsigned int,2> {0,0};
        }
        // jump to child
        node = trie.at(node->children.at(result_index));
    }

    return array<unsigned int,2> {node->most_left,node->most_right};
}

// index in the dictionary must start from one (zero excluded)
void BasicTrie::insert(string word, unsigned int index) {

    int len = word.length();
    unsigned int result_index;
    unsigned int new_node_index;
    BasicTrieNode* node = trie.at(0);
    BasicTrieNode* new_node;

    vector<unsigned int> path_index;
    // root index
    path_index.push_back(0);


    for (int j = 0; j < len; j++) {
        char c = word[j];

        // leaf check
        if (node->letters.size() > 0) {
            // binary search over children
            vector<char>::iterator pos = lower_bound(node->letters.begin(), node->letters.end(),c);

            char result = '\0';
            result_index = pos - node->letters.begin();
            // binary search can return vector size (mean that value is greater than all values inside)
            if (result_index <= node->letters.size()-1) {
                result = node->letters.at(result_index);
            }

            // if the letter not exist append a new leaf
            if (result != c) {
                new_node = new BasicTrieNode();
                trie.push_back(new_node);
                new_node_index = (unsigned int) trie.size()-1;
                node->children.insert(node->children.begin()+result_index, new_node_index);
                node->letters.insert(node->letters.begin()+result_index, c);
            }
        } else {
            // node is a leaf -> append new leaf (binary search not necessary)
            new_node = new BasicTrieNode();
            trie.push_back(new_node);
            new_node_index = (unsigned int) trie.size()-1;
            node->children.push_back(new_node_index);
            node->letters.push_back(c);
            result_index = 0;
        }

        // update node to continue the insertion process
        path_index.push_back(node->children.at(result_index));
        node = trie.at(node->children.at(result_index));

    }
    node->index = index;

    for (int i=0; i < path_index.size(); i++) {
        BasicTrieNode* path_node = trie.at(path_index[i]);
        path_node->most_left = min(index, path_node->most_left);
        path_node->most_right = max(index, path_node->most_right);
    }

}


#endif //AUTOCOMPLETE_TRIE_H
