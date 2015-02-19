/*
 * File:   SimilarityFileCreator.h
 * Author: vivekdhiman
 *
 * Created on 12 December, 2014, 10:34 PM
 */

#ifndef SimilarityFileCreator_H
#define	SimilarityFileCreator_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <queue>
#include <set>
#include <cstdlib>
#include <iostream>

#define DEBUG 0


#define ll long long

using namespace std;

class SimilarityFileCreator {
public:
    SimilarityFileCreator();
    void print(string s);
    void printErr(string s);
    void setLinks(char * fileName) ;
    virtual ~SimilarityFileCreator();
    pair<ll, ll>   getNextMostSimilar();
    void calculateSimilarities(char * fileName);
    void getCommonNeighbours(ll nodeA, ll node);
private:
    double threshold;
    priority_queue < pair<double, pair<ll, ll> >  > similarityQ; // between two links with shared node
    vector < pair<ll, ll> > allLinks;
    vector < pair<ll, ll> >::iterator it1, it2, _it;
    ll totalLinks, totalCommunities, totalNodes;
    ll totalNeghboursOfA , totalNeghboursOfB, totalNeghbours;
    
};

#endif	/* GRAPH_H */


