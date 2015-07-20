<h1>How to create ground truth?<h2>
<p>
Take all the pageds on a give disambigous pages. they all should belong to a different category?
</p>
<p>Creating graphs in R</p>
install.packages("igraph")
library(igraph)
g <-  erdos.renyi.game(n, m, type=c("gnm"))
//n = vertices, m = edges

write.graph(g, "n_m.pairs", format=c("edgelist"))

