/* 
 * File:   Graph.h
 * Author: vivekdhiman
 *
 * Created on 12 February, 2015, 10:34 PM
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include "Node.h"
#include "Link.h"

class Graph {
public:
    Graph();
    Graph(const Graph& orig);
    virtual ~Graph();
    double getSimilarity(Node);
    void calculateSimilarities();
private:
    double threshold;
    map < pair<long long> , double> similarity;
    vector <Link> links;
    set < set<int > > clusters;
    int totalLinks;
    int totalCommunities;
};

#endif	/* GRAPH_H */

