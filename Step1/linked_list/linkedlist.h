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

/* function prototypes */
node *getNewNode();
void MergeSort(struct node** headRef);
headNode *getNewHeadNode();
void addNode(headNode *head, long long newNodeId);
long long getCommonNodesCount(headNode *head1, headNode *head2);
void printLL(headNode *head);
void deleteLL(headNode *head);
struct node* SortedMerge(struct node* a, struct node* b);
void FrontBackSplit(struct node* source, struct node** frontRef, struct node** backRef);


/* sorts the linked list by changing next pointers (not nodeId) */
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
  node*newNode = getNewNode();
  newNode->nodeId = newNodeId;
  newNode->next = head->next;
  head->next = newNode;
  head->size++;
}

void addNode2(headNode *head, long long newNodeId){ // approach 2
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

void printLL(headNode *head){
    node *ll1node = getNewNode();
    ll1node = head->next;
    while(ll1node){
        cout << ll1node->nodeId << ", ";
        ll1node = ll1node->next;
    }
    cout << endl;
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


/* sorts the linked list by changing next pointers (not nodeId) */
void MergeSort(struct node** headRef)
{
  struct node* head = *headRef;
  struct node* a;
  struct node* b;

  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL))
  {
    return;
  }

  /* Split head into 'a' and 'b' sublists */
  FrontBackSplit(head, &a, &b); 

  /* Recursively sort the sublists */
  MergeSort(&a);
  MergeSort(&b);

  /* answer = merge the two sorted lists together */
  *headRef = SortedMerge(a, b);
}

struct node* SortedMerge(struct node* a, struct node* b)
{
  struct node* result = NULL;

  /* Base cases */
  if (a == NULL)
     return(b);
  else if (b==NULL)
     return(a);

  /* Pick either a or b, and recur */
  if (a->nodeId <= b->nodeId)
  {
     result = a;
     result->next = SortedMerge(a->next, b);
  }
  else
  {
     result = b;
     result->next = SortedMerge(a, b->next);
  }
  return(result);
}
 
/* UTILITY FUNCTIONS */
void FrontBackSplit(struct node* source,
          struct node** frontRef, struct node** backRef)
{
  struct node* fast;
  struct node* slow;
  if (source==NULL || source->next==NULL)
  {
    /* length < 2 cases */
    *frontRef = source;
    *backRef = NULL;
  }
  else
  {
    slow = source;
    fast = source->next;
 
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
      fast = fast->next;
      if (fast != NULL)
      {
        slow = slow->next;
        fast = fast->next;
      }
    }
    /* 'slow' is before the midpoint in the list, so split it in two
      at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}
