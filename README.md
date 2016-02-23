# Autocomplete System

Autocomplete system based on a compressed ternary serch tree with range maximum query data structures.

- Search(P) in   O(P + log n)
- PrefixSearch(P) in   O(P + log n)
- Top-K Selection in O(k log k)

####Issue:

- Use of object and pointer insted of use of fixed size array to store the tree.
- Construction algorithm build the tree then compress it, insted of build a compressed tree.
- Compress function compress only unitary path to leaf and not inner unitary path.


####Possible Improvement:

- Use an ibrid tree, first level with M-child then standard TST.
