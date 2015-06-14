#include <bits/stdc++.h>
#include    <ctime>
using namespace std;
#define MAX_HIEGHT 32
#define ONEIN 2

long long totalComparisons;

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
    long long listId;
    long long size;
    node **next;
};

node *getNewNode(int height){
    /*
    Node.
    height will be decided randomally from 1 to MAX_HIEGHT
    */
    node * newnode = new node;
    newnode->height = height;
    newnode->nodeId = 0;
    newnode->next = new node*[height];
    for(int i=0; i<height; i++)
        newnode->next[i] = NULL;
    return newnode;
}

skipList *getNewSkipList(){
    /*
    Head node. Initially all levels of the headnode will point to Null
    height will be equal to maximum possible height
    */
    skipList * newSkipList = new skipList;
    newSkipList->height = 0;
    newSkipList->next = new node*[MAX_HIEGHT];
    for(int i=0; i<MAX_HIEGHT; i++)
        newSkipList->next[i] = NULL;
    newSkipList->size = 0;
    return newSkipList;
}

void printSkipList(skipList *s){
    if(!s) return;
    cout << "Skiplist #" << s->listId << endl;
    node *current = getNewNode(0);
    for(int lvl = s->height-1; lvl>= 0; lvl-- ){
        cout << "Height " << lvl << ": ";
        current = s->next[lvl];
        while(current){
            cout  << " - " << current->nodeId << "(" << current->height << ")";
            current = current->next[lvl];
        }
        cout << endl;
    }
    cout << endl;
}

void addNode(skipList *s, long long newNodeId){
    // first we decide the height of the new node
    // height = level+1
    int level = 0;

	for(;getRandom() >= 1 ; level++) {
        if(level+1 > s->height || level==MAX_HIEGHT-1) {
            break;
        }
	}
	if(level+1 > s->height)  s->height = level+1;

	node *prev = getNewNode(0);
    node *current = getNewNode(0);
    node *newNode = getNewNode(level+1);
    prev = NULL;
    newNode->nodeId = newNodeId;
    current = NULL;
    /// for each level starting from max height, we wanna reach the
    /// node with nodeid > newNodeId
    /// and then we insert the new node there
    for(int lvl = s->height-1; lvl>= 0; lvl-- ){
        prev = NULL;
        /// take the first node at this lvl of skip list
        for(current = s->next[lvl]; current&&current->next[lvl]; current = current->next[lvl]){
			if(current->nodeId > newNodeId){
				break;
			}
			prev = current;
		}
		if(lvl > level ){
            //if the level is greater than the new node's level move on
			continue;
		}
        if(current == NULL){
            /// node at this lvl of skip list is empty
            s->next[lvl] = newNode;
        }else{
            /// We found at least one node which is current
            if(current->nodeId > newNode->nodeId){
                if(prev==NULL){
                /// this means we got to add it at the front of this lvl of the skip list
                    s->next[lvl] = newNode;
                }else{
                    prev->next[lvl] = newNode;
                }
                newNode->next[lvl] = current;
            }else{
                /// so add it at the end
                newNode->next[lvl] = current->next[lvl];
                current->next[lvl] = newNode;
            }
        }
	}
	// increase the size of the skip list
    s->size++;
}

long long getCommonNodesCount(skipList *s1, skipList *s2){
    long long count = 0;
    int lvl1, lvl2= 0;
    lvl1 = 0;
    if(s2->size > s1->size){
        swap(s1, s2);
    }
    node *nextn1 = getNewNode(0);
    node *n1 = getNewNode(0);
    node *n2 = getNewNode(0);
    n1 = s1->next[lvl1];
    n2 = s2->next[lvl2];

    while(n1 && n2){
        // first let's check if n1 node is bigger than n2
        //printf("%lld(%d), %lld(%d)\n",  n1->nodeId, lvl1,  n2->nodeId, lvl2);
        //cout <<  n1->nodeId << "," <<  n2->nodeId << endl;
        totalComparisons++;
        if(n1->nodeId < n2->nodeId){
            // if yes get the next node
            nextn1 = n1->next[lvl1];
            // if there is no next_node  or next_nodeis bigger and lvl1 is not 0
            // decrease the level
            if((!nextn1 || nextn1->nodeId > n2->nodeId) && lvl1>0){
                lvl1--;
            }else{
                // else we have level ==0 or nextn1 nodeid < n2 node id
                n1= nextn1;
                if(n1)lvl1 = n1->height-1;
            }
        }else
        {
            // if nodeids are equal then increase count
            if(n1->nodeId == n2->nodeId)
                count++;
            // go to the next node in the second skip list
            n2 = n2->next[lvl2];
            // go to the highest  level of skiplist 1
            // this will reduce the number of comparisons
            lvl1 = n1->height-1;
        }
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
    totalComparisons = 0;
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
        n_i = s->next[0];
        while(n_i && n_i->next[0]) { // neighbors of keystone in the linkedlist
            if (n_i->nodeId == keystone){
                //cout << "n_i->nodeId == keystone" << endl;
                n_i = n_i->next[0];
                continue;
            }
            s_i = nodeids[n_i->nodeId];
            n_j = n_i->next[0];
            while(n_j) { // next neighbors of n_k
                if (n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId){
                    //out << "n_j->nodeId == keystone or n_i->nodeId >= n_j->nodeId" << endl;
                    n_j = n_j->next[0];
                    continue;
                }
                s_j = nodeids[n_j->nodeId];
                if(storedSimilarityHashTable.count(make_pair(n_i->nodeId, n_j->nodeId))){
                    curr_jacc = storedSimilarityHashTable[make_pair(n_i->nodeId, n_j->nodeId)];
                }else{
                    len_int = getCommonNodesCount(s_i, s_j);    // my set intersection function
                    curr_jacc = (double) len_int / (double)( s_i->size  + s_j->size - len_int);
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
                n_j = n_j->next[0];
            }
            n_i = n_i->next[0];
        }
    } // done loop over keystones
    fclose(jaccFile);
    cout << "Total comparisons are: " << totalComparisons << endl;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        s = (*it).second;
        deleteLL(s);
    }
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}
