#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <iostream>

#define DEBUG 0
#define ll long long

using namespace std;

//--- Union set ----
map < pair<ll, ll>, ll > allLinks;
long totalLinks;
priority_queue < pair<double, pair<ll, ll> > > queueOfPairsOfLinks;

void print(string s){
    cout << "DEBUG: " << s << endl;
}

void printErr(string s){
    cout << "ERROR: " << s << endl;
}

vector <long long> community,  rank;
void initUnionFind(ll N) {
    rank.assign(N, 0);
    community.assign(N, 0);
    for (ll i = 0; i < N; i++)
        community[i] = i;
}

ll findCommunity(ll i) {
    return (community[i] == i) ? i : (community[i] = findCommunity(community[i]));
}

bool isSameCommunity(ll i, ll j) {
    return findCommunity(i) == findCommunity(j);
}

void unionCommunity(ll i, ll j) {
    ll x = findCommunity(i); ll y = findCommunity(j);
    if (x = y) { // if from different set
        if (rank[x] > rank[y])
            community[y] = x; // rank keeps the tree short
        else {
            community[x] = y;
            if (rank[x] == rank[y]) rank[y]++;
        }
    }
}
//---------------------------------------------------
void setLinks(char * fileName) {
    if(DEBUG) print("Starting creating links.");
    ifstream inFile;
    inFile.open (fileName);
    if (!inFile) {
        printErr("unable to open input file.");
        exit(1); // terminate with error
    }else{
        if(DEBUG) print("input file opened.");
    }
    ll a,b;
    totalLinks = 0;
    while (inFile >> a >> b){ // scan edgelist once to get number of edges and nodes, for allocation
        allLinks[(make_pair(min(a,b), max(a,b)))] = totalLinks;
        totalLinks++;
    }
    inFile.close();
    printf("NOTE : AllLinks created with %d links.\n", totalLinks);
}

void createQueueOfPairsOfLinks(char * fileName){
    if(DEBUG) print("Starting creating links.");
    ifstream inFile;
    inFile.open (fileName);
    if (!inFile) {
        printErr("unable to open input file.");
        exit(1); // terminate with error
    }else{
        if(DEBUG) print("input file opened.");
    }
    double similarity;
    ll a,b,c,d;
    while (inFile >> a >> b >> c >> d >> similarity){ // scan edgelist once to get number of edges and nodes, for allocation
        queueOfPairsOfLinks.push(make_pair( similarity, (make_pair
                                                            (allLinks[make_pair(min(a,b), max(a,b))],
                                                                       allLinks[make_pair(min(c,d), max(c,d))]))
                                           )
                                 );
    }
    inFile.close();
    printf("NOTE : QUEUE created.\n");
}

void createClusters(float threshold){
    double similarity;
    ll link1, link2;
    pair<ll ,ll> linkPair;
    while(!queueOfPairsOfLinks.empty()){
        similarity = queueOfPairsOfLinks.top().first;
        if(similarity < threshold) break;
        linkPair = queueOfPairsOfLinks.top().second;
        unionCommunity(linkPair.first, linkPair.second);
        queueOfPairsOfLinks.pop();
    }
}

void printToFile(char * fileName){
    FILE * jaccFile = fopen(fileName,"w");
    if (!jaccFile) {
        printErr("unable to open output file");
        exit(1); // terminate with error
    }else{
        if(DEBUG) print("output file opened.");
    }
    bool atleastOne;
    set <ll> tempSet;
    for(ll link = 0; link < totalLinks; link++){
        tempSet.insert(link);
    }
    for(ll c = 0;  c<totalLinks; c++){
        atleastOne = false;
        for(set <ll> ::iterator it=tempSet.begin(); it != tempSet.end(); ){
            if(community[*it] == c){
                fprintf( jaccFile, "%i,%i ", allLinks[*it].first, allLinks[*it].second);
                atleastOne = true;
                tempSet.erase(*it);
            }else{
                it++;
            }
        }
        if(atleastOne)fprintf(jaccFile,"\t >> %i\n", c);
    }
    fprintf( jaccFile, "%i\t%i\t%i\t%i\t%f\n",  min(nodeA, keystoneNode), max(nodeA, keystoneNode), min(nodeB, keystoneNode), max(nodeB, keystoneNode),
                                                        similarity);
    fclose(jaccFile);
}

int main(int argc, char** argv) {
    printf("--------------------\n");
    printf("--------------------\n");
    if (!argv[1]){
        cout << "ERROR: no input file specified" << endl;
        cout << "usage:\n    " << argv[0] << " input.pairs output.jaccs" << endl;
        exit(1);
    }
    if (!argv[2]){
        cout << "ERROR: no output file specified" << endl;
        cout << "usage:\n    " << argv[0] << " input.pairs output.jaccs" << endl;
        exit(1);
    }
    if (argc != 6){
        cout << "ERROR: something wrong with the inputs" << endl;
        cout << "usage:\n    " << argv[0] << " network.pairs network.jaccs network.clusters network.cluster_stats threshold" << endl;
        exit(1);
    }
    float threshold = atof( argv[5] );
    if (threshold < 0.0 || threshold > 1.0){
        cout << "ERROR: specified threshold not in [0,1]" << endl;
        exit(1);
    }
    setLinks(argv[1]);
    initUnionFind(totalLinks);
    createQueueOfPairsOfLinks(argv[2]);
    createClusters(threshold);
    printToFile(argv[4]);
    printf("\n--------------------\n");
    return 0;
}
