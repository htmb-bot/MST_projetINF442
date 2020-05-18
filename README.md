# MST_projetINF442

MST use in Data Analysis, May 2020

The objective of this project is to understand and implement sequential and parallel algorithms for minimum-spanning-tree (MST) search in a graph, in order to be able to use efficient MST-Clustering methods and test it on public datasets.

# Task 1: 

Implementation of PRIM and BORUKVA -sequential algorithms for MST search- in file test-graph.cpp

# Task 2: 

Implementation of KRUSKAL -sequential algorithm for MST search- in file test-graph.cpp, requires class edge, class graph

*note: for parallel algorithm, we use the MPI (Message Passing Interface) standard for C/C++

# Task 3: 

Implementation of PRIM's algorithm adapted for parallel computing. in file parallel-prim.cpp 

# Task 4: 

Implementation of GHS (Gallager-Humblet-Spira) algorithm for MST construction in asynchronous Message-passing model. in file ghs_fromscratch.cpp, requires class ghs_edge

# Task 5: 

Implementation of an MST-based k-clustering strategy. in file test-graph.cpp, requires class edge, class graph, class point, class cloud

# Task 6: 

Implementation of an MST-based k-clustering strategy, where k is unknown (using the silhouette method). in file test-graph.cpp, requires class edge, class graph, class point, class cloud

*Tests for tasks 1-4: Testing results and efficiency on randomly generated network-simulating graphs using the ASHIIP software, and custom graph GraphMatrix2.txt for gallager
*Tests for tasks 5-6: Testing results and efficiency on public datasets such as iris.csv and cereals.csv

# Testing tasks 1-2:

First generate a graph using $java -jar ashiip_gui.jar You will need to enter graph size and other desired properties.

Compile using $make test-graph.

Then convert the graph to a readable format using our friend javiertheconverter.cpp $./javiertheconverter < Graphexample.txt

Then feel free to test the program on test-graph.cpp $./test-graph < example.txt

# Testing task 3

Use $make parallel_prim to compile.
Execute using $mpirun -n p ./parallel_prim where p is the desired number of processors. A graph is randomly generated. You can change maxWeight and V (number of vertices in the file parallel_prim.cpp). If you want to test on a custom imput, make sure it is of the following adjacency matrix .txt format:

3 \n
INF 1 2 \n
1 INF INF \n
2 INF INF \n

# Testing task 4

*note: at this point in time, the program freezes after a certain amount of time. Lines in cyan are confirmed MST branches. The execution on our example graph stops before it finds the last MST branch (6--8).

Use $make gallager to compile.

Execute using $mpirun -np p gallager < Input.txt where p is the number of vertices in the graph. Input must be in the ajacency matrix format. You can use our custom graph GraphMatrix2.txt, with 9 vertices. Be careful that all edge weights be distinct, otherwise fragmentIDs may not be unique. 

On our school computers, this would be
$mpirun -np 9 -host perone:3,radius:3,metacarpe:3 gallager < GraphMatrix2.txt

# Testing tasks 5-6

Once test-graph.cpp compiled, you can test the clustering method with $./test-graph < input.txt  (input.txt here is just to launch the executable). It will create a cluster out of the .csv file mentionned in the main function, after ESSAI CLUSTERING. Then it will determine the optimal k number of clusters for the dataset, thanks to the silhouette evaluation.

Without changing the test-graph.cpp, $./test-graph < input.txt apply the methods on the iris.csv dataset. To change the number of clusters, change the nb_clusters integer in the main function.

You can try it on cereales.csv by uncommenting lines 658-662 and commenting lines 664-668.

You can feel free to test the clustering method on test-graph.cpp on other csv files. However, provide the file name csv_filename, the desired number of cluster nb_cluster, the point space dimension nb_dim (WITHOUT THE POINTS NAMES), the number of points nb_samples (without the first line containing the names of coordinates), and the name column index colonne_noms (if it is the first column, enter 0).
If you decide to change csv file, the file must respect one space between point coordinates at each line.


  

