<p>Creating graphs in R</p>
install.packages("igraph")
library(igraph)
g <-  erdos.renyi.game(n, m, type=c("gnm"))
//n = vertices, m = edges

write.graph(g, "n_m.pairs", format=c("edgelist"))

