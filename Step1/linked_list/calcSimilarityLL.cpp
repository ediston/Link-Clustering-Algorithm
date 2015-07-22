// USAGE:
//      g++ -O3 -o calc calcSimilarityLL.cpp
//      ./calc network.pairs network.jaccs
//
//  -- network.pairs is an integer edgelist (one edge, two nodes
//  per line)
//  -- network.jaccs will contain the jaccard coefficient for each
//  pair of edges compared, of the form:
//      i_0 i_1 j_0 j_1 jaccard<newline>
//      ...
//  for edges (i_0,i_1) and (j_0,j_1)

// all this does is calculate the jaccard for "each" edge pair and
// write it to a file.  Two make this into real code will take some
// more work (the next step can be the hierarchical clustering over
// the output jacc file...)

// CORRECTNESS:
//  Returns same jaccard file as calcAndWrite_Jaccards.cpp, provided by the origina authors.
#include "linkedlist.h"
#include <ctime>
#include <algorithm>
map<long long, headNode*> nodeids;
map<long long, headNode*> :: iterator it;
map<pair <long, long>, double> storedSimilarityHashTable;


int main (int argc, char const *argv[]){
    double minJaccs = 223232, maxJaccs = -1;
    clock_t begin = clock();
    // make sure args are present:
    if (!argv[1]){
        cerr << "ERROR: no input file specified" << endl;
        cerr << "usage:\n    " << argv[0] << " input.pairs output.jaccs" << endl;
        exit(1);
    }
    if (!argv[2]){
        cerr << "ERROR: no output file specified" << endl;
        cerr << "usage:\n    " << argv[0] << " input.pairs output.jaccs" << endl;
        exit(1);
    }


    // load edgelist into linkedlists
    ifstream inFile;
    inFile.open (argv[1]);
    if (!inFile) {
        cerr << "ERROR: unable to open input file" << endl;
        exit(1); // terminate with error
    }
    long long ni,nj;
    headNode *head1, *head2;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        if(nodeids.count(ni)==0){
            head1 = getNewHeadNode();
            nodeids[ni] = head1;
            addNode(head1, ni); // we include this node itself
        }else head1 = nodeids[ni];
        if(nodeids.count(nj)==0){
            head2 = getNewHeadNode();
            nodeids[nj] = head2;
            addNode(head2, nj); // we include this node itself
        }else head2 = nodeids[nj];
        addNode(head1, nj);
        addNode(head2, ni);
    }
    inFile.close();

    // end load edgelist

    // iterate through the map of nodeIds

    // do the gosh darn calculation, fool!
    FILE * jaccFile = fopen(argv[2],"w");
    long long keystone, len_int;
    headNode *head, *head_i, *head_j;
    node *n_i, *n_j;
    head = getNewHeadNode(); n_i = getNewNode(); n_j = getNewNode();
    head_i = getNewHeadNode(); head_j = getNewHeadNode();
    double curr_jacc;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        keystone = (*it).first;
        head = (*it).second;
        n_i = head->next;
        while(n_i && n_i->next) { // neighbors of keystone in the linkedlist
            if (n_i->nodeId == keystone){
                //cout << "n_i->nodeId == keystone" << endl;
                n_i = n_i->next;
                continue;
            }
            head_i = nodeids[n_i->nodeId];
            n_j = n_i->next;
            while(n_j) { // next neighbors of n_k
                head_j = nodeids[n_j->nodeId];
                if (n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId){
                    //cout << "n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId" << endl;
                    n_j = n_j->next;
                    continue;
                }
                //cout << keystone << ": " << n_i->nodeId << " " << n_j->nodeId  << " | "
                //        <<head_i->size << " " << head_j->size << " || " ;
                if(storedSimilarityHashTable.count(make_pair(n_i->nodeId, n_j->nodeId))){
                    curr_jacc = storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)];
                }else{
                    len_int = getCommonNodesCount(head_i, head_j);    // my set intersection function
                    //cout << len_int << ", ";
                    curr_jacc = (double) len_int / (double)( head_i->size  + head_j->size - len_int);
                    storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)] = curr_jacc;
                }
                minJaccs = min(minJaccs, curr_jacc);
                maxJaccs = max(maxJaccs, curr_jacc);
                //cout << curr_jacc << endl;
                if (keystone < n_i->nodeId && keystone < n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i->nodeId, keystone, n_j->nodeId, curr_jacc );
                } else if (keystone < n_i->nodeId && keystone > n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i->nodeId, n_j->nodeId, keystone, curr_jacc );
                } else if (keystone > n_i->nodeId && keystone < n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i->nodeId, keystone, keystone, n_j->nodeId, curr_jacc );
                } else {
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i->nodeId, keystone, n_j->nodeId, keystone, curr_jacc );
                }
                n_j = n_j->next;
            }
            n_i = n_i->next;
        }
    } // done loop over keystones
    fclose(jaccFile);
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        head = (*it).second;
        deleteLL(head);
    }
    cout << "minJaccs = " << minJaccs << endl;
    cout << "maxJaccs = " << maxJaccs << endl;
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC  << " seconds. "<< endl;
    return 0;
}
