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
    class Element {
    public:
        int val;
        int left_index, index, right_index;

        Element(int v, int i, int li, int ri) {
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

    class K_Heap {

        std::vector<int> scores;
        RMMQ<int> rmmq;

    public:

        // return the indices of the best k scores in the range [i,j]
        std::vector<int> get(int k, int i, int j);

        K_Heap (const std::vector<int>& vec) {
            scores = vec;
            rmmq.make(scores);
        }

        ~K_Heap () {
            rmmq.destroy();
        }

    };


    std::vector<int> K_Heap::get(int k, int i, int j) {

        // max heap that allow top-k retrival
        std::priority_queue<Element> max_heap;
        // top-k result in [i,j]
        std::vector<int> result;

        if (j-i <= k) {
            for ( ; i<=j; i++) {
                result.push_back(i);
            }
            return result;
        }

        auto index = rmmq.MaxPos(i,j);
        Element e = Element(scores[index],index,i,j);
        max_heap.push(e);

        while (result.size() < k) {

            e = max_heap.top();
            max_heap.pop();
            result.push_back(e.index);

            auto lc = (e.left_index==e.index) ? -1 : rmmq.MaxPos(e.left_index,e.index-1);
            auto rc = (e.right_index==e.index) ? -1 : rmmq.MaxPos(e.index+1,e.right_index);

            if (lc != -1)
                max_heap.push(Element(scores[lc],lc,e.left_index,e.index-1));
            if (rc != -1)
                max_heap.push(Element(scores[rc],rc,e.index+1,e.right_index));

        }

        return result;

    }

}

#endif //AUTOCOMPLETE_TOP_K_H
