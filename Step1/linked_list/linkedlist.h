#include <bits/stdc++.h>
using namespace std;

struct node{
    long long nodeId;
    node *next;
};

struct headNode{
    long long size;
    node *next;
};

node *getNewNode(){
  node * newnode = new node;
  newnode->next = NULL;
  newnode->nodeId = 0;
  return newnode;
}

headNode *getNewHeadNode(){
  headNode * newnode = new headNode;
  newnode->next = NULL;
  newnode->size = 0;
  return newnode;
}

void addNode(headNode *head, long long newNodeId){
  node *tempnode = getNewNode();
  node *prevnode = getNewNode();
  node*newNode = getNewNode();
  prevnode = NULL;
  tempnode = head->next;
  while(tempnode && tempnode->nodeId<newNodeId){
    prevnode = tempnode;
    tempnode = tempnode->next;
  }
  head->size++;
  newNode->nodeId = newNodeId;
  if(prevnode) prevnode->next = newNode;
  else head->next = newNode;
  newNode->next = tempnode;
}

long long getCommonNodesCount(headNode *head1, headNode *head2){
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

void deleteLL(headNode *head){
    node *ll1node = head->next;
    node *ll2node = head->next;
    delete head;
    while(ll1node){
        ll2node = ll1node->next;
        delete ll1node;
        ll1node = ll2node;
    }
}
