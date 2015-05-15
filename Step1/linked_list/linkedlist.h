/*


*/
#include <bits/stdc++.h>
using namespace std;

struct node{
  long long nodeId;
  node *next;
};

node *getNewNode(){
  node * newnode = new node;
  newnode->next = NULL;
  return newnode;
}

void addNode(node *head, long long newNodeId){
  node *tempnode = getNewNode();
  node *prevnode = getNewNode();
  prevnode = head;
  tempnode = head->next;
  while(tempnode && tempnode->nodeId<newNodeId){
    prevnode = tempnode;
    tempnode = tempnode->next;
  }
  node*newNode = getNewNode();
  newNode->nodeId = newNodeId;
  prevnode->next = newNode;
  newNode->next = tempnode;
}

long long getCommonNodesCount(node *head1, node *head2){
  long long count = 0;
  node *ll1node = getNewNode();
  node *ll2node = getNewNode();
  ll1node = head1->next;
  ll2node = head2->next;
  while(ll1node && ll2node){
    if(ll1node->nodeId<ll2node->nodeId){
      ll1node = ll1node->next;
    }else if(ll2node->nodeId<ll1node->nodeId){
      ll2node = ll2node->next;
    }else{
      ll1node = ll1node->next;
      ll2node = ll2node->next;
      count++;
    }
  }
  return count;
}

