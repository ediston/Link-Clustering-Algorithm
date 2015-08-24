#include <bits/stdc++.h>
#include    <ctime>
using namespace std;
#define MAX_HIEGHT 32
#define ONEIN 2
#define INFINITY 99999999

//long long totalComparisons;

int getRandom(){
     return rand() % ONEIN;
}

struct node{
    long long nodeId;
    node *next;
    node *below;
};

struct skipList{
    int height;
    long long listId;
    long long length;
    node **head;
};

node *getNewNode(long long nodeId){
    node * newnode = new node; 
    newnode->nodeId = nodeId;
    newnode->next = NULL;
    newnode->below = NULL;
    return newnode;
}

skipList *getNewSkipList(){
    skipList * newSkipList = new skipList;
    newSkipList->height = 0;
    // head has the maximum height
    newSkipList->head = new node*[MAX_HIEGHT];
    for(int i=0; i<MAX_HIEGHT; i++){
        newSkipList->head[i] = getNewNode(-INFINITY);
        if(i==0)
            newSkipList->head[i]->below = NULL;
        else 
            newSkipList->head[i]->below = newSkipList->head[i-1];
    }
    newSkipList->length = 0;
    return newSkipList;
}

void printSkipList(skipList *s){
    if(!s) return;
    cout << "Skiplist #" << s->listId << endl;
    node *current = getNewNode(0);
    for(int lvl = s->height-1; lvl>= 0; lvl-- ){
        cout << "Height " << lvl << ": ";
        current = s->head[lvl];
        while(current){
            cout  << " - " << current->nodeId ;
            current = current->next;
        }
        cout << endl;
    }
    cout << endl;
}

void addNode(skipList *s, long long newNodeId){
    // first we decide the height of the new node
    // height = level+1
    int newNodeLevel = 0, level  ;

    for(;getRandom() >= 1 ; newNodeLevel++) {
        if(newNodeLevel==MAX_HIEGHT-1) {
            break;
        }
    }
    // change the height of the skiplist if needed
    if(newNodeLevel+1 > s->height){
        s->height = newNodeLevel+1;
    }
    
    node *prev = getNewNode(0); ;
    node *current = getNewNode(0); 
    prev = NULL;
    // start from max height of the skip list
    level = s->height-1;
    current = s->head[level];
    // A good video explaining the insertion
    // https://www.youtube.com/watch?v=Dx7Hk8-8Kdw

    // Loop while we go through all levels
    while(current){
        // This will help us reach the appropriate node already
        while(current->next && current->next->nodeId < newNodeId){
            current = current->next;
        }
        // if this level is greater than new level
        // then we add the a new node to next of current
        if(level <= newNodeLevel){
            node *newNode = getNewNode(newNodeId);
            if(prev){
                prev->below = newNode;
            }
            newNode->next = current->next;
            current->next = newNode;
            prev = newNode;
        }
        current = current->below;
        level--;
    }
    // increase the length of the skip list
    s->length++;
}

node * searchAndGetNearestNode(node *curr, long long &nodeId){
    if(!curr) return NULL;
    if(curr->nodeId >= nodeId)
        return curr;
    while(curr->next && curr->next->nodeId < nodeId){
        curr = curr->next;
    }
    if(curr->nodeId >= nodeId)
        return curr;
    if(!curr->below)
        return curr->next;
    return searchAndGetNearestNode(curr->below, nodeId);
}

long long getCommonNodesCount(skipList *s1, skipList *s2){
    long long count = 0;
    int lvl1=0, lvl2= 0;
    if(s1->length > s2->length)
        swap(s1, s2);
    node *nextn1 = getNewNode(0);
    node *n1 = getNewNode(0);
    node *n2 = getNewNode(0);
    n1 = s1->head[0]->next;
    n2 = s2->head[s2->height-1];
    while(n1 && n2){
        if(n1->nodeId > n2->nodeId){
            n2 = searchAndGetNearestNode(n2, n1->nodeId);
        }else if(n1->nodeId < n2->nodeId){
            n1 = n1->next;
        }else{
            count++;
            n1 = n1->next;
        }
    }
    return count;
}

void deleteLL(skipList *s){ 
    for(int i=0; i<s->height; i++){
        node *ll1node = s->head[i];
        node *ll2node = ll1node;
        while(ll1node){
            ll2node = ll1node;
            ll1node = ll1node->next;
            delete ll2node;
        }
    }
}
