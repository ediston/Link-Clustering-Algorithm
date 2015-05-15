#include <bits/stdc++.h>
using namespace std;
#define MAX_HIEGHT 32
#define ONEIN 2

int getRandom(){
     return rand() % ONEIN;
}

struct node{
    int height;
    long long nodeId;
    node **next;
};

struct skipList{
    int height;
    long long size;
    node **next;
};

node *getNewNode(int height){
    node * newnode = new node;
    newnode->height = height;
    newnode->nodeId = 0;
    newnode->next = new node*[height];
    return newnode;
}

skipList *getNewSkipList(){
    skipList * newSkipList = new skipList;
    newSkipList->height = 0;
    newSkipList->next = new node*[MAX_HIEGHT];
    for(int i=0; i<MAX_HIEGHT; i++) newSkipList->next[i] = NULL;
    newSkipList->size = 0;
    return newSkipList;
}

void addNode(skipList *s, long long newNodeId){
    int level = 0;
	for(;getRandom() == 1 ; level++) {
        if(level+1 > s->height || level==MAX_HIEGHT-1) {
            s->height = level+1;
            break;
        }
	}
	node *prev = getNewNode(0);
    node *current = getNewNode(0);
    node *newNode = getNewNode(level);
    prev = NULL;
    newNode->nodeId = newNodeId;
    current = NULL;
    for(int i = s->height-1; i >= 0; i-- ){
        if(!current){
            current = s->next[i];
        }
        for(;current; current = current->next[i]){
			if(current->nodeId > newNodeId){
				break;
			}
			prev = current;
            if(!current->next[i]) break;
		}
		if( i > level ){
			continue;
		}
		if(current->nodeId > newNodeId) newNode->next[i] = current;
		else newNode->next[i] = NULL;
		if(prev){
            prev->next[i] = newNode;
            if(prev->nodeId != current->nodeId) current = prev;
		}
		else s->next[i] = newNode;
	}
    s->size++;
}

int findInSkipList(node *curr, int *lvl, long long nodeid){
    node *prev = curr;
    while(curr->next[*lvl] && curr->nodeId < nodeid){
        curr = curr->next[*lvl];
        prev = curr;
    }
    if(curr->nodeId == nodeid){
        *lvl = curr->height;
        return 1;
    }else{
        if(curr->nodeId > nodeid && *lvl>0){
            *lvl = max(*lvl - 1, 0);
            curr = prev;
            return findInSkipList(curr, lvl, nodeid);
        }
        *lvl = max(*lvl - 1, 0);
        return 0;
    }
}

long long getCommonNodesCount(skipList *s1, skipList *s2){
    long long count = 0;
    if(s1->size > s2->size) swap(s1, s2);
    int lvl = s2->height-1;
    node *tempNode = getNewNode(0);
    node *currNode = getNewNode(0);
    currNode = s2->next[lvl-1];
    tempNode = s1->next[0];
    while(tempNode && lvl>=0){
        if(findInSkipList(currNode, &lvl, tempNode->nodeId)){
            count++;
        }
        tempNode = tempNode->next[0];
    }
    return count;
}

void deleteLL(skipList *s){
    node *ll1node = s->next[0];
    node *ll2node = s->next[0];
    delete s;
    while(ll1node){
        ll2node = ll1node->next[0];
        delete ll1node;
        ll1node = ll2node;
    }
}
