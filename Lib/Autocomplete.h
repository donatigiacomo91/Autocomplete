//
// Created by Giacomo Donati on 19/02/16.
//

#ifndef AUTOCOMPLETE_AUTOCOMPLETE_H
#define AUTOCOMPLETE_AUTOCOMPLETE_H

#import "TST_P.h"
#import "Top_K.h"

namespace atl {

    // A is the type used for the alphabet of the dictionary
    // S is the type used for the scores
    // I is the type used for index scores and dictionary
    template <typename A, typename S, typename I>
    class Autocomplete {

        tst_p::Tree<A,I> tst;
        top_k::Selector<S,I> k_max;

    public:

        Autocomplete(const std::vector<const A*>& dic, const std::vector<S>& sc) {
            // build and compress the ternary search tree
            tst.make(dic);
            tst.compress(tst.getRoot());
            // initialize the top-k selector data structures
            k_max.make(sc);
        }

        // return the indices of the top-k scores of the words
        // in the dictionary that are prefixed by pref
        std::vector<I> topK(const A* prefix, unsigned int k) {

            std::array<I,2> range = tst.prefix(prefix);
            return k_max.get(k, range[0], range[1]);
        }

    };

}

#endif //AUTOCOMPLETE_AUTOCOMPLETE_H
