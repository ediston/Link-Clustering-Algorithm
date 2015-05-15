// USAGE:
//      g++ -O3 -o calc calcSimilaritySL.cpp
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
#include<linkedlist.h>
map<long long, skipList*> nodeids;
map<long long, skipList*> :: iterator it;
map<pair <long, long>, double> storedSimilarityHashTable;


int main (int argc, char const *argv[]){
     srand (time(NULL));
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
    skipList *s1, *s2;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        cout << ni << " " << nj << endl;
        if(nodeids.count(ni)==0){
            s1 = getNewSkipList();
            nodeids[ni] = s1;
            addNode(s1, ni); // we include this node itself
        }else s1 = nodeids[ni];
        if(nodeids.count(nj)==0){
            s2 = getNewSkipList();
            nodeids[nj] = s2;
            addNode(s2, nj); // we include this node itself
        }else s2 = nodeids[nj];
        addNode(s1, nj);
        addNode(s2, ni);
    }
    inFile.close();
    cout << "load edgelist done\n";
    // end load edgelist

    // iterate through the map of nodeIds

    // do the gosh darn calculation, fool!
    FILE * jaccFile = fopen(argv[2],"w");
    long long keystone, len_int;
    skipList *s, *s_i, *s_j;
    node *n_i, *n_j;
    s = getNewSkipList(); n_i = getNewNode(0); n_j = getNewNode(0);
    s_i = getNewSkipList(); s_j = getNewSkipList();
    double curr_jacc;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        keystone = (*it).first;
        s = (*it).second;
        n_i = s->next[0];
        while(n_i && n_i->next[0]) { // neighbors of keystone in the linkedlist
            if (n_i->nodeId == keystone){
                cout << "n_i->nodeId == keystone" << endl;
                n_i = n_i->next[0];
                continue;
            }
            s_i = nodeids[n_i->nodeId];
            n_j = n_i->next[0];
            while(n_j) { // next neighbors of n_k
                if (n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId){
                    cout << "n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId" << endl;
                    n_j = n_j->next[0];
                    continue;
                }
                s_j = nodeids[n_j->nodeId];
                //cout << keystone << ": " << n_i->nodeId << " " << n_j->nodeId  << " | "
                //        <<head_i->size << " " << head_j->size << " || " ;
                if(storedSimilarityHashTable.count(make_pair(n_i->nodeId, n_j->nodeId))){
                    curr_jacc = storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)];
                }else{
                    len_int = getCommonNodesCount(s_i, s_j);    // my set intersection function
                    //cout << len_int << ", ";
                    curr_jacc = (double) len_int / (double)( s_i->size  + s_j->size - len_int);
                    storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)] = curr_jacc;
                }
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
                n_j = n_j->next[0];
            }
            n_i = n_i->next[0];
        }
    } // done loop over keystones
    fclose(jaccFile);
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        s = (*it).second;
        deleteLL(s);
    }
    return 0;
}
