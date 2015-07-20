/*
By Vivek Dhiman
1. Create two files
    1.1. For storing the new jaccs with id id jaccs, call edgeIdsJacc.jaccs
    1.2. For storing the mapping, edgeIDMapFile.csv
2. Open the jaccs file
3. Read in the next line
4. If both edges exist in the map, just write id id jaccs to edgeIdsJacc.jaccs
5. Else whichever edge doesn't exist in the map, add it to map and increment the edge id by1
*/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <utility>   // for pairs
#include <algorithm> // for swap
using namespace std;

int main (int argc, char const *argv[]){
    //************* make sure args are present:
    if (argc != 5){
        cout << "ERROR: something wrong with the inputs" << endl;
        cout << "usage:\n    " << argv[0] << " network.pairs network.jaccs networkEdgeIdMap.csv newnetwork.jaccs" << endl;
        exit(1);
    } 
    //************* got the args
    clock_t begin = clock();
    
    //************* start load edgelist
    ifstream inFile;
    inFile.open( argv[1] );
    if (!inFile) {
        cout << "ERROR: unable to open input file" << endl;
        exit(1); // terminate with error
    }
    // index should be iterator not integer????
    long long edgeId = 0;
    map< pair<long long, long long>, long long> edgeIdMap; // O(log n) access too slow? 
    FILE * networkEdgeIdMapFile  = fopen( argv[3], "w" );

    long long ni, nj, wij;
    while (inFile >> ni >> nj){ // scan edgelist to populate 
    //while (inFile >> ni >> nj >> wij){ // scan edgelist to populate WEIGHTED
        if (ni >= nj) swap(ni,nj); // undirected!
        edgeIdMap[make_pair(ni,nj)] = edgeId;
        fprintf( networkEdgeIdMapFile, "%ld %ld %ld\n", ni, nj,  edgeId);
        edgeId++;
    }
    fclose(networkEdgeIdMapFile);
    inFile.close(); inFile.clear();
    //************* end load edgelist
    
    //************* loop over jaccards file and do the clustering
    ifstream jaccFile;  
    jaccFile.open( argv[2] );
    if (!jaccFile) {
        cout << "ERROR: unable to open jaccards file" << endl;
        exit(1); // terminate with error
    }
    FILE * newnetworkFile = fopen( argv[4], "w" );
    long long i0,i1,j0,j1; double jacc;
    long long edgeId1, edgeId2; 
    while ( jaccFile >> i0 >> i1 >> j0 >> j1 >> jacc ) {
        if (i0 >= i1) swap(i0,i1);  
        if (j0 >= j1) swap(j0,j1);  
         
        if(edgeIdMap.count(make_pair(i0,i1) ) == 0){
            cout << "Not found: " << i0 << "," << i1 << endl;
            continue;
        } 
        edgeId1 = edgeIdMap[ make_pair(i0,i1) ];
        edgeId2 = edgeIdMap[ make_pair(j0,j1) ];
        fprintf( newnetworkFile, "%ld %ld %f\n", edgeId1, edgeId2, jacc);
    }
    fclose(newnetworkFile);
    //************* done looping over jaccards file
    
    
    //************* write the clusters to file:
    jaccFile.close();
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC  << " seconds. "<< endl;
    return 0;
}
