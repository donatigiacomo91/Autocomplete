//
// Created by Giacomo Donati on 04/02/16.
//

#ifndef AUTOCOMPLETE_TST_H
#define AUTOCOMPLETE_TST_H

#include <vector>

namespace tst {

    template<typename A, typename P, typename D>
    class Node {
    public:
        A character;
        P left, middle, right;
        D index;

        Node(const A c) {
            character = c;
            left = middle = right = 0;
            index = 0;
        }

        size_t size() {
            return sizeof(A) + sizeof(P)*3 + sizeof(D);
        }

    };

    template<typename A, typename P, typename D>
    class Tree {

        std::vector<Node<A,P,D>*> vec;

        void insert(const A*, const D);
        void create(std::vector<A*>&, P, P);

    public:

        D search(const A*);
        long node_count(P);
        size_t size();
        int compress(P);
        long compressed_count();

        Tree(std::vector<A*>& dic){
            if (dic.size() < 2) {
                return ;
            }
            // root must stay at index 1, index 0 means no child
            vec.push_back(nullptr);
            // dic[0] is ignored, index 0 means no string
            create(dic, 1, dic.size());
        }

    };

    // count the number of compressed node
    template<typename A, typename P, typename D>
    long Tree<A,P,D>::compressed_count() {
        auto count = 0;
        for (auto i=1; i != vec.size(); i++) {
            if(vec[i]->middle == -1)
                count++;
        }
        return count;
    }

    // compress unitary path to leaf in one node
    // mark deleted node with node.middle = -1
    template<typename A, typename P, typename D>
    int Tree<A,P,D>::compress(P index) {

        if (index == 0) {
            return 0;
        }

        Node<A,P,D>* node = vec[index];
        if (node->left != 0 || node->right != 0) {
            compress(node->left);
            compress(node->middle);
            compress(node->right);
            return 0;
        }

        // leaf node
        if (node->middle == 0) {
            return 1;
        }

        // internal node with only middle child
        if (int count = compress(node->middle)) {
            Node<A,P,D>* child = vec[node->middle];
            // mark node as deleted
            child->middle = -1;
            node->index = child->index;
            node->character = '\0';
            count++;
            node->middle = count;
            return count;
        }

        return 0;

    }

    // return the number of node of the subtree rooted in vec[index]
    template<typename A, typename P, typename D>
    long Tree<A,P,D>::node_count(P index) {

        if (index == 0) {
            return 0;
        }

        Node<A,P,D>* node = vec[index];
        if (node->character == '\0') {
            return 1;
        }
        if (node->left == 0 && node->middle == 0 && node->right == 0 ) {
            return 1;
        }

        return 1 + node_count(node->left) + node_count(node->middle) + node_count(node->right);
    }

    // return the size in byte of the whole tree
    template<typename A, typename P, typename D>
    size_t Tree<A,P,D>::size() {

        size_t total_size = 0;
        total_size += vec.size() * sizeof(Node<A,P,D>);
        total_size += vec.size() * sizeof(Node<A,P,D>*);
        total_size += sizeof(vec);

        return total_size;
    }

    // insert a word in the tree
    template<typename A, typename P, typename D>
    void Tree<A,P,D>::insert(const A* word, const D dic_index) {

        Node<A,P,D>* node;
        auto i = 0;

        if (vec.size() == 1) {
            vec.push_back(new Node<A,P,D>(word[i]));
        }
        node = vec[1];

        while (word[i] != 0) {

            if (word[i] < node->character) {
                if (node->left == 0) {
                    vec.push_back(new Node<A,P,D>(word[i]));
                    node->left = (P) vec.size()-1;
                }
                node = vec[node->left];

            }
            else if (word[i] == node->character) {
                i++;
                if (word[i] == 0) {
                    node->index = dic_index;
                    return;
                }
                if (node->middle == 0) {
                    vec.push_back(new Node<A,P,D>(word[i]));
                    node->middle = (P) vec.size()-1;
                }
                node = vec[node->middle];
            }
            else {
                if (node->right == 0) {
                    vec.push_back(new Node<A,P,D>(word[i]));
                    node->right = (P) vec.size()-1;
                }
                node = vec[node->right];
            }


        }
        node->index = dic_index;

    };

    // create a balanced tree starting from a sorted dictionary
    // recursively insert the middle string in the sub-array [min,max]
    template<typename A, typename P, typename D>
    void Tree<A,P,D>::create(std::vector<A*>& dic, P min, P max) {
        if (min < max) {
            P mid = (min+max)/2;
            insert(dic[mid], mid);
            create(dic, min, mid);
            create(dic, mid+1, max);
        }
    }

    // TODO: to be fixed to work with compact tree
    // return the index of the word in the dictionary
    // 0 if the string do not appears
    template<typename A, typename P, typename D>
    D Tree<A,P,D>::search(const A* word) {

        Node<A,P,D>* node = vec[1];
        auto i = 0;

        while (true) {

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

}

#endif //AUTOCOMPLETE_TST_H
