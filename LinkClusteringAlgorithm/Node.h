/*
 * File:   Node.h
 * Author: vivekdhiman
 *
 * Created on 12 December, 2014, 10:33 PM
 */

#ifndef NODE_H
#define	NODE_H

#include <vector>
#include <algorithm>
#include <map>

#define ll long long
using namespace std;

class Node {
public:
    Node();
    Node(const Node& orig);
    virtual ~Node();
private:
    long long nodeId;
};

#endif	/* NODE_H */

