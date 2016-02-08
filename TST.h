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
        D index = 0;

        Node(const A* c) {
            character = *c;
        }
    };

    template<typename A, typename P, typename D>
    class Tree {
        std::vector<Node<A,P,D>*> vec;

        P insert(P, const A*, const D);
        void create(const std::vector<const A*>&, P, P);

    public:
        Tree(const std::vector<const A*>& dic){

            if (dic.size() <= 1) {
                return ;
            }
            vec.push_back(nullptr);

            D root_index = dic.size()/2;
            insert(0, dic[root_index], root_index);

            create(dic, 1, root_index);
            create(dic, root_index+1, dic.size());

        }
    };

    template<typename A, typename P, typename D>
    P Tree<A,P,D>::insert(P node_index, const A* word, const D dic_index) {

        Node<A,P,D>* node;

        if (node_index == 0) {
            node = new Node<A,P,D>(word);
            node->left = 0;
            node->middle = 0;
            node->right = 0;

            vec.push_back(node);
            node_index = vec.size()-1;
        } else {
            node = vec[node_index];
        }

        if (*word < node->character) {
            node->left = insert(node->left, word, dic_index);
        }
        else if (*word == node->character) {

            if (*word != 0) {
                node->middle = insert(node->middle, ++word, dic_index);
            } else {
                node->index = dic_index;
            }

        } else {
            node->right = insert(node->right, word, dic_index);
        }

        return node_index;

    };

    template<typename A, typename P, typename D>
    void Tree<A,P,D>::create(const std::vector<const A*>& dic, P min, P max) {

        if (min < max) {
            P mid = (min+max)/2;
            insert(1, dic[mid], mid);
            create(dic, min, mid);
            create(dic, mid+1, max);
        }
    }

}

#endif //AUTOCOMPLETE_TST_H
