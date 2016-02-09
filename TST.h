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
    };

    template<typename A, typename P, typename D>
    class Tree {
        std::vector<Node<A,P,D>*> vec;

        void insert(const A*, const D);
        void create(const std::vector<const A*>&, P, P);

    public:
        Tree(const std::vector<const A*>& dic){

            if (dic.size() < 2) {
                return ;
            }

            // dic[0] is ignored, index 0 means no string
            create(dic, 1, dic.size());

        }
    };

    template<typename A, typename P, typename D>
    void Tree<A,P,D>::insert(const A* word, const D dic_index) {

        Node<A,P,D>* node;
        Node<A,P,D>* next_node;
        uint16_t i = 0;

        if (vec.size() == 0) {
            vec.push_back(new Node<A,P,D>(word[i]));
        }

        node = vec[0];

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
    void Tree<A,P,D>::create(const std::vector<const A*>& dic, P min, P max) {

        if (min < max) {
            P mid = (min+max)/2;
            insert(dic[mid], mid);
            create(dic, min, mid);
            create(dic, mid+1, max);
        }
    }

}

#endif //AUTOCOMPLETE_TST_H
