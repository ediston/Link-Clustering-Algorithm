Linked List

Why we doing this?<br>
1. To check if it impacts complexity (space and time)

2. Will help in deciding hardware architecture 

Implementation Details:

1. LinkedList.h and LinkedList.cpp contains the linked list implementation

2. calcSimilarity.cpp is the dirving file


Linked List Implementation

1.First linked list node structure.

struct node{

  long long nodeid;
  
  node* next;
  
};


2.Implementation of sub steps

Step1.1

Here we will combine Step1.1 and Step1.2 of the original implementation

Method: Loop from step 1 to 5 till the end of input file

  1. Read an edge from the file, E= (n_i n_j)
  
  2. Check if this nodeid has been added before, maintain a map<long long, node* > nodeids;
  
  3. If not added then create a new linked list head and add it to nodeids map.
 
  4. Else add n_i to Linkedlist of n_j and add n_j to Linkedlist of n_i
  
  5. All additions will be done keeping the linked list in sorted order
  
Complexity: O(E*N)


Step  1.2, Covered in Step1.1

Step 1.3 Iterate through map nodeids, consider each key as the keystone(n_k)node and then for each node(n_i) present in linkedlist of (n_k), select another node (n_j) such that (n_i!=n_k) and (n_j!=n_k) and (n_i  < n_j).

Method: We iterate through the neighbors of n_k. For each neighbor we again iterate the neighbors of n_k. As the nodeids are added in increasing order. For each node n_i we start from n_j=n_k->next 

Complexity: O(n^2)

Step 1.4 For the two given nodes(n_i, n_j) and the keystone node count the common neighbors for the two nodes

Method: We iterate through the neighbors of the two nodes.

Complexity: O(len(Adj(n_i)) + len(Adj(n_j)))

Step 1.5 Calculate the similarity measure.
Method: Let’s say the count of common neighbors is Ccn. Then similarity is:
		Ccn/(Cn_i + Cn_j - Ccn)
Complexity: O(1)

Step 1.6 Write the two links and the similarity measure to the file
Method: Writing to a file is linear?
Complexity: O(1)

Overall Complexity: O(n^3)