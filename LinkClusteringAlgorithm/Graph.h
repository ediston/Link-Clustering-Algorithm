/*
 * File:   Graph.h
 * Author: vivekdhiman
 *
 * Created on 12 December, 2014, 10:34 PM
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include "Node.h"
#include "Link.h"
#include <queue>


class Graph {
public:
    Graph();
    void setLinks(vector < pair<ll, ll> > links);
    virtual ~Graph();
    pair<ll, ll>   getNextMostSimilar();
    void calculateSimilarities();
    int getNeighbours(int a);
    int getCommonNeighbours(int a, int b);
private:

    double threshold;
    priority_queue < pair<double, pair<ll, ll> >  > similarityQ; // between two links with shared node
    vector < pair<ll, ll> > allLinks;
    ll totalLinks;
    ll totalCommunities;

    //--- Union set ----
    vector <long long> community,  rank ;
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
        if (!isSameCommunity(i, j)) { // if from different set
            ll x = findCommunity(i), y = findCommunity(j);
            if (rank[x] > rank[y])
                community[y] = x; // rank keeps the tree short
            else {
                community[x] = y;
                if (rank[x] == rank[y]) rank[y]++;
            }
        }
    }
};

#endif	/* GRAPH_H */

