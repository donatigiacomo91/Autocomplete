//
// Created by Giacomo Donati on 28/01/16.
//
#include "catch.hpp"
#include "../Trie.h"

TEST_CASE( "Basic Trie One Word", "[trie]" ) {
    BasicTrie trie;
    trie.insert("pippo",99);
    vector<BasicTrieNode*> vec = trie.getTrieVector();

    REQUIRE(vec.size() == 6);
    REQUIRE(vec.at(0)->letters.at(0) == 'p');
    REQUIRE(vec.at(1)->letters.at(0) == 'i');
    REQUIRE(vec.at(2)->letters.at(0) == 'p');
    REQUIRE(vec.at(3)->letters.at(0) == 'p');
    REQUIRE(vec.at(4)->letters.at(0) == 'o');
    REQUIRE(vec.at(5)->index == 99);
}

TEST_CASE( "Basic Trie Two Word", "[trie]" ) {
    BasicTrie trie;
    trie.insert("pi",99);
    trie.insert("pa",44);
    vector<BasicTrieNode*> vec = trie.getTrieVector();

    REQUIRE(vec.size() == 4);

    REQUIRE(vec[0]->letters[0] == 'p');

    unsigned int child = vec[0]->children[0];
    REQUIRE(vec[child]->letters.size() == 2);
    REQUIRE(vec[child]->letters[0] == 'a');
    REQUIRE(vec[child]->letters[1] == 'i');

    unsigned int left_child = vec[child]->children[0];
    unsigned int right_child = vec[child]->children[1];
    REQUIRE(vec[left_child]->index == 44);
    REQUIRE(vec[right_child]->index == 99);

}