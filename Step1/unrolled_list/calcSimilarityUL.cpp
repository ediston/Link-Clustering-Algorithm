/*
Vivek Dhiman
TU KL
vivek4dhiman@gmail.com
*/

#include <bits/stdc++.h>
#define MAX_ELEMENTS_IN_A_NODE 10
using namespace std;
long long totalComparisons;

set<long long> :: iterator listIt;
struct node{
    set<long long> elems;
    node *next;
};

struct unrolledList{
    long long size;
    long long listid;
    node *next;
};

node *getNewNode(){
  node * newnode = new node;
  newnode->next = NULL;
  return newnode;
}

unrolledList *getNewHeadNode(){
    unrolledList * ul = new unrolledList;
    ul->next = NULL;
    ul->size = 0;
    return ul;
}

void printList(unrolledList *head){
    if(!head) return;
    cout << "unrolledList #" << head->listid  << ": " << endl;
    node *tempnode = getNewNode();
    tempnode = head->next;
    while(tempnode){
        listIt = tempnode->elems.begin();
        while(listIt!=tempnode->elems.end()){
            cout << *(listIt) << " - ";
            listIt++;
        }
        tempnode = tempnode->next;
    }
    cout << endl << "Total Size=" << head->size << endl;
}

void addNode(unrolledList *head, long long newNodeId){
    node *tempnode = getNewNode();
    node *prevnode = getNewNode();
    prevnode = NULL;
    tempnode = head->next;
    while(tempnode && (*(tempnode->elems.begin())<newNodeId)){
        prevnode = tempnode;
        tempnode = tempnode->next;
    }
    if(!prevnode){
        if(tempnode && tempnode->elems.size() < MAX_ELEMENTS_IN_A_NODE){
            // if atleast one node and first node has some space
            // also if
            tempnode->elems.insert(newNodeId);
        }else{
            // if no element or no space
            node *newNode = getNewNode();
            head->next = newNode;
            newNode->elems.insert(newNodeId);
            newNode->next = tempnode;
        }
        head->size++;
        return;
    }
    bool insertNew = false;
    if(!tempnode){
        tempnode = prevnode;
        if(tempnode->elems.size()<MAX_ELEMENTS_IN_A_NODE){
            tempnode->elems.insert(newNodeId);
        }else{
            insertNew = true;
        }
    }else{
        if(prevnode->elems.size()<MAX_ELEMENTS_IN_A_NODE){
            prevnode->elems.insert(newNodeId);
        }else{
            if(tempnode->elems.size()<MAX_ELEMENTS_IN_A_NODE){
                listIt = prevnode->elems.end();
                listIt--;
                if(newNodeId < *(listIt)){
                    long long maxid = *(listIt);
                    prevnode->elems.erase(maxid);
                    prevnode->elems.insert(newNodeId);
                    newNodeId = maxid;
                }
                tempnode->elems.insert(newNodeId);
            }else{
                tempnode = prevnode;
                insertNew = true;
            }
        }
    }
    if(insertNew){
        node *newNode = getNewNode();
        listIt = tempnode->elems.end();
        listIt--;
        if(newNodeId < *(listIt)){
            //cout << head->listid << " | " << newNodeId << "," << *(listIt) << endl;
            long long maxid = *(listIt);
            tempnode->elems.erase(maxid);
            tempnode->elems.insert(newNodeId);
            newNode->elems.insert(maxid);
        }else{
            newNode->elems.insert(newNodeId);
        }
        newNode->next = tempnode->next;
        tempnode->next = newNode;
    }
    head->size++;
}

long long getCommonNodesCount(unrolledList *head1, unrolledList *head2){
    long long count = 0;
    node *ll1node = getNewNode();
    node *ll2node = getNewNode();
    ll1node = head1->next;
    ll2node = head2->next;
    long long n_i, n_j;
    set<long long> :: iterator it_i, it_j, it;
    if(ll1node) it_i = ll1node->elems.begin();
    if(ll2node) it_j = ll2node->elems.begin();
    while(ll1node && ll2node){
        totalComparisons++;
        n_i = *(it_i);
        n_j = *(it_j);
        if(n_i < n_j){
            if(ll1node->next && *(ll1node->next->elems.begin()) < n_j ){
                ll1node = ll1node->next;
                it_i = ll1node->elems.begin();
                continue;
            }else{
                it_i++;
            }
        }else if(n_i > n_j){
            if(ll2node->next && n_i > *(ll2node->next->elems.begin())  ){
                ll2node = ll2node->next;
                it_j = ll2node->elems.begin();
                continue;
            }else{
                it_j++;
            }
        }else{
            count++;
            it_i++;
            it_j++;
        }
        if(it_i == ll1node->elems.end()){
            ll1node = ll1node->next;
            if(ll1node) it_i = ll1node->elems.begin();
        }
        if(it_j == ll2node->elems.end()){
            ll2node = ll2node->next;
            if(ll2node) it_j = ll2node->elems.begin();
        }
    }
    return count;
}

void deleteUL(unrolledList *head){
    if(!head) return;
    node *prevnode = NULL;
    node *tempnode = getNewNode();
    tempnode = head->next;
    delete head;
    while(tempnode){
        prevnode = tempnode;
        tempnode = tempnode->next;
        delete prevnode;
    }
}

map<long long, unrolledList*> nodeids;
map<long long, unrolledList*> :: iterator it;
map<pair <long, long>, double> storedSimilarityHashTable;


int main (int argc, char const *argv[]){
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
    unrolledList *head1, *head2;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        if(nodeids.count(ni)==0){
            head1 = getNewHeadNode();
            head1->listid = ni;
            nodeids[ni] = head1;
            addNode(head1, ni); // we include this node itself
        }else head1 = nodeids[ni];
        if(nodeids.count(nj)==0){
            head2 = getNewHeadNode();
            head2->listid = nj;
            nodeids[nj] = head2;
            addNode(head2, nj); // we include this node itself
        }else head2 = nodeids[nj];
        addNode(head1, nj);
        addNode(head2, ni);
    }
    inFile.close();
    //printList(nodeids[17]);
    //printList(nodeids[18]);
    //getCommonNodesCount(nodeids[17], nodeids[18]);
    //return 0;


    // end load edgelist

    // iterate through the map of nodeIds

    // do the gosh darn calculation, fool!
    totalComparisons = 0;
    FILE * jaccFile = fopen(argv[2],"w");
    set<long long> :: iterator it_i, it_j;
    long long keystone, len_int;
    unrolledList *head, *head_i, *head_j;
    long long n_i, n_j;
    node *head_i_node, *head_j_node;
    head = getNewHeadNode();
    head_i = getNewHeadNode(); head_j = getNewHeadNode();
    double curr_jacc;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        keystone = (*it).first;
        head = (*it).second;
        head_i_node = head->next;
        while(head_i_node) { // neighbors of keystone in the unrolled linkedlist
            for(it_i=head_i_node->elems.begin(); it_i!=head_i_node->elems.end();it_i++){
                n_i = *(it_i);
                if (n_i == keystone){
                    continue;
                }
                head_i = nodeids[n_i];
                head_j_node = head_i_node;
                it_j = it_i;
                while(head_j_node){
                    for(;it_j!=head_j_node->elems.end();it_j++){
                        n_j = *(it_j);
                        if (n_j == keystone or n_i >= n_j){
                            continue;
                        }
                        head_j = nodeids[n_j];
                        if(storedSimilarityHashTable.count(make_pair(n_i, n_j))){
                            curr_jacc = storedSimilarityHashTable[make_pair(n_i, n_j)];
                        }else{
                            len_int = getCommonNodesCount(head_i, head_j);    // my set intersection function
                            //cout << len_int << ", ";
                            curr_jacc = (double) len_int / (double)( head_i->size  + head_j->size - len_int);
                            storedSimilarityHashTable[make_pair(n_i, n_j)] = curr_jacc;
                        }

                        //cout << n_i << " " << n_j << " = " << curr_jacc <<" | ";
                        if (keystone < n_i && keystone < n_j){
                            fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i, keystone, n_j, curr_jacc );
                        } else if (keystone < n_i && keystone > n_j){
                            fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i, n_j, keystone, curr_jacc );
                        } else if (keystone > n_i && keystone < n_j){
                            fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i, keystone, keystone, n_j, curr_jacc );
                        } else {
                            fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i, keystone, n_j, keystone, curr_jacc );
                        }
                    }
                    head_j_node = head_j_node->next;
                    if(head_j_node) it_j = head_j_node->elems.begin();
                }
            }
            head_i_node = head_i_node->next;
        }
    } // done loop over keystones
    fclose(jaccFile);
    cout << "Total comparisons are: " << totalComparisons << endl;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        head = (*it).second;
        deleteUL(head);
    }
    return 0;
}
