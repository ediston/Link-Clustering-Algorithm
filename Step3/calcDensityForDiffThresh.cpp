/*
//  $ g++ -O3 -o calcDensityForDiffThresh calcDensityForDiffThresh.cpp
//  $ ./calcDensityForDiffThresh network.pairs network.jaccs threshDensity.csv
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
using namespace std;

int main (int argc, char const *argv[]){
    //************* make sure args are present:
    if (argc != 4){
        cout << "ERROR: something wrong with the inputs" << endl;
        cout << "usage:\n    " << argv[0] << " network.pairs network.jaccs threshDensity.csv" << endl;
        exit(1);
    }
    float threshold = 0;
    float D = 0.0;
    float highestD=0.0;
    float highestDThr = 0.0;
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
    set<float> thresholdSet;
    set<float> ::reverse_iterator thIt;
    map< int,          set<int> > index2cluster; // O(log n) access too slow?
    map< int, map<int, set<int> >::iterator > edge2iter;
    int ni, nj, edgeId, index = 0;
    while (inFile >> ni >> nj >> edgeId){ // scan edgelist to populate  
        index2cluster[ index ].insert( edgeId );         // build cluster index to set of edge-pairs map
        edge2iter[ edgeId ] = index2cluster.find(index); // build edge pair to cluster iter map ******????
        index++;
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
    int edgeId1,edgeId2; double jacc; 
    while ( jaccFile >> edgeId1 >> edgeId2 >> jacc ) {
        thresholdSet.insert(jacc);
    }
    jaccFile.close();
    map< int, set<int > >::iterator iter_i,iter_j;
    set<int>::iterator iterS;
    FILE * threshDensityFile = fopen( argv[3], "w" ); 
    fprintf( threshDensityFile, "%.6f %.6f " );
    for(thIt = thresholdSet.rbegin(); thIt!=thresholdSet.rend(); thIt++){
        threshold = *thIt;
        if (threshold < 0.0 || threshold > 1.0){
            cout << "ERROR: specified threshold not in [0,1]" << endl;
            exit(1);
        }
        jaccFile.open( argv[2] );
        while ( jaccFile >> edgeId1 >> edgeId2 >> jacc ) {
            if ( jacc >= threshold ) {
                iter_i = edge2iter[ edgeId1 ];
                iter_j = edge2iter[ edgeId2 ];
                if ( iter_i != iter_j ) {
                    // always merge smaller cluster into bigger:
                    if ( (*iter_j).second.size() > (*iter_i).second.size() ){ // !!!!!!
                        swap(iter_i, iter_j);
                    }
                    // merge cluster j into i and update index for all elements in j:
                    for (iterS = iter_j->second.begin(); iterS != iter_j->second.end(); iterS++){
                        iter_i->second.insert( *iterS );
                        edge2iter[ *iterS ] = iter_i;
                    }
                    // delete cluster j:
                    index2cluster.erase(iter_j);
                } 
            } // done merging clusters i and j
        }
        //************* write the clusters to file:
        jaccFile.close();
        cout << "There were " << index2cluster.size() << " clusters at threshold " << threshold << "." << endl;
        
        // all done clustering, write to file (and calculated partition density):
        set<int> clusterNodes;
        int mc, nc;
        int M = 0, Mns = 0;
        double wSum = 0.0;
        set< int >::iterator S;
        map< int, set< int > >::iterator it;
        for ( it = index2cluster.begin(); it != index2cluster.end(); it++ ) {
            clusterNodes.clear();
            for (S = it->second.begin(); S != it->second.end(); S++ ){
                //fprintf( clustersFile, "%i ", *S ); // this leaves a trailing space...!
                clusterNodes.insert(*S); 
            }
            mc = it->second.size();
            nc = clusterNodes.size();
            M += mc;
            if (nc > 2) {
                Mns  += mc;
                wSum += mc * (mc - (nc-1.0)) / ((nc-2.0)*(nc-1.0));
            }
        }  
        D = 2.0 * wSum / M;
        if (isinf(D)){
            fprintf( threshDensityFile, "\nERROR: D == -inf \n\n"); 
            fclose(threshDensityFile);
            exit(1);
        }
        //*************
        fprintf( threshDensityFile, "%.6f %.6f ", threshold, D);
        if(D > highestD){
            highestD = D;
            highestDThr = threshold;
        } 
    }
    fprintf( threshDensityFile, "\n highest D=%.6f at thresh:%.6f.\n", highestD, highestDThr);
    fclose(threshDensityFile);
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}

