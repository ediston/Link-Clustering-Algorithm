/*
Vivek Dhiman
TU Kaiserslautern
vivek4dhiman@gmail.com
*/
#include <ctime>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm> // for swap
using namespace std;
bool **adjMatrix;
long long totalComparisons;
set<int> *neighbors ;

void initAdjMatrix(long long N){
    adjMatrix = new bool *[N];
    for(long i=0; i<N; i++){
        adjMatrix[i] = new bool[N];
        for(long j=0; j<N; j++){
            adjMatrix[i][j] = 0;
        }   
    }  
}

int intersection_size(  long long a, long long b) {
    int num = 0;
    // take the smaller size as a
    if(neighbors[a].size() > neighbors[b].size()){ 
        swap(a,b); 
    }
    set<int>::const_iterator As = neighbors[a].begin(), Af = neighbors[a].end();
                             //Bs = B.begin(), Bf = B.end();
    // iterate through the neighbors of A
    // check in O(1) if B also has the same neighbor
    while ( As != Af ) {
        if (adjMatrix[b][*As]) {
            ++num;
        }
        ++As;
        totalComparisons++;
    }
    return num;
}

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


    // load edgelist into two arrays, and into an array of sets:
    ifstream inFile;
    inFile.open (argv[1]);
    if (!inFile) {
        cerr << "ERROR: unable to open input file" << endl;
        exit(1); // terminate with error
    }
    int ni,nj, max_node = -1;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        if (ni > max_node){  max_node = ni;  }
        if (nj > max_node){  max_node = nj;  }
    }
    inFile.close();
    
    int num_nodes = max_node + 1; // assumes nodes are contiguous ints starting at zero
    initAdjMatrix(num_nodes);
    neighbors = new set<int>[num_nodes]; // this stores the network

    inFile.open( argv[1] );
    int index = 0;
    while (inFile >> ni >> nj){ // rescan edgelist to populate 
        neighbors[ni].insert(nj);
        neighbors[nj].insert(ni); // undirected
        adjMatrix[nj][ni] = 1;
        adjMatrix[ni][nj] = 1;
    }
    for (int i=0; i < num_nodes; i++){
        neighbors[i].insert(i); // neighbors[] is now INCLUSIVE!
        adjMatrix[i][i] = 1;
    }
    inFile.close();
    // end load edgelist
    totalComparisons = 0;
    // do the gosh darn calculation, fool!
    FILE * jaccFile = fopen(argv[2],"w");
    int n_i, n_j, keystone, len_int;
    double curr_jacc;
    set<int>::iterator i, j;
    /// ----------------------------------
    for (int keystone=0; keystone < num_nodes; keystone++) { // loop over keystones 
        
        for ( i = neighbors[keystone].begin(); i != neighbors[keystone].end(); i++) { // neighbors of keystone
            n_i = *i;
            if (n_i == keystone)
                continue;
            
            for ( j = neighbors[keystone].begin(); j != neighbors[keystone].end(); j++ ) { // neighbors of keystone
                n_j = *j;
                if (n_j == keystone or n_i >= n_j)
                    continue;
                // my set intersection function
                len_int = intersection_size(n_i, n_j);    

                curr_jacc = (double) len_int / (double)( neighbors[n_i].size() + neighbors[n_j].size() - len_int );
                if (keystone < n_i && keystone < n_j){
                    fprintf( jaccFile, "%i\t%i\t%i\t%i\t%f\n", keystone, n_i, keystone, n_j, curr_jacc );
                } else if (keystone < n_i && keystone > n_j){
                    fprintf( jaccFile, "%i\t%i\t%i\t%i\t%f\n", keystone, n_i, n_j, keystone, curr_jacc );
                } else if (keystone > n_i && keystone < n_j){
                    fprintf( jaccFile, "%i\t%i\t%i\t%i\t%f\n", n_i, keystone, keystone, n_j, curr_jacc );
                } else {
                    fprintf( jaccFile, "%i\t%i\t%i\t%i\t%f\n", n_i, keystone, n_j, keystone, curr_jacc );
                }
            }
        }
    } // done loop over keystones
    fclose(jaccFile);
    cout << "Total Comparisons = " << totalComparisons << endl;
    delete [] neighbors; // all done, clean up memory...
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}

