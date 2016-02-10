//
// Created by Giacomo Donati on 27/01/16.
//

#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef AUTOCOMPLETE_TRIE_H
#define AUTOCOMPLETE_TRIE_H


template <typename Alphabet_t, typename Pointer_t>
class BasicTrieNode {
public:

    // index in the dictionary of the word that end in the node (index in the dictionary must be start from 1)
    // value of 0 means: no word end in this node
    Pointer_t index;

    // index of the children in the tree vector (this must be alphabetically sorted to allow a binary search)
    vector<Pointer_t> children;
    vector<Alphabet_t> letters;

    Pointer_t most_left;
    Pointer_t most_right;

    BasicTrieNode(){
        index = 0;
        most_left = (Pointer_t) UINT64_MAX;
        most_right = 0;
    }

    size_t get_size() {

        size_t total_size = sizeof(this);

        total_size += (sizeof(Pointer_t)*children.size());
        total_size += (sizeof(Alphabet_t)*letters.size());

        return total_size;
    }

};

template<typename Alphabet_t, typename Pointer_t>
class BasicTrie {

    vector<BasicTrieNode<Alphabet_t, Pointer_t>*> trie;

public:

    vector<BasicTrieNode<Alphabet_t, Pointer_t>*> getTrieVector();

    void insert(const Alphabet_t* word, int len, Pointer_t index);

    Pointer_t find(const Alphabet_t* word, int len);

    array<Pointer_t,2> prefix(const Alphabet_t* word, int len);

    BasicTrie() {
        BasicTrieNode<Alphabet_t, Pointer_t>* root = new BasicTrieNode<Alphabet_t, Pointer_t>();
        trie.push_back(root);
    };

};

template<typename Alphabet_t, typename Pointer_t>
vector<BasicTrieNode<Alphabet_t, Pointer_t>*> BasicTrie<Alphabet_t,Pointer_t>::getTrieVector() {
    return trie;
}

// 0 -> fail
// >0 -> index of the word in the dictionary
template<typename Alphabet_t, typename Pointer_t>
Pointer_t BasicTrie<Alphabet_t,Pointer_t>::find(const Alphabet_t* word, int len) {

    Alphabet_t result_index;
    BasicTrieNode<Alphabet_t, Pointer_t>* node = trie.at(0);

    for (int j = 0; j < len; j++) {
        Alphabet_t c = word[j];

        // leaf node
        if (node->letters.size() == 0) {
            return 0;
        }
        // binary search
        typename vector<Alphabet_t>::iterator pos;
        pos = lower_bound(node->letters.begin(), node->letters.end(), c);
        result_index = pos - node->letters.begin();
        // check index out of range
        if (result_index > node->letters.size()-1) {
            return 0;
        }
        // get char and check the match
        Alphabet_t result = node->letters.at(result_index);
        if (result != c) {
            return 0;
        }
        // jump to child
        node = trie.at(node->children.at(result_index));
    }

    return node->index;
}

template<typename Alphabet_t, typename Pointer_t>
array<Pointer_t,2> BasicTrie<Alphabet_t,Pointer_t>::prefix(const Alphabet_t* word, int len) {

    Alphabet_t result_index;
    BasicTrieNode<Alphabet_t, Pointer_t>* node = trie.at(0);

    for (int j = 0; j < len; j++) {
        Alphabet_t c = word[j];

        // leaf node
        if (node->letters.size() == 0) {
            return array<Pointer_t,2> {0,0};
        }
        // binary search
        typename vector<Alphabet_t>::iterator pos;
        pos = lower_bound(node->letters.begin(), node->letters.end(), c);

        result_index = pos - node->letters.begin();
        // check index out of range
        if (result_index > node->letters.size()-1) {
            return array<Pointer_t,2> {0,0};
        }
        // get char and check the match
        char result = node->letters.at(result_index);
        if (result != c) {
            return array<Pointer_t,2> {0,0};
        }
        // jump to child
        node = trie.at(node->children.at(result_index));
    }

    array<Pointer_t,2> res;
    res[0] = node->most_left;
    res[1] = node->most_right;
    return  res;
}

// index in the dictionary must start from one (zero excluded)
template<typename Alphabet_t, typename Pointer_t>
void BasicTrie<Alphabet_t,Pointer_t>::insert(const Alphabet_t* word, int len, Pointer_t index) {

    Alphabet_t result_index;
    Pointer_t new_node_index;

    BasicTrieNode<Alphabet_t, Pointer_t>* new_node;
    BasicTrieNode<Alphabet_t, Pointer_t>* node = trie.at(0);

    vector<Pointer_t> path_index;
    // push root index
    path_index.push_back(0);


    for (int j = 0; j < len; j++) {
        Alphabet_t c = word[j];

        // leaf check
        if (node->letters.size() > 0) {
            // binary search over children
            typename vector<Alphabet_t>::iterator pos;
            pos = lower_bound(node->letters.begin(), node->letters.end(),c);

            Alphabet_t result = '\0';
            result_index = pos - node->letters.begin();
            // binary search can return vector size (mean that value is greater than all values inside)
            if (result_index <= node->letters.size()-1) {
                result = node->letters.at(result_index);
            }

            // if the letter not exist append a new leaf
            if (result != c) {
                new_node = new BasicTrieNode<Alphabet_t, Pointer_t>();
                trie.push_back(new_node);
                new_node_index = (Pointer_t) trie.size()-1;
                node->children.insert(node->children.begin()+result_index, new_node_index);
                node->letters.insert(node->letters.begin()+result_index, c);
            }
        } else {
            // node is a leaf -> append new leaf (binary search not necessary)
            new_node = new BasicTrieNode<Alphabet_t, Pointer_t>();
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
        BasicTrieNode<Alphabet_t, Pointer_t>* path_node = trie.at(path_index[i]);
        path_node->most_left = min(index, path_node->most_left);
        path_node->most_right = max(index, path_node->most_right);
    }

}


#endif //AUTOCOMPLETE_TRIE_H
