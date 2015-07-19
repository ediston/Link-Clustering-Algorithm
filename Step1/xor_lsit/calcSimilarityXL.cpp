#include <bits/stdc++.h>
#include <cstdint>
#include <ctime>
using namespace std;
//long long totalComparisons;

struct node{
    long long nodeId;
    node *nextPrevXor;
};

struct xorListHeadNode{
    long long size;
    long long listId;
    node *nextPrevXor;
};

/* returns XORed value of the node addresses */
node* XOR (node *a, node *b)
{
    return (node*) ((uintptr_t) (a) ^ (uintptr_t) (b));
}

node *getNewNode(){
    node * newnode = new node;
    newnode->nodeId = 0;
    newnode->nextPrevXor = NULL;
    return newnode;
}

xorListHeadNode *getNewHeadNode(long long id){
    xorListHeadNode * newnode = new xorListHeadNode;
    newnode->nextPrevXor = NULL;
    newnode->size = 0;
    newnode->listId = id;
    return newnode;
}

void printList(xorListHeadNode *head){
    if(!head) return;
    node *currNode = getNewNode();
    node *prevNode = NULL;
    node *next;
    cout << "List #"<< head->listId << endl;
    currNode = head->nextPrevXor;
    while(currNode){
        cout << currNode->nodeId << " ";
        next = XOR(prevNode, currNode->nextPrevXor);
        prevNode = currNode;
        currNode = next;
    }
    cout << endl;
}

void sortList(xorListHeadNode *head){
    if(!head || head->size == 0) return;
    node *currNode = head->nextPrevXor;
    node *prevNode = NULL;
    node *next;
    long long *newNums = new long long [head->size];
    currNode = head->nextPrevXor;
    long long i=0;
    while(currNode){

        newNums[i] = currNode->nodeId;
        next = XOR(prevNode, currNode->nextPrevXor);
        prevNode = currNode;
        currNode = next;
        i++;
    }
    sort(newNums, newNums + head->size);
    currNode = head->nextPrevXor;
    prevNode = NULL;
    i=0;
    while(currNode){
        currNode->nodeId = newNums[i];
        next = XOR(prevNode, currNode->nextPrevXor);
        prevNode = currNode;
        currNode = next;
        i++;
    }
}

/* Insert a node at the begining of the XORed linked list and make the
   newly inserted node as head->next*/
void addNode(xorListHeadNode *head, long long newNodeId){
    if(!head) return;
    // create a new node
    node *newNode = getNewNode();
    if(head->size==0){
        newNode->nodeId = newNodeId;
        head->nextPrevXor = newNode;
        head->size++;
        return ;
    }
    node *headnode = head->nextPrevXor;
    /// if this not the first node then
    /// next of this node is the current headnode
    newNode->nextPrevXor = headnode;
    newNode->nodeId = newNodeId;
    /// head->nextPrevXor is XOR of NULL and next. So if we do XOR of
    /// it with NULL, we get next
    ///node* next = XOR( (headnode->nextPrevXor),  NULL);
    ///headnode->npx = XOR(new_node, next);
    headnode->nextPrevXor = XOR(newNode, headnode->nextPrevXor);

    /// header points to a new node
    head->nextPrevXor = newNode;
    head->size++;

    /*
    If we have A->next = B, C->next = B
    and B->next = Xor(A,C)
    then we can create C from Xor(A,B->next) and C = (A, B->next)
    so we need only A to get B,C. Because B = Xor(A, A->next);
    */
}

long long getCommonNodesCount(xorListHeadNode *head1, xorListHeadNode *head2){

    long long count = 0;
    node *currNode1 = getNewNode(); currNode1 = head1->nextPrevXor;
    node *prevNode1 = NULL;
    node *next1;

    node *currNode2 = getNewNode(); currNode2=head2->nextPrevXor;
    node *prevNode2 = NULL;
    node *next2;
    bool move1, move2;
    while(currNode1 && currNode2){
        move1 = false, move2 = false;
        //totalComparisons++;
        //cout << "\t\t"<< currNode1->nodeId << " ? " << currNode1->nodeId << endl;
        if(currNode1->nodeId < currNode2->nodeId){
            move1 = true;
        }else if(currNode2->nodeId < currNode1->nodeId){
            move2 = true;
        }else{

            move1 = true;
            move2 = true;
            count++;
        }
        if(move1){
            next1 = XOR(prevNode1, currNode1->nextPrevXor);
            prevNode1 = currNode1;
            currNode1 = next1;
        }
        if(move2){
            next2 = XOR(prevNode2, currNode2->nextPrevXor);
            prevNode2 = currNode2;
            currNode2 = next2;
        }
    }
    return count;
}

void deleteLL(xorListHeadNode *head){
    node *currNode = head->nextPrevXor;
    node *prevNode = NULL;
    node *next;
    currNode = head->nextPrevXor;
    delete head;
    while(currNode){
        next = XOR(prevNode, currNode->nextPrevXor);
        if(prevNode) delete prevNode;
        prevNode = currNode;
        currNode = next;
    }
}

map<long long, xorListHeadNode*> nodeids;
map<long long, xorListHeadNode*> :: iterator it;
map<pair <long long, long long>, double> storedSimilarityHashTable;


int main (int argc, char const *argv[]){
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
    xorListHeadNode *head1, *head2;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        if(nodeids.count(ni)==0){
            head1 = getNewHeadNode(ni);
            nodeids[ni] = head1;
            addNode(head1, ni); // we include this node itself
        }else head1 = nodeids[ni];
        if(nodeids.count(nj)==0){
            head2 = getNewHeadNode(nj);
            nodeids[nj] = head2;
            addNode(head2, nj); // we include this node itself
        }else head2 = nodeids[nj];
        addNode(head1, nj);
        addNode(head2, ni);
    }
    inFile.close();

    // end load edgelist



    node *currNode_i = getNewNode();
    node *prevNode_i = getNewNode();
    node *next_i = getNewNode();

    node *currNode_j = getNewNode();
    node *prevNode_j = getNewNode();
    node *next_j = getNewNode();

    FILE * jaccFile = fopen(argv[2],"w");
    long long keystone, len_int;
    xorListHeadNode *head, *head_i, *head_j;
    long long n_i, n_j;
    head = getNewHeadNode(0);
    head_i = getNewHeadNode(0); head_j = getNewHeadNode(0);

    // iterate through the map of nodeIds

    for (it = nodeids.begin(); it != nodeids.end(); it++){
        head = (*it).second;
        sortList(head);
    }

    double curr_jacc;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        keystone = (*it).first;
        head = (*it).second;
        //printList(head);
        currNode_i = head->nextPrevXor;
        prevNode_i = NULL;
        while(currNode_i && XOR(prevNode_i, currNode_i->nextPrevXor)) { // neighbors of keystone in the linkedlist
            if (currNode_i->nodeId == keystone){
                next_i = XOR(prevNode_i, currNode_i->nextPrevXor);
                prevNode_i = currNode_i;
                currNode_i = next_i;
                continue;
            }
            n_i = currNode_i->nodeId;

            //cout << n_i << " = n_i" << endl;
            head_i = nodeids[n_i];
            currNode_j = XOR(prevNode_i, currNode_i->nextPrevXor);
            prevNode_j = currNode_i;
            while(currNode_j) { // next neighbors of n_k
                n_j = currNode_j->nodeId;
                if (n_j == keystone or n_i >= n_j){
                    next_j = XOR(prevNode_j, currNode_j->nextPrevXor);
                    prevNode_j = currNode_j;
                    currNode_j = next_j;
                    continue;
                }
                //cout << "\t" << n_j << " = n_j" << endl;
                head_j = nodeids[n_j];
                if(storedSimilarityHashTable.count(make_pair(n_i, n_j))){
                    curr_jacc = storedSimilarityHashTable[make_pair(n_i, n_j)];
                }else{
                    len_int = getCommonNodesCount(head_i, head_j);
                    curr_jacc = (double) len_int / (double)( head_i->size  + head_j->size - len_int);
                    storedSimilarityHashTable[make_pair(n_i, n_j)] = curr_jacc;
                }
                if (keystone < n_i && keystone < n_j){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i, keystone, n_j, curr_jacc );
                } else if (keystone < n_i && keystone > n_j){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", keystone, n_i, n_j, keystone, curr_jacc );
                } else if (keystone > n_i && keystone < n_j){
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i, keystone, keystone, n_j, curr_jacc );
                } else {
                    fprintf( jaccFile, "%lld\t%lld\t%lld\t%lld\t%.6f\n", n_i, keystone, n_j, keystone, curr_jacc );
                }
                next_j = XOR(prevNode_j, currNode_j->nextPrevXor);
                prevNode_j = currNode_j;
                currNode_j = next_j;
            }
            next_i = XOR(prevNode_i, currNode_i->nextPrevXor);
            prevNode_i = currNode_i;
            currNode_i = next_i;
        }
    } // done loop over keystones
    fclose(jaccFile);
    //cout << "Total comparisons are: " << totalComparisons << endl;
    for (it = nodeids.begin(); it != nodeids.end(); it++) { // loop over keys
        head = (*it).second;
        deleteLL(head);
    }
    cout << "Time taken = " << double(clock() - begin)/ CLOCKS_PER_SEC << " seconds. "<< endl;
    return 0;
}
