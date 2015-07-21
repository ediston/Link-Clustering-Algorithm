/*
By Vivek Dhiman 
*/

#include <cstdlib>
#include <string>
#include <sstream>
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
        cout << "usage:\n    " << argv[0] << " networkEdgeIdMap.csv network.clusters newnetwork.clusters" << endl;
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
    map<long long, pair<long long, long long> > edgeIdMap; // O(log n) access too slow? 
    long long ni, nj;
    while (inFile >> ni >> nj >> edgeId){ // scan edgelist to populate  
        edgeIdMap[edgeId] = make_pair(ni,nj);
    } 
    inFile.close(); inFile.clear();
    //************* end load edgelist
    
    //************* loop over jaccards file and do the clustering
    
    ifstream jaccFile;  
    jaccFile.open( argv[2] );
    if (!jaccFile) {
        cout << "ERROR: unable to open jaccards file" << endl;
        exit(1); // terminate with error
    }   
    FILE * nwClusterFile  = fopen( argv[3], "w" );
    string line;
    while (std::getline(jaccFile, line)){
      std::stringstream ss(line);
      while(ss >> edgeId){
        fprintf( nwClusterFile, "%ld,%ld ", edgeIdMap[edgeId].first, edgeIdMap[edgeId].second);
      }
      fprintf( nwClusterFile, "\n");
    } 
    fclose(nwClusterFile);
    //************* done looping over jaccards file
    jaccFile.close(); jaccFile.clear();
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC  << " seconds. "<< endl;
    return 0;
}
