//
// Created by Giacomo Donati on 28/01/16.
//
#include "catch.hpp"
#include "../Trie.h"

TEST_CASE( "Basic Trie Creation", "[trie]" ) {
    BasicTrie trie;
    trie.insert("pippo",1);
}