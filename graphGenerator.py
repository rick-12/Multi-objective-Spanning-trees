import networkx as nx
import numpy as np

n = 17   # number of nodes
p = 0.23  # density of edges

G = nx.gnp_random_graph(n, p)

# Open file for writing
with open("./input.txt", "w") as f:
    f.write(str(len(G.nodes())) + " ")
    f.write(str(len(G.edges())) + "\n")

    # Write edges to file
    for edge in G.edges():
        wt = np.random.randint(1, 10, 2)
        f.write(f"{str(edge[0])} {str(edge[1])} {wt[0]} {wt[1]}\n")
    f.flush()

# Print a message indicating that the file was written
print("Output written to input.txt")
