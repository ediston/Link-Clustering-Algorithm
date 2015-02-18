/*
 * File:   Graph.cpp
 * Author: vivekdhiman
 *
 * Created on 12 December, 2014, 10:34 PM
 */

#include "Graph.h"

Graph::Graph() {
}

Graph::~Graph() {
}

void Graph::setLinks(vector < pair<ll, ll> > links) {
    totalLinks = links.size();
    allLinks = links;
}

pair<ll, ll>  Graph::getNextMostSimilar() {
    pair<ll, ll> topPair = similarityQ.top().second;
    similarityQ.pop();
    return topPair;
}

int Graph::getNeighbours(int a){
    int sum = 0;
    for(int i=0; i<allLinks.size(); i++){
        if(allLinks[i].first == a || allLinks[i].second == a ) sum++;
    }
    return sum-1;
}

int Graph::getCommonNeighbours(int a, int b){
    int sum = 0;
    for(int i=0; i<allLinks.size(); i++){
        if(allLinks[i].first == a || allLinks[i].first == b ||
           allLinks[i].second == a || allLinks[i].second == b) sum++;
    }
    return sum-1;
}

void Graph::calculateSimilarities() {
    vector<set<ll> >totalNeghbours(totalLinks, 0);
    long nodeA, nodeB, keystoneNode;
    long unionAB;
    long interAB;
    double similarity;
    for(int i=0; i<allLinks.size(); i++){

    }
    for(int i=0; i<allLinks.size(); i++){
        for(int j=i+1; j<allLinks.size(); j++){
            if(allLinks[i].first == allLinks[j].first){
                nodeA = allLinks[i].second; nodeB = allLinks[j].second;
                keystoneNode = allLinks[i].first;
            }else if(allLinks[i].second == allLinks[j].second){
                nodeA = allLinks[i].first; nodeB = allLinks[j].first;
                keystoneNode = allLinks[i].second;
            }else if(allLinks[i].first == allLinks[j].second){
                nodeA = allLinks[i].second; nodeB = allLinks[j].first;
                keystoneNode = allLinks[i].first;
            }else if(allLinks[i].second == allLinks[j].first){
                nodeA = allLinks[i].first; nodeB = allLinks[j].second;
                keystoneNode = allLinks[i].second;
            }else{
                continue;
            }
            totalNeghboursOfA =0; totalNeghboursOfB =0; totalNeghbours = 0;
            getNeighbours(nodeA, nodeB, &totalNeghboursOfA, &totalNeghboursOfB, &totalNeghbours);
            unionAB = totalNeghboursOfA + totalNeghboursOfB - totalNeghbours;
            similarity = (double)(interAB)/(double)unionAB;
            similarityQ.push(make_pair(similarity, make_pair(i,j)));
        }
    }
}




