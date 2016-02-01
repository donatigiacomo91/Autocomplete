//
// Created by Giacomo Donati on 28/01/16.
//
#include "catch.hpp"
#include "../Trie.h"

TEST_CASE( "Basic Trie One Word Insertion", "[trie]" ) {
    BasicTrie<char,unsigned int> trie;
    trie.insert("pippo",5,99);
    vector<BasicTrieNode<char,unsigned int>*> vec = trie.getTrieVector();

    REQUIRE(vec.size() == 6);
    REQUIRE(vec.at(0)->letters.at(0) == 'p');
    REQUIRE(vec.at(1)->letters.at(0) == 'i');
    REQUIRE(vec.at(2)->letters.at(0) == 'p');
    REQUIRE(vec.at(3)->letters.at(0) == 'p');
    REQUIRE(vec.at(4)->letters.at(0) == 'o');
    REQUIRE(vec.at(5)->index == 99);
}

TEST_CASE( "Basic Trie Two Word Insertion", "[trie]" ) {
    BasicTrie<char,unsigned int> trie;
    trie.insert("pi",2,99);
    trie.insert("pa",2,44);
    vector<BasicTrieNode<char,unsigned int>*> vec = trie.getTrieVector();

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

TEST_CASE( "Basic Trie Two Word Find", "[trie]" ) {
    BasicTrie<char,int> trie;
    trie.insert("pippo",5,99);
    trie.insert("pluto",5,44);

    REQUIRE(trie.find("pip",3) == 0);
    REQUIRE(trie.find("pippo",5) == 99);
    REQUIRE(trie.find("pippopluto",10) == 0);
    REQUIRE(trie.find("pluto",5) == 44);
}

TEST_CASE( "Basic Trie simulate slide n.24 Lez3-4-5.pdf", "[trie]") {
    BasicTrie<char,unsigned int> trie;
    trie.insert("ab",2,1);
    trie.insert("bab",3,2);
    trie.insert("bca",3,3);
    trie.insert("cab",3,4);
    trie.insert("cac",3,5);
    trie.insert("cbac",4,6);
    trie.insert("cbba",4,7);

    array<unsigned int,2> result = trie.prefix("c",1);
    REQUIRE(result[0] == 4);
    REQUIRE(result[1] == 7);

    result = trie.prefix("ca",2);
    REQUIRE(result[0] == 4);
    REQUIRE(result[1] == 5);

    result = trie.prefix("cbac",4);
    REQUIRE(result[0] == 6);
    REQUIRE(result[1] == 6);

    result = trie.prefix("cbacd",5);
    REQUIRE(result[0] == 0);
    REQUIRE(result[1] == 0);

    result = trie.prefix("cz",2);
    REQUIRE(result[0] == 0);
    REQUIRE(result[1] == 0);

}