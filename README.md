# MST_projetINF442

MST use in Data Analysis, May 2020

The objective of this project is to understand and implement sequential and parallel algorithms for minimum-spanning-tree (MST) search in a graph, in order to be able to use efficient MST-Clustering methods and test it on public datasets.

# Task 1: 

Implementation of PRIM and BORUKVA -sequential algorithms for MST search- in file test-graph.cpp

# Task 2: 

Implementation of KRUSKAL -sequential algorithm for MST search- in file test-graph.cpp

*note: for parallel algorithm, we use the MPI (Message Passing Interface) standard for C/C++

# Task 3: 

Implementation of PRIM's algorithm adapted for parallel computing. in file parallel-prim.cpp 

# Task 4: 

Implementation of GHS (Gallager-Humblet-Spira) algorithm for MST construction in asynchronous Message-passing model. in file ghs_fromscratch.cpp

# Task 5: 

Implementation of an MST-based k-clustering strategy. in file test-graph.cpp

# Task 6: 

Implementation of an MST-based k-clustering strategy, where k is unknown (using the silhouette method). in file test-graph.cpp

*Tests for tasks 1-4: Testing results and efficiency on randomly generated network-simulating graphs using the ASHIIP software
*Tests for tasks 5-6: Testing results and efficiency on public datasets such as iris.csv

# Small guide on how to test tasks 1-2:

First generate a graph using $java -jar ashiip_gui.jar You will need to enter graph size and other desired properties.

Then convert the graph to a readable format using our friend javiertheconverter.cpp $./javiertheconverter < Graphexample.txt

Then feel free to test the program on test-graph.cpp $./test-graph < example.txt

#Small guide on how to test tasks 5-6

Once test-graph.cpp compiled, you can test the clustering method with $./test-graph < input.txt  (input.txt here is just to launch the executable). It will create a cluster out of the .csv file mentionned in the main function, after ESSAI CLUSTERING. Then it will determine the optimal k number of clusters for the dataset, thanks to the silhouette evaluation.

Without changing the test-graph.cpp, $./test-graph < input.txt apply the methods on the iris.csv dataset. To change the number of clusters, change the nb_clusters integer in the main function.

You can try it on cereales.csv by uncommenting lines 658-662 and commenting lines 664-668.

You can feel free to test the clustering method on test-graph.cpp on other csv files. However, provide the file name csv_filename, the desired number of cluster nb_cluster, the point space dimension nb_dim (WITHOUT THE POINTS NAMES), the number of points nb_samples (without the first line containing the names of coordinates), and the name column index colonne_noms (if it is the first column, enter 0).
If you decide to change csv file, the file must respect one space between point coordinates at each line.


  

