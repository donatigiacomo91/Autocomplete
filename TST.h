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
        int compress(P);

    public:

        D search(const A*);
        long node_count(P);
        size_t size();

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

    // compress unitary path to leaf in one node
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

    template<typename A, typename P, typename D>
    size_t Tree<A,P,D>::size() {
        size_t total_size = 0;

        total_size += vec.size() * sizeof(Node<A,P,D>);
        total_size += vec.size() * sizeof(Node<A,P,D>*);
        total_size += sizeof(vec);

        return total_size;
    }

    template<typename A, typename P, typename D>
    void Tree<A,P,D>::insert(const A* word, const D dic_index) {

        Node<A,P,D>* node;
        uint32_t i = 0;

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
                if (node->middle == 0) {
                    if (word[i] == 0) {
                        node->index = dic_index;
                        return;
                    }
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
    template<typename A, typename P, typename D>
    D Tree<A,P,D>::search(const A* word) {

        Node<A,P,D>* node = vec[1];
        uint32_t i = 0;

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
