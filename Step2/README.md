<h1>Create an edge_id file and replace edges with edge ids in jaccs file</h1>
<h2>Why?</h2>
1. We have smaller run-time memory.</br>
2. Also smaller storage is required</br></br>

<h2>How?</h2>
Steps:</br>
1. Go through the .jaccs file</br>
2. Read in a new line, edge edge jacc</br>
3. For a new edge, increment the index and store mapping in a map</br>
4. For an exisiting edge, use the stored mapped index</br>
5. Replace line by id id jacc</br>

 

