//
// Created by Giacomo Donati on 04/02/16.
//

#ifndef AUTOCOMPLETE_TST_H
#define AUTOCOMPLETE_TST_H

#include <vector>
#include <array>

namespace tst {

    template<typename A, typename P, typename D>
    class Node {
    public:
        A character;
        P left, middle, right;
        D index;

        // additional info to support prefix_search in the same time of search
        D most_left_index;
        D most_right_index;

        Node(const A c) {
            character = c;
            left = middle = right = 0;
            index = 0;
        }

        Node() {};

        size_t size() {
            return sizeof(A) + sizeof(P)*3 + sizeof(D)*3;
        }

    };

    template<typename A, typename P, typename D>
    class Tree {

        // dictionary must be sorted
        // otherwise we don't have elements with the same prefix contiguous
        std::vector<const A*> dictionary;
        std::vector<Node<A,P,D>*> vec;

    public:

        Tree() {};
        Tree(std::vector<const A*>&, std::vector<Node<A,P,D>*>&);

        D search(const A*);
        std::array<D,2> prefix(const A* pref);
        size_t size();

    };

    template<typename A, typename P, typename D>
    Tree<A,P,D>::Tree(std::vector<const A*>& dic, std::vector<Node<A,P,D>*>& vector1) {
        dictionary = dic;
        vec = vector1;
    };

    // return the size in byte of the whole tree
    template<typename A, typename P, typename D>
    size_t Tree<A,P,D>::size() {

        size_t total_size = 0;
        total_size += vec.size() * sizeof(Node<A,P,D>);
        total_size += vec.size() * sizeof(Node<A,P,D>*);
        total_size += sizeof(vec);

        return total_size;
    }


    // return the index of the word in the dictionary
    // 0 if the string do not appears
    template<typename A, typename P, typename D>
    D Tree<A,P,D>::search(const A* word) {

        Node<A,P,D>* node = vec[1];
        auto i = 0;

        while (true) {

            if (node->character == '\0') {
                // special case (compressed unitary path)
                if (strcmp(word, dictionary[node->index]) == 0) {
                    return node->index;
                } else {
                    return nullptr;
                }
            }

            if (word[i] < node->character) {
                if (node->left == 0) {
                    return 0;
                }
                node = vec[node->left];
            }
            else if (word[i] == node->character) {
                i++;
                if (word[i] == 0) {
                    return node->index;
                }
                if (node->middle == 0) {
                    return 0;
                }
                node = vec[node->middle];
            }
            else {
                if (node->right == 0) {
                    return 0;
                }
                node = vec[node->right];
            }

        }

    }

    // return the index of the minimum and maximum string
    // prefixed by "pref" in the form [min,max]
    // return [-1,-1] if "pref" don't belong to the dictionary
    template<typename A, typename P, typename D>
    std::array<D,2> Tree<A,P,D>::prefix(const A* pref) {
        std::array<D,2> result;
        D node_index = search(pref);

        if (node_index != 0) {

            Node<A,P,D>* node = vec[node_index];
            // all words prefixed by "pref" are in the middle subtree
            if (node->middle != 0) {
                // inner node
                result[0] = (node->index != -1) ? node->index : node->middle->most_left_index;
                result[1] = node->middle->most_right_index;
            } else {
                // leaf
                result[0] = node->most_left_index;
                result[1] = node->most_right_index;
            }

            return result;
        }

        return std::array<D,2> {-1,-1};
    }

}

#endif //AUTOCOMPLETE_TST_H
