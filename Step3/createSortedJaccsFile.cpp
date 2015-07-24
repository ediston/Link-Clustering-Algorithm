/*
1. Treat each edge id as a cluster, unordered map from edgeId to a set
2. also store size of each cluster which will 0 in the begining
//  $ g++ -std=c++0x -O3 -o calcDensityForDiffThresh calcDensityForDiffThresh.cpp
//  $ ./calcDensityForDiffThresh networkEdgeIdMap.csv network.jaccs sortedjaccs.csv  Nthresholds threshDensity.csv
*/
#include <math.h> 
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <set>
#include <map>
#include <utility>   // for pairs
#include <algorithm> // for swap
using namespace std;

 

int main (int argc, char const *argv[]){
    //************* make sure args are present:
    if (argc != 6){
        cout << "ERROR: something wrong with the inputs" << endl;
        cout << "usage:\n    " << argv[0] << " networkEdgeIdMap.csv network.jaccs sortedjaccs.csv  "
                    << " Nthresholds threshDensity.csv" << endl;
        exit(1);
    }
    // Nthresholds is the total thresholds user wants us to try
    int nThresh = atoi(argv[4]);
    int gapBetweenthresh = 0;
    float threshold = 0;
    float D = 0.0;
    int mc, nc;
    int M = 0, Mns = 0;
    double wSum = 0.0;
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
    // store edge ids in a map
    // each edge id will be a cluster of it's own in the begining
    // each cluster will be og size 1
    unordered_map< int,  pair<int,int> > idEdgePairMap;
    unordered_map< int,  set<int > > index2cluster; // O(log n) access too slow?
    unordered_map< int,  unordered_map< int, set<int > >::iterator > edge2iter;
    int ni, nj, edgeId, index = 0;
    while (inFile >> ni >> nj >> edgeId){ 
        idEdgePairMap[ edgeId ] = make_pair(ni,nj);
        index2cluster[ index ].insert(edgeId);         // build cluster index to set of edge-pairs map
        edge2iter[edgeId] = index2cluster.find(index); 
        index++;
    }
    inFile.close(); inFile.clear();
    cout << "There were " << index2cluster.size() << " edges." << endl;
    /// end reading edge ids ----- 
    
    //----- ----- ----- ----- 
    ifstream sortedjaccFile;  
    sortedjaccFile.open( argv[3] );
    if (!sortedjaccFile) {
        cout << "ERROR: unable to open sortedjaccFile file" << endl;
        exit(1); // terminate with error
    }
    int totalThresh = 0;
    int edgeId1,edgeId2;
    float jacc; 
    // Count totalines in the file, we are setting 
    while ( sortedjaccFile  >> jacc ) {
        if(jacc>0 && jacc<=1.0)
            totalThresh++;
    }
    sortedjaccFile.close(); sortedjaccFile.clear();
    cout << "Done counting totalines in the file, \nTotal unique Jaccs = " << totalThresh << endl;
    if(totalThresh==0){
        cout << "ERROR: there are 0 Jaccs!!!" << endl;
        exit(1); // terminate with error
    }
    // now we want gap between two consecutive thresholds
    // we want to consider
    gapBetweenthresh =  totalThresh/nThresh;
    if(totalThresh < nThresh)
            gapBetweenthresh = totalThresh;
    set<float> thresholdSet;
    set<float> ::reverse_iterator thIt;
    // ----------------------------
    totalThresh = -1;
    sortedjaccFile.open( argv[3] );
    while ( sortedjaccFile  >> jacc ){
        totalThresh++;
        if(totalThresh%gapBetweenthresh!=0){
            continue;
        }
        thresholdSet.insert(jacc);
    }
    thresholdSet.insert(1.1);
    cout << "Done with thresholdSet creation." << endl;
    // ---------------------------

    ifstream jaccFile; 
    jaccFile.open(argv[2]);
    // read first line
    jaccFile >> edgeId1 >> edgeId2 >> jacc ;
    // open the outputfile
    unordered_map< int, set< int> >::iterator iter_i,iter_j;
    set<int>::iterator iterS;
    FILE * threshDensityFile = fopen( argv[3], "w" ); 
    fclose(threshDensityFile);
    fprintf( threshDensityFile, "thresh  D\n" );
    long long done = 0;
    float percDone = 0.01;
    clock_t lastBegin = clock(); 
    for(thIt = thresholdSet.rbegin(); thIt!=thresholdSet.rend(); thIt++){
        threshold = *thIt;
        if (threshold < 0.0 || threshold > 1.1){
            cout << "ERROR: specified threshold not in [0,1]" << endl;
            exit(1);
        }
        do{
            if( jacc < threshold ) 
                break; 
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
        }while ( jaccFile >> edgeId1 >> edgeId2 >> jacc );        
        // all done clustering, write to file (and calculated partition density):
        M = 0, Mns = 0;
        wSum = 0.0;
        set< int >::iterator S;
        set<int> clusterNodes;
        unordered_map< int, set< int > >::iterator it;
        for ( it = index2cluster.begin(); it != index2cluster.end(); it++ ) {
            clusterNodes.clear();
            for (S = it->second.begin(); S != it->second.end(); S++ ){
                //fprintf( clustersFile, "%i ", *S ); // this leaves a trailing space...!
                clusterNodes.insert(idEdgePairMap[*S].first);
                clusterNodes.insert(idEdgePairMap[*S].second);
            }
            mc = it->second.size();
            nc = clusterNodes.size();
            M += mc;
            if (nc > 2) {
                Mns  += mc;
                wSum += mc * (mc - (nc-1.0)) / ((nc-2.0)*(nc-1.0));
            }
        }  
        threshDensityFile = fopen( argv[3], "a" ); 
        D = 2.0 * wSum / M;
        if (isinf(D)){
            fprintf( threshDensityFile, "\nERROR: D == -inf \n\n"); 
            fclose(threshDensityFile);
            exit(1);
        }
        //*************
        fprintf( threshDensityFile, "%.6f %.6f \n", threshold, D);
        fclose(threshDensityFile);
        if(D > highestD){
            highestD = D;
            highestDThr = threshold;
        } 
        done++;
        if((float)done/(float)thresholdSet.size() >= percDone){
            cout << percDone*100 << " pc done.\n" << endl;
            percDone += 0.01;
        }
        cout << "Time taken = " << double(clock() - lastBegin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
        lastBegin = clock();
    }
    jaccFile.close(); jaccFile.clear();
    fprintf( threshDensityFile, "\n highest D=%.6f at thresh:%.6f.\n", highestD, highestDThr);
    fclose(threshDensityFile);
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}
