/*
 * File:   main.cpp
 * Author: vivekdhiman
 *
 * Created on 12 December, 2014, 10:10 PM
 */
#include <cstdlib>
#include <iostream>
#include "Graph.h"
using namespace std;

/*
 *
 */
int main(int argc, char** argv) {
    Graph graph;
    vector < pair<ll, ll> > links(78);
    int a,b;
    while(cin >> a >> b){
       links.push_back(make_pair(a,b));
    }
    graph.setLinks(links);
    return 0;
}

