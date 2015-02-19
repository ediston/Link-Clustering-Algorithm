/*
 * File:   SimilarityFileCreator.cpp
 * Author: vivekdhiman
 *
 * Created on 12 December, 2014, 10:34 PM
 */

#include "SimilarityFileCreator.h"

SimilarityFileCreator::SimilarityFileCreator() {
}

SimilarityFileCreator::~SimilarityFileCreator() {
}

void SimilarityFileCreator::print(string s){
    cout << "DEBUG: " << s << endl;
}

void SimilarityFileCreator::printErr(string s){
    cout << "ERROR: " << s << endl;
}

void SimilarityFileCreator::setLinks(char * fileName) {
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
    while (inFile >> a >> b){ // scan edgelist once to get number of edges and nodes, for allocation

        allLinks.push_back(make_pair(min(a,b), max(a,b)));
    }
    inFile.close();
    totalLinks = allLinks.size();
    printf("NOTE : AllLinks created with %d links.\n", totalLinks);
}

pair<ll, ll>  SimilarityFileCreator::getNextMostSimilar() {
    pair<ll, ll> topPair = similarityQ.top().second;
    similarityQ.pop();
    return topPair;
}


void SimilarityFileCreator::getCommonNeighbours(ll nodeA, ll nodeB){
    //if(DEBUG) printf("DEBUG: Getting neighbors for %d, %d. \n", a,b);
    // Include A and B itself first
    totalNeghboursOfA =1, totalNeghboursOfB =1;
    totalNeghbours = 0;
    set <ll> commonNeighboursAB ;
	commonNeighboursAB.insert(nodeA);
	commonNeighboursAB.insert(nodeB);
    for(ll i=0; i<totalLinks; i++){
        if(allLinks[i].first == nodeA){
                totalNeghboursOfA++;
                commonNeighboursAB.insert(allLinks[i].second);
            }
            else if(allLinks[i].second == nodeA){
                totalNeghboursOfA++;
                commonNeighboursAB.insert(allLinks[i].first);
            }
            if(allLinks[i].first == nodeB){
                totalNeghboursOfB++;
                commonNeighboursAB.insert(allLinks[i].second);
            }
            else if(allLinks[i].second == nodeB){
                totalNeghboursOfB++;
                commonNeighboursAB.insert(allLinks[i].first);
            }
    }
    if(DEBUG) {
        cout << "In set are:";
        for(set <ll>::iterator it = commonNeighboursAB.begin();
            it != commonNeighboursAB.end(); it++){
            cout << *it << ", ";
        }
        cout << endl;
    }

    totalNeghbours = commonNeighboursAB.size();
     if(DEBUG) printf("DEBUG: Got %ld common neighbors and totalNeghboursOfA= %lld,  totalNeghboursOfB=%lld. \n",
                            totalNeghbours, totalNeghboursOfA, totalNeghboursOfB);
}

/*
/// Assumption is that there will be no duplication of links
*/

void SimilarityFileCreator::calculateSimilarities(char * fileName) {
    if(DEBUG) print("Starting calculate Similarities.");
    FILE * jaccFile = fopen(fileName,"w");
    if (!jaccFile) {
        printErr("unable to open output file");
        exit(1); // terminate with error
    }else{
        if(DEBUG) print("output file opened.");
    }

    ll nodeA, nodeB, keystoneNode;
    ll unionAB = 0 ;
    ll interAB;
    double similarity;
    pair<ll, ll> l1, l2;
    for(it1=allLinks.begin(); it1<allLinks.end(); it1++){
        for(it2=it1+1; it2<allLinks.end(); it2++){
            // check if there is a common neighbor
            l1 = *it1;
            l2 = *it2;
            if(l1.first == l2.first){
                nodeA = l1.second; nodeB = l2.second;
                keystoneNode = l1.first;
            }else if(l1.second == l2.second){
                nodeA = l1.first; nodeB = l2.first;
                keystoneNode = l1.second;
            }else if(l1.first == l2.second){
                nodeA = l1.second; nodeB = l2.first;
                keystoneNode = l1.first;
            }else if(l1.second == l2.first){
                nodeA = l1.first; nodeB = l2.second;
                keystoneNode = l1.second;
            }else{
                continue;
            }
            if(DEBUG)
                printf("DEBUG: keystone %d found for %d, %d\n", keystoneNode, nodeA, nodeB);
            getCommonNeighbours(nodeA, nodeB);
            interAB = totalNeghboursOfA + totalNeghboursOfB - totalNeghbours;
            if(DEBUG) printf("NOTE: SO %lld, %lld, %lld, %lld\n", totalNeghboursOfA,  totalNeghboursOfB,
                                                    totalNeghbours, interAB);
            similarity = (double)((interAB)/(double)totalNeghbours);
            //if(DEBUG) printf("DEBUG: >>>>> have similarity= %.3f \n",similarity);
            fprintf( jaccFile, "%i\t%i\t%i\t%i\t%f\n",  min(nodeA, keystoneNode), max(nodeA, keystoneNode), min(nodeB, keystoneNode), max(nodeB, keystoneNode),
                                                        similarity);
        }
    }
    fclose(jaccFile);
}




int main(int argc, char** argv) {
    printf("--------------------\n");
    printf("--------------------\n");
    /// vector of all the edges
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
    SimilarityFileCreator graph;
    graph.setLinks(argv[1]);
    graph.calculateSimilarities(argv[2]);

    printf("\n--------------------\n");
    return 0;
}


