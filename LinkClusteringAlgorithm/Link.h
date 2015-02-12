/* 
 * File:   Link.h
 * Author: vivekdhiman
 *
 * Created on 12 February, 2015, 10:31 PM
 */

#ifndef LINK_H
#define	LINK_H
#include "Node.h"
class Link {
public:
    Link();
    Link(Node a, Node b);
    Link(const Link& orig);
    virtual ~Link();
private:
    Node a;
    Node b;
    long long communityId;
};

#endif	/* LINK_H */

