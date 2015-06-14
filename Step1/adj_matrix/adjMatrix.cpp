#include <cmath>
#include <ctime>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <cstdint>
#include <iostream>
#include <fstream>

using namespace std;
long long totalComparisons;
long long N;
double total;
bool** adjMatrix;
void intMatrix(){
    adjMatrix = new bool *[N];
    for(long i=0; i<N; i++){
        adjMatrix[i] = new bool[N];
        for(long j=0; j<N; j++){
            adjMatrix[i][j] = 0;
        }
        adjMatrix[i][i] = 1;
    }
}

long long getCommonNodesCount(long long ni, long long nj){
    long long count = 0;
    total = 0;
    for(long long i=0; i<N; i++){
        totalComparisons++;
        count  += (long long)(adjMatrix[ni][i] && adjMatrix[nj][i]);
        if(adjMatrix[ni][i]) total++;
        if(adjMatrix[nj][i]) total++;
    }
    return count;
}

void deleteLL(){
    for(long long i=0; i<N; i++)
        free(adjMatrix[i]);
}
map <pair<long long, long>, double> storedSimilarityHashTable;

int main (int argc, char const *argv[]){
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
    // first we need the biggest node
    long long ni,nj;
    N = 0;
    ifstream inFile;
    inFile.open (argv[1]);
    if (!inFile) {
        cerr << "ERROR: unable to open input file" << endl;
        exit(1); // terminate with error
    }
    while (inFile >> ni >> nj){
        N = max(N, max(ni, nj));
    }
    inFile.close();
    N++;
    intMatrix();
    // load edgelist into linkedlists
    
    inFile.open (argv[1]);
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for
        adjMatrix[ni][nj] = 1;
        adjMatrix[nj][ni] = 1;
    }
    inFile.close();
    
    // end load edgelist
    //----------------------
    FILE * jaccFile = fopen(argv[2],"w");
    long long keystone, len_int;
    long long n_i, n_j;
    // iterate through the map of nodeIds
    totalComparisons = 0;
    double curr_jacc;
    for (keystone=0; keystone<N; keystone++) { // loop over keys
        //printList(head);
        for(n_i=0; n_i<N; n_i++) { // neighbors of keystone in the linkedlist
            if (n_i== keystone || adjMatrix[n_i][keystone]==0){
                continue;
            }
            //cout << n_i << " = n_i" << endl;
            n_j = n_i + 1;
            for(; n_j<N; n_j++) { // next neighbors of n_k
                if (n_j == keystone || adjMatrix[n_j][keystone]==0){
                    continue;
                }
                //cout << "\t" << n_j << " = n_j" << endl;
                if(storedSimilarityHashTable.count(make_pair(n_i, n_j))){
                    curr_jacc = storedSimilarityHashTable[make_pair(n_i, n_j)];
                }else{
                    len_int = getCommonNodesCount(n_i, n_j);
                    curr_jacc = (double) len_int / (total  - (double)len_int );
                    storedSimilarityHashTable[make_pair(n_i, n_j)] = curr_jacc;
                }
                if (keystone < n_i && keystone < n_j){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i, keystone, n_j, curr_jacc );
                } else if (keystone < n_i && keystone > n_j){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i, n_j, keystone, curr_jacc );
                } else if (keystone > n_i && keystone < n_j){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i, keystone, keystone, n_j, curr_jacc );
                } else {
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i, keystone, n_j, keystone, curr_jacc );
                }
            }
        }
    } // done loop over keystones
    fclose(jaccFile);
    deleteLL();
    cout << "Total comparisons are: " << totalComparisons << endl;
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}
