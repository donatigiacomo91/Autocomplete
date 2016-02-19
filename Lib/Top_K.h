//
// Created by Giacomo Donati on 17/02/16.
//

#ifndef AUTOCOMPLETE_TOP_K_H
#define AUTOCOMPLETE_TOP_K_H

#include <vector>
#include <queue>
#include <algorithm>

#include "rmmq.hpp"

// TOP-K SELECTOR WITH RMQ DATA STRUCTURES
//
// top-k selection in O(k log k) using priority queue and range maximum query
// RMQ is build in O(n log n) and return the max in constant time

namespace top_k {

    // element to be stored in the max heap :
    // - val is the score of the element
    // - left_index and right_index delimit the range
    //   for the next calls of rmq when element is extracted from heap
    template <typename S, typename I>
    class Element {
    public:
        S val;
        I left_index, index, right_index;

        Element(S v, I i, I li, I ri) {
            val = v;
            index = i;
            left_index = li;
            right_index = ri;
        }

        bool operator<(const Element<S,I>& e1) const;
    };

    template <typename S, typename I>
    bool Element<S,I>::operator<(const Element<S,I>& e1) const {
        return val < e1.val;
    }

    template <typename S, typename I>
    class Selector {

        std::vector<S> scores;
        RMMQ<S> rmmq;

    public:

        // initialize RMMQ with the passed vector
        void make (const std::vector<S>& vec);
        // return the indices of the best k scores in the range [i,j]
        std::vector<I> get(unsigned int k, I i, I j);

        Selector() {}

        Selector (const std::vector<S>& vec) {
            scores = vec;
            rmmq.make(scores);
        }

        ~Selector () {
            rmmq.destroy();
        }

    };

    template <typename S, typename I>
    void Selector<S,I>::make (const std::vector<S>& vec) {
        scores = vec;
        rmmq.make(scores);
    }

    template <typename S, typename I>
    std::vector<I> Selector<S,I>::get(unsigned int k, I i, I j) {

        // max heap that allow top-k retrieval
        std::priority_queue<Element<S,I>> max_heap;
        // top-k result in [i,j]
        std::vector<I> result;

        // we have less than K elements
        if (j-i <= k) {
            for ( ; i<=j; i++) {
                result.push_back(i);
            }
            return result;
        }

        // push the max in the range
        auto index = rmmq.MaxPos(i,j);
        Element<S,I> e = Element<S,I>(scores[index],index,i,j);
        max_heap.push(e);

        while (result.size() < k) {
            // get the max in the range
            e = max_heap.top();
            max_heap.pop();
            result.push_back(e.index);

            // get left max and right max
            auto lc = (e.left_index==e.index) ? -1 : rmmq.MaxPos(e.left_index,e.index-1);
            auto rc = (e.right_index==e.index) ? -1 : rmmq.MaxPos(e.index+1,e.right_index);

            // push the children in the heap
            if (lc != -1)
                max_heap.push(Element<S,I>(scores[lc],lc,e.left_index,e.index-1));
            if (rc != -1)
                max_heap.push(Element<S,I>(scores[rc],rc,e.index+1,e.right_index));
        }

        return result;
    }

}

#endif //AUTOCOMPLETE_TOP_K_H
