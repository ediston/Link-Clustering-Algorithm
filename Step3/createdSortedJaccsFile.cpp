/*
//  $ g++ -O3 -o createdSortedJaccsFile createdSortedJaccsFile.cpp
//  $ ./createdSortedJaccsFile network.jaccs sortedjaccs.csv
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
    if (argc != 3){
        cout << "ERROR: something wrong with the inputs" << endl;
        cout << "usage:\n    " << argv[0] << " network.jaccs sortedjaccs.csv" << endl;
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
    int edgeId1,edgeId2; 
    float jacc;
    set<float> thresholdSet;
    set<float> ::reverse_iterator thIt;
    // read each line from the jacc file
    while ( jaccFile >> edgeId1 >> edgeId2 >> jacc ) {
        // if this value of jacc does exists
        // then add inser the value
        if(thresholdSet.count(jacc) == 0)
          thresholdSet.insert(jacc);
    }
    jaccFile.close();jaccFile.clear();
    cout << "Done with thresholdSet creation. total unique jaccs:" << thresholdSet.size() << endl;
    // create the file
    // if exists then overwrite everything
    FILE * sortedJaccsFile = fopen( argv[2], "w" );
    fclose(sortedJaccsFile);
    for(thIt = thresholdSet.rbegin(); thIt!=thresholdSet.rend(); thIt++){
      jacc = *thIt;
      // open the file
      sortedJaccsFile = fopen( argv[2], "a" );
      fprintf( sortedJaccsFile, "%.6f \n", jacc);
      fclose(sortedJaccsFile);
    }
    thresholdSet.clear();
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}
