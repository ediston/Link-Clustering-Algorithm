#include <bits/stdc++.h>
using namespace std;
long long totalComparisons;
long long commonNodeCount;
struct node{
    long long nodeId;
    node *left;
    node *right;
};

struct bst{
    long long size;
    node *root;
};

node *getNewNode(){
  node * newnode = new node;
  newnode->left = NULL;
  newnode->right = NULL;
  newnode->nodeId = 0;
  return newnode;
}

bst *getNewBST(){
  bst * newnode = new bst;
  newnode->root = NULL;
  newnode->size = 0;
  return newnode;
}

void printTree(node *root){
    if(!root) return;
    printTree(root->left);
    cout << root->nodeId << " ";
    printTree(root->right);
}

void printBST(bst *head){
    cout << "Size:" << head->size << endl;
    printTree(head->root);
    cout << endl;
}

void addNode(node *root, long long newNodeId){
    if(root->nodeId > newNodeId){
        if(root->left){
            addNode(root->left, newNodeId);
        }else{
            node*newNode = getNewNode();
            newNode->nodeId = newNodeId;
            root->left = newNode;
        }
    }else{
        if(root->right){
            addNode(root->right, newNodeId);
        }else{
            node*newNode = getNewNode();
            newNode->nodeId = newNodeId;
            root->right = newNode;
        }
    }
}

void addNodeToTree(bst *head, long long newNodeId){
    if(head->size ==0 ){
        node*newNode = getNewNode();
        newNode->nodeId = newNodeId;
        head->root = newNode;
        head->size++;
        return;
    }
    addNode(head->root, newNodeId);
    head->size++;
}

bool searchNode(node* root, long long nodeID){
    if(!root) return false;
    totalComparisons++;
    if(root->nodeId == nodeID) return true;
    if(nodeID < root->nodeId)  return searchNode(root->left, nodeID) ;
    else return searchNode(root->right, nodeID);
}

void getCommonNodes(node *root1, node*root2){
    if(!root1 || !root2) return;
    getCommonNodes(root1->left, root2);
    if(searchNode(root2, root1->nodeId))
        commonNodeCount++;
    getCommonNodes(root1->right, root2);
}

long long getCommonNodesCount(bst *head1, bst *head2){
    commonNodeCount = 0;
    node *root1 = getNewNode();
    node *root2 = getNewNode();
    root1 = head1->root;
    root2 = head2->root;
    //if(head1->size > head2->size) swap(root1, root2);
    getCommonNodes(root1, root2);
    return commonNodeCount;
}

void deleteTree(node *root){
    if(!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void deleteBST(bst *head){
    node *root = head->root;
    deleteTree(root);
    delete head;
}

map<long long, bst*> bsts;
map<long long, bst*> :: iterator it;
map<pair <long, long>, double> storedSimilarityHashTable;
FILE * jaccFile;
bst *head, *head_i, *head_j;
long long keystone, len_int, n_i, n_j;
double curr_jacc;

void tryNode(node *root, long long prevNodeId){
    if(!root ) return;
    if(root->nodeId >= prevNodeId) tryNode(root->left, prevNodeId);
    if(root->nodeId != keystone && prevNodeId != keystone && prevNodeId < root->nodeId){
        n_j = root->nodeId; n_i = prevNodeId;
        if(storedSimilarityHashTable.count(make_pair(n_i, n_j))){
            curr_jacc = storedSimilarityHashTable[make_pair(n_i, n_j)];
        }else{
            head_i = bsts[n_i];
            head_j = bsts[n_j];
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
    }
    tryNode(root->right, prevNodeId);
}

void createSimilarityOrder(node *root, long long nodeid){
    if(!root) return;
    createSimilarityOrder(root->left, nodeid);
    tryNode(bsts[nodeid]->root, root->nodeId);
    createSimilarityOrder(root->right, nodeid);
}

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


    // load edgelist into BSTS
    ifstream inFile;
    inFile.open (argv[1]);
    if (!inFile) {
        cerr << "ERROR: unable to open input file" << endl;
        exit(1); // terminate with error
    }
    long long ni,nj;
    bst *head1, *head2;
    while (inFile >> ni >> nj){ // scan edgelist once to get number of edges and nodes, for allocation
        if(bsts.count(ni)==0){
            head1 = getNewBST();
            bsts[ni] = head1;
            addNodeToTree(head1, ni); // we include this node itself
        }else head1 = bsts[ni];
        if(bsts.count(nj)==0){
            head2 = getNewBST();
            bsts[nj] = head2;
            addNodeToTree(head2, nj); // we include this node itself
        }else head2 = bsts[nj];
        addNodeToTree(head1, nj);
        addNodeToTree(head2, ni);
    }
    inFile.close();

    // end load edgelist
    printBST(bsts[17]);
    //return 0;
    // iterate through the map of nodeIds

    // do the gosh darn calculation, fool!
    jaccFile = fopen(argv[2],"w");
    head =  getNewBST();  head_i =  getNewBST(); head_j =  getNewBST();

    for (it = bsts.begin(); it != bsts.end(); it++) {
        keystone = (*it).first;
        head = (*it).second;
        createSimilarityOrder(head->root, keystone);
    }
    fclose(jaccFile);
    cout << "Total comparisons are: " << totalComparisons << endl;
    for (it = bsts.begin(); it != bsts.end(); it++) { // loop over keys
        head = (*it).second;
        deleteBST(head);
    }
    return 0;
}
