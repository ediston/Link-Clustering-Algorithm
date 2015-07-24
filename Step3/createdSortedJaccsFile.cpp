/*
//  $ g++ -O3 -o createdSortedJaccsFile createdSortedJaccsFile.cpp
//  $ ./createdSortedJaccsFile network.jaccs newSortedNewtwork.jaccs sortedJaccs.csv
*/
#include <math.h> 
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <utility>   // for pairs
#include <algorithm> // for swap
#define MIN_DIFF_BW_JACCS 0.0001
using namespace std;

int main (int argc, char const *argv[]){
    //************* make sure args are present:
    if (argc != 4){
        cout << "ERROR: something wrong with the inputs" << endl;
        cout << "usage:\n    " << argv[0] << " network.jaccs newSortedNewtwork.jaccs sortedJaccs.csv" << endl;
        exit(1);
    }
    //************* got the args
    clock_t begin = clock(); 
    ifstream jaccFile;  
    jaccFile.open( argv[1] );
    if (!jaccFile) {
        cout << "ERROR: unable to open jaccards file" << endl;
        exit(1); // terminate with error
    }
    set<pair<float, pair<int, int> > > jaccsEdgeEdgeSet;
    int edgeId1,edgeId2; 
    float jacc;
    set<float> thresholdSet;
    // read each line from the jacc file
    while ( jaccFile >> edgeId1 >> edgeId2 >> jacc ) {
        // if this value of jacc does exists
        // then add inser the value
        jaccsEdgeEdgeSet.insert(make_pair(jacc, make_pair(edgeId1, edgeId2)));
        if(thresholdSet.count(jacc) == 0)
          thresholdSet.insert(jacc);
    }
    jaccFile.close();jaccFile.clear();
    cout << "Done with thresholdSet creation. total unique jaccs:" << thresholdSet.size() << endl;
    // create the file
    // if exists then overwrite everything
    FILE * sortedJaccsFile = fopen( argv[3], "w" ); 
    set<float> ::reverse_iterator thIt;
    for(thIt = thresholdSet.rbegin(); thIt!=thresholdSet.rend(); thIt++){
      jacc = *thIt;
      // open the file 
      fprintf( sortedJaccsFile, "%.6f \n", jacc);
    }
    fclose(sortedJaccsFile);
    thresholdSet.clear();
    cout << "Done writing unique jaccs! ";
    clock_t end1 = clock(); 
    cout << "Time taken = " << double(end1 - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    //------------------------
    // Create a file with sorted in on-increasing order jaccs  with:
    // < edgeId edgeId jaccs > on each line
    long long done = 0;
    float percDone = 0.01;
    FILE * sortedNewNWJaccsFile = fopen( argv[2], "w" );
    set<pair<float, pair<int, int> > >::reverse_iterator  jaccsEdgeEdgeSetIt;
    for(jaccsEdgeEdgeSetIt = jaccsEdgeEdgeSet.rbegin(); 
            thIt!=jaccsEdgeEdgeSet.rend(); jaccsEdgeEdgeSetIt++){
      jacc = (*jaccsEdgeEdgeSetIt)->first;
      edgeId1 = (*jaccsEdgeEdgeSetIt)->second->first; 
      edgeId2 = (*jaccsEdgeEdgeSetIt)->second->second; 
      // open the file 
      fprintf( sortedNewNWJaccsFile, "%lld %lld %.6f\n", edgeId1, edgeId2, jacc);
      done++;
      if((float)done/(float)jaccsEdgeEdgeSet.size() >= percDone){
        cout << percDone*100 << " pc done.\n" << endl;
        percDone += 0.01;
      }
    }
    fclose(sortedNewNWJaccsFile);
    jaccsEdgeEdgeSet.clear();
    cout << "Done writing sorted New NW JaccsFile! ";
    cout << "Time taken = " << double(clock() - end1)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    
    cout << "Total Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}
