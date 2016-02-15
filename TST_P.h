//
// Created by Giacomo Donati on 12/02/16.
//

#ifndef AUTOCOMPLETE_TST_P_H
#define AUTOCOMPLETE_TST_P_H

#include <vector>
#include <string.h>

namespace tst_p {

    template<typename A,typename D>
    class Node {
    public:
        A character;
        D index;
        Node<A,D> *left, *middle, *right;

        Node(const A c) {
            character = c;
            left = middle = right = nullptr;
            index = -1;
        }

        size_t size() {
            return sizeof(A) + sizeof(Node<A,D>*)*3 + sizeof(D);
        }

    };

    template<typename A,typename D>
    class Tree {

        std::vector<const A*> dictionary;
        Node<A,D>* root = nullptr;

        void insert(const A*, const D);
        void create(D, D);

    public:

        D search(const A*);
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
    D Tree<A,D>::search(const A* word) {

        Node<A,D>* node = root;
        auto i = 0;

        while (true) {

            if (node->character == '\0') {
                // special case (compressed unitary path)
                if (strcmp(word, dictionary[node->index]) == 0) {
                    return node->index;
                } else {
                    return -1;
                }
            }

            if (word[i] < node->character) {
                if (node->left == nullptr) {
                    return -1;
                }
                node = node->left;
            }
            else if (word[i] == node->character) {
                i++;
                if (word[i] == 0) {
                    return node->index;
                }
                if (node->middle == nullptr) {
                    return -1;
                }
                node = node->middle;
            }
            else {
                if (node->right == nullptr) {
                    return -1;
                }
                node = node->right;
            }

        }

    }

}

#endif //AUTOCOMPLETE_TST_P_H
