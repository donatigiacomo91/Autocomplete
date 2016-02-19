//
// Created by Giacomo Donati on 17/02/16.
//

#ifndef AUTOCOMPLETE_TOP_K_H
#define AUTOCOMPLETE_TOP_K_H

#include <vector>
#include <queue>
#include <algorithm>

#include "rmmq.hpp"

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

        bool operator<(const Element& e1) const;
        Element ( const Element & ) = default;
        Element& operator=( const Element & ) = default;
        Element ( Element && ) = default;

    };

    bool Element::operator<(const Element& e1) const {
        return val < e1.val;
    }

    template <typename S, typename I>
    class K_Heap {

        std::vector<S> scores;
        RMMQ<S> rmmq;

    public:

        // return the indices of the best k scores in the range [i,j]
        std::vector<I> get(unsigned int k, I i, I j);

        void make (const std::vector<S>& vec) {
            scores = vec;
            rmmq.make(scores);
        }

        K_Heap() {}

        K_Heap (const std::vector<S>& vec) {
            scores = vec;
            rmmq.make(scores);
        }

        ~K_Heap () {
            rmmq.destroy();
        }

    };

    template <typename S, typename I>
    std::vector<I> K_Heap::get(unsigned int k, I i, I j) {

        // max heap that allow top-k retrieval
        std::priority_queue<Element> max_heap;
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
        Element e = Element(scores[index],index,i,j);
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
                max_heap.push(Element(scores[lc],lc,e.left_index,e.index-1));
            if (rc != -1)
                max_heap.push(Element(scores[rc],rc,e.index+1,e.right_index));
        }

        return result;
    }

}

#endif //AUTOCOMPLETE_TOP_K_H
