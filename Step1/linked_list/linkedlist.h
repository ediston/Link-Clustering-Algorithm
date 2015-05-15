/*


*/
#include <bits/stdc++.h>
struct node{
  long long nodeid;
  node *next;
}

node *newnode();
void addNode(long long nodeid);
node * getLargestSmallerNode();
long long getCommonNodesCount(node *head1, node *head2);
