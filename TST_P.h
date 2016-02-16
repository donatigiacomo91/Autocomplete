//
// Created by Giacomo Donati on 12/02/16.
//

#ifndef AUTOCOMPLETE_TST_P_H
#define AUTOCOMPLETE_TST_P_H

#include <array>
#include <vector>
#include <string.h>

namespace tst_p {

    template<typename A,typename D>
    class Node {
    public:
        A character;
        D index;
        Node<A,D> *left, *middle, *right;

        D most_left_index;
        D most_right_index;

        Node(const A c) {
            character = c;
            left = middle = right = nullptr;
            index = -1;
            most_left_index = std::numeric_limits<D>::max();
            most_right_index = -1;
        }

        size_t size() {
            return sizeof(A) + sizeof(Node<A,D>*)*3 + sizeof(D)*3;
        }

    };

    template<typename A,typename D>
    class Tree {

        // dictionary must be sorted
        // otherwise we don't have elements with the same prefix contiguous
        std::vector<const A*> dictionary;
        Node<A,D>* root = nullptr;

        void insert(const A*, const D);
        void create(D, D);

    public:

        Node<A,D>* search(const A*);
        std::array<D,2> prefix(const A*);
        bool compress(Node<A,D>*);
        size_t size(Node<A,D>*);
        long node_count(Node<A,D>*);

        Node<A,D>* getRoot() {
            return root;
        };

        Tree(std::vector<const A*> &dic){
            dictionary = dic;
            create(0, dic.size());
        }

    };


    // TODO: compress also inner unitary path
    // compress the tree rooted in node by
    // compressing unitary path to leaf in one node (cutting the linear subtree)
    // mark the compressed leaf node with the special '\0' char
    // we assume that the dictionary do not contain string with '\0' character in the middle
    template<typename A,typename D>
    bool Tree<A,D>::compress(Node<A,D>* node) {

        if (node == nullptr) {
            return false;
        }

        if (node->left != nullptr || node->right != nullptr) {
            compress(node->left);
            compress(node->middle);
            compress(node->right);
            return false;
        }

        // leaf node
        if (node->middle == nullptr) {
            return true;
        }

        // internal node with only middle child
        if (compress(node->middle)) {
            Node<A,D>* child = node->middle;
            node->index = child->index;
            node->character = '\0'; // special mark for compressed node
            node->middle = nullptr;
            delete child; // the delete is called bottom-up so we do not need a recursive destructor
            return true;
        }
        // compression is not possible (this is not a unitary path to leaf)
        return false;

    }

    // return the size in byte of the tree rooted by node
    template<typename A,typename D>
    size_t Tree<A,D>::size(Node<A,D>* node) {

        if (node == nullptr) {
            return 0;
        }

        if (node->left == nullptr && node->middle == nullptr && node->right == nullptr ) {
            return node->size();
        }

        return node->size() + size(node->left) + size(node->middle) + size(node->right);
    }

    // return the number of nodes of the subtree rooted in "node"
    template<typename A,typename D>
    long Tree<A,D>::node_count(Node<A,D>* node) {

        if (node == nullptr) {
            return 0;
        }

        if (node->left == nullptr && node->middle == nullptr && node->right == nullptr) {
            return 1;
        }

        return 1 + node_count(node->left) + node_count(node->middle) + node_count(node->right);
    }

    // insert a word in the tree
    template<typename A,typename D>
    void Tree<A,D>::insert(const A* word, const D dic_index) {

        Node<A,D>* node;
        auto i = 0;

        if (root == nullptr) {
            root = new Node<A,D>(word[i]);
        }
        node = root;

        while (word[i] != 0) {

            node->most_left_index = std::min(dic_index, node->most_left_index);
            node->most_right_index = std::max(dic_index, node->most_right_index);

            if (word[i] < node->character) {
                if (node->left == nullptr) {
                    node->left = new Node<A,D>(word[i]);
                }
                node = node->left;

            }
            else if (word[i] == node->character) {
                i++;
                if (word[i] == 0) {
                    node->index = dic_index;
                    return;
                }
                if (node->middle == nullptr) {
                    node->middle = new Node<A,D>(word[i]);
                }
                node = node->middle;
            }
            else {
                if (node->right == nullptr) {
                    node->right = new Node<A,D>(word[i]);
                }
                node = node->right;
            }


        }
        node->index = dic_index;

    };

    // create a balanced tree if we start from a sorted dictionary
    // recursively insert the middle string in the sub-array [min,max]
    template<typename A, typename D>
    void Tree<A,D>::create(D min, D max) {
        if (min < max) {
            D mid = (min+max)/2;
            insert(dictionary[mid], mid);
            create(min, mid);
            create(mid+1, max);
        }
    }

    // return the index of the word in the dictionary
    // -1 if the string do not appears
    template<typename A,typename D>
    Node<A,D>* Tree<A,D>::search(const A* word) {

        Node<A,D>* node = root;
        auto i = 0;

        while (true) {

            if (node->character == '\0') {
                // special case (compressed unitary path)
                if (strcmp(word, dictionary[node->index]) == 0) {
                    return node;
                } else {
                    return nullptr;
                }
            }

            if (word[i] < node->character) {
                if (node->left == nullptr) {
                    return nullptr;
                }
                node = node->left;
            }
            else if (word[i] == node->character) {
                i++;
                if (word[i] == 0) {
                    return node;
                }
                if (node->middle == nullptr) {
                    return nullptr;
                }
                node = node->middle;
            }
            else {
                if (node->right == nullptr) {
                    return nullptr;
                }
                node = node->right;
            }

        }

    }

    template<typename A,typename D>
    std::array<D,2> Tree<A,D>::prefix(const A* pref) {
        std::array<D,2> result;
        Node<A,D>* node = search(pref);

        if (node != nullptr) {
            // all words prefixed by "pref" are in the middle subtree
            if (node->middle != nullptr) {
                result[0] = node->middle->most_left_index;
                result[1] = node->middle->most_right_index;
            }
            // if a word end in the node the range start from the target node
            if (node->index != -1) {
                result[0] = node->index;
            }

            return result;

        }

        return std::array<D,2> {-1,-1};

    }

}

#endif //AUTOCOMPLETE_TST_P_H
