#include <bits/stdc++.h>
#include <ctime>
using namespace std;
#define MAX_HIEGHT 32
#define ONEIN 2
#define INFINITE 99999999

//long long totalComparisons;

int getRandom(){
     return rand() % ONEIN;
}

struct node{
    long long nodeId;
    node *next;
    node *below;
    node *up;
};

struct skipList{
    int height;
    long long listId;
    long long length;
    node **head;
};

node *getNewNode(long long nodeId){
    /*
    Node.
    height will be decided randomally from 1 to MAX_HIEGHT
    */
    node * newnode = new node; 
    newnode->nodeId = nodeId;
    newnode->next = NULL;
    newnode->below = NULL;
    newnode->up = NULL;
    return newnode;
}

skipList *getNewSkipList(){
    /*
    Head node. Initially all levels of the headnode will point to Null
    height will be equal to maximum possible height
    */
    skipList * newSkipList = new skipList;
    newSkipList->height = 0;
    newSkipList->head = new node*[MAX_HIEGHT];
    for(int i=0; i<MAX_HIEGHT; i++){
        newSkipList->head[i] = getNewNode(-INFINITE);
        if(i>0){ 
            newSkipList->head[i]->below = newSkipList->head[i-1];
            newSkipList->head[i-1]->up = newSkipList->head[i];
        }
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
            newNode->up = prev;
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
            while(n2->up) n2 = n2->up;
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

map<long long, skipList*> nodeids;
map<long long, skipList*> :: iterator it;
map<pair <long, long>, double> storedSimilarityHashTable;


int main (int argc, char const *argv[]){
    clock_t begin = clock();

     srand (time(NULL));
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
    long long maxn=0;
    long long ni,nj;
    skipList *s1, *s2;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        //cout << ni << " " << nj << endl;
        if(nodeids.count(ni)==0){
            s1 = getNewSkipList();
            s1->listId = ni;
            nodeids[ni] = s1;
            addNode(s1, ni); // we include this node itself
        }else s1 = nodeids[ni];
        if(nodeids.count(nj)==0){
            s2 = getNewSkipList();
            s2->listId = nj;
            nodeids[nj] = s2;
            addNode(s2, nj); // we include this node itself
        }else s2 = nodeids[nj];
        addNode(s1, nj);
        addNode(s2, ni);
        maxn = max(maxn, max(ni, nj));
    }
    inFile.close();
    // end load edgelist
    //printSkipList(nodeids[17]);
    //printSkipList(nodeids[18]);
    //getCommonNodesCount(nodeids[17], nodeids[18]);
   // return 0;
    // iterate through the map of nodeIds

    // do the gosh darn calculation, fool!
    ///totalComparisons = 0;
    FILE * jaccFile = fopen(argv[2],"w");
    long long keystone, len_int;
    skipList *s, *s_i, *s_j;
    node *n_i, *n_j;
    s = getNewSkipList(); n_i = getNewNode(0); n_j = getNewNode(0);
    s_i = getNewSkipList(); s_j = getNewSkipList();
    double curr_jacc;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        keystone = (*it).first;
        s = (*it).second;
        n_i = s->head[0]->next;
        while(n_i && n_i->next) { // neighbors of keystone in the linkedlist
            if (n_i->nodeId == keystone){
                //cout << "n_i->nodeId == keystone" << endl;
                n_i = n_i->next;
                continue;
            }
            s_i = nodeids[n_i->nodeId];
            n_j = n_i->next;
            while(n_j) { // next neighbors of n_k
                if (n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId){
                    //out << "n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId" << endl;
                    n_j = n_j->next;
                    continue;
                }
                s_j = nodeids[n_j->nodeId];
                if(storedSimilarityHashTable.count(make_pair(n_i->nodeId, n_j->nodeId))){
                    curr_jacc = storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)];
                }else{
                    len_int = getCommonNodesCount(s_i, s_j);    // my set intersection function
                    curr_jacc = (double) len_int / (double)( s_i->length  + s_j->length - len_int);
                    storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)] = curr_jacc;
                }
                if (keystone < n_i->nodeId && keystone < n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i->nodeId, keystone, n_j->nodeId, curr_jacc );
                } else if (keystone < n_i->nodeId && keystone > n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i->nodeId, n_j->nodeId, keystone, curr_jacc );
                } else if (keystone > n_i->nodeId && keystone < n_j->nodeId){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i->nodeId, keystone, keystone, n_j->nodeId, curr_jacc );
                } else {
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i->nodeId, keystone, n_j->nodeId, keystone, curr_jacc );
                }
                n_j = n_j->next;
            }
            n_i = n_i->next;
        }
    } // done loop over keystones
    fclose(jaccFile);
    //cout << "Total comparisons are: " << totalComparisons << endl;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        s = (*it).second;
        deleteLL(s);
    }
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}

