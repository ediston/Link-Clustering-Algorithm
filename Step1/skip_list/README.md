Skip List<br>
Why we doing this?<br>
1. To check if it impacts complexity (space and time)<br>
2. Will help in deciding hardware architecture<br>

Implementation Details:<br>
1. SkipList.h contains the skip list implementation<br>
2. calcSimilaritySL.cpp is the dirving file<br>

 
Linked List Implementation<br>
1.Create map< pair < long,long > , double> storedSimilarityHashTable<br>
2.Create a map < long long, node* > nodeids <br>
3.First linked list node structure.<br>
  struct node{<br>
    long long nodeid;<br>
    node* next;<br>
  };<br>

4.Implementation of sub steps<br>
Step1.1<br>
Here we will combine Step1.1 and Step1.2 of the original implementation<br>
Method: Loop from step 1 to 5 till the end of input file<br>
  1. Read an edge from the file, E= (n_i n_j)<br>
  2. Check if this nodeid has been added before, maintain nodeids<br>
  3. If not added then create a new linked list head and add it to nodeids map.<br>
  4. Else add n_i to Linkedlist of n_j and add n_j to Linkedlist of n_i<br>
  5. All additions will be done keeping the linked list in sorted order<br>
Complexity: O(E*N)<br>

Step  1.2, Covered in Step1.1<br>

Step 1.3 Iterate through map nodeids, consider each key as the keystone(n_k)node and then for each node(n_i) present in linkedlist of (n_k), select another node (n_j) such that (n_i!=n_k) and (n_j!=n_k) and (n_i  < n_j).<br>
Method: We iterate through the neighbors of n_k. For each neighbor we again iterate the neighbors of n_k. As the nodeids are added in increasing order. For each node n_i we start from n_j=n_k->next. <br>
Complexity: O(n^2)<br>

Step 1.4 For the two given nodes(n_i, n_j) and the keystone node count the common neighbors for the two nodes<br>
Method: We iterate through the neighbors of the two nodes.<br>
Method: 
  1. For the two nodes(n_i, n_j) check if we have in the storedSimilarityHashTable.<br>
  2. If not then iterate through the neighbors of the two nodes to get the count of common neighbors and go to step 1.5<br>
  3. Go to step 1.6<br>
   
Complexity: O(len(Adj(n_i)) + len(Adj(n_j)))<br>

Step 1.5 Calculate the similarity measure.<br>
Method: Let’s say the count of common neighbors is Ccn. Then similarity is:
		Ccn/(Cn_i + Cn_j - Ccn)<br>
Complexity: O(1)<br>

Step 1.6 Write the two links and the similarity measure to the file<br>
Method: Writing to a file is linear?<br>
Complexity: O(1)<br>

Overall Complexity: O(n^3)<br>
