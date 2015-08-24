#include <bits/stdc++.h>
using namespace std;
#include <math.h>  
/*
Adamic/Adar based similarity
*/

struct node{
    long long nodeId;
    node *next;
};

struct headNode{
    long long size;
    node *next;
};

map<long long, headNode*> nodeids;
map<long long, headNode*> :: iterator it;
map<pair <long, long>, double> storedSimilarityHashTable;

/* function prototypes */
node *getNewNode();
void MergeSort(struct node** headRef);
headNode *getNewHeadNode();
void addNode(headNode *head, long long newNodeId);
double calculateAdamicAdar(headNode *head1, headNode *head2);
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

double calculateAdamicAdar(headNode *head1, headNode *head2){
  double totalSim = 0, sim = 0; 
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
      if(nodeids.count(ll1node->nodeId) && nodeids[ll1node->nodeId]->size>1) 
        sim = 1/log(nodeids[ll1node->nodeId]->size);
      ll1node = ll1node->next;
      ll2node = ll2node->next; 
      totalSim += sim;
    }
  }
  return totalSim;
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



int main (int argc, char const *argv[]){
    double minJaccs = 223232, maxJaccs = -1;
    clock_t begin = clock();
    // make sure args are present:
    if (!argv[1]){
        cerr << "ERROR: no input file specified" << endl;
        cerr << "usage:\n    " << argv[0] << " input.pairs output.jaccs" << endl;
        exit(1);
    }
    if (!argv[2]){
        cerr << "ERROR: no output file specified" << endl;
        cerr << "usage:\n    " << argv[0] << " input.pairs output.jaccs" << endl;
        exit(1);
    }


    // load edgelist into linkedlists
    ifstream inFile;
    inFile.open (argv[1]);
    if (!inFile) {
        cerr << "ERROR: unable to open input file" << endl;
        exit(1); // terminate with error
    }
    long long ni,nj;
    headNode *head1, *head2;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        if(nodeids.count(ni)==0){
            head1 = getNewHeadNode();
            nodeids[ni] = head1;
            addNode(head1, ni); // we include this node itself
        }else head1 = nodeids[ni];
        if(nodeids.count(nj)==0){
            head2 = getNewHeadNode();
            nodeids[nj] = head2;
            addNode(head2, nj); // we include this node itself
        }else head2 = nodeids[nj];
        addNode(head1, nj);
        addNode(head2, ni);
    }
    inFile.close();

    // end load edgelist

    // iterate through the map of nodeIds

    // do the gosh darn calculation, fool!
    FILE * jaccFile = fopen(argv[2],"w");
    long long keystone, len_int;
    headNode *head, *head_i, *head_j;
    node *n_i, *n_j;
    head = getNewHeadNode(); n_i = getNewNode(); n_j = getNewNode();
    head_i = getNewHeadNode(); head_j = getNewHeadNode();
    double curr_aa;
    // Let's sort the linked list
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        head = (*it).second;
        MergeSort(&(head->next));
    }
    
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        keystone = (*it).first;
        head = (*it).second;
        n_i = head->next;
        while(n_i && n_i->next) { // neighbors of keystone in the linkedlist
            if (n_i->nodeId == keystone){
                //cout << "n_i->nodeId == keystone" << endl;
                n_i = n_i->next;
                continue;
            }
            head_i = nodeids[n_i->nodeId];
            n_j = n_i->next;
            while(n_j) { // next neighbors of n_k
                head_j = nodeids[n_j->nodeId];
                if (n_j->nodeId == keystone || n_i->nodeId >= n_j->nodeId){
                    //cout << "n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId" << endl;
                    n_j = n_j->next;
                    continue;
                }
                //cout << keystone << ": " << n_i->nodeId << " " << n_j->nodeId  << " | "
                //        <<head_i->size << " " << head_j->size << " || " ;
                if(storedSimilarityHashTable.count(make_pair(n_i->nodeId, n_j->nodeId))){
                    curr_aa = storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)];
                }else{
                    //cout << len_int << ", ";
                    curr_aa = calculateAdamicAdar(head_i, head_j);  
                    storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)] = curr_aa;
                }
                minJaccs = min(minJaccs, curr_aa);
                maxJaccs = max(maxJaccs, curr_aa);
                //cout << curr_aa << endl;
                if (keystone < n_i->nodeId && keystone < n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i->nodeId, keystone, n_j->nodeId, curr_aa );
                } else if (keystone < n_i->nodeId && keystone > n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i->nodeId, n_j->nodeId, keystone, curr_aa );
                } else if (keystone > n_i->nodeId && keystone < n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i->nodeId, keystone, keystone, n_j->nodeId, curr_aa );
                } else {
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i->nodeId, keystone, n_j->nodeId, keystone, curr_aa );
                }
                n_j = n_j->next;
            }
            n_i = n_i->next;
        }
    } // done loop over keystones
    fclose(jaccFile);
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        head = (*it).second;
        deleteLL(head);
    }
    cout << "minJaccs = " << minJaccs << endl;
    cout << "maxJaccs = " << maxJaccs << endl;
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC  << " seconds. "<< endl;
    return 0;
}
