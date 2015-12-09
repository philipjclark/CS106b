/*
 * Authors: Philip Clark and Susannah Neighbor
 * Date: 12/2/15
 * Description: The trailblazer.cpp file is the file in which all of our code is executed for
 * the base line program. The program consists of four main methods: DFS, BFS, and Dijikstra's algorithim helper
 * (which implements both Dijisktra and A*), and kruskal. The first three serve as four path finding
 * algorithims and kruskal creates a randomly generated maze by finding a graph's minimum spanning tree.
 */

#include "trailblazer.h"
#include "basicgraph.h"
#include "set.h"
#include "vector.h"
#include "Color.h"
#include "queue.h"
#include "pqueue.h"

using namespace std;
#include "stack.h"

/*
 * THe depthFirstSearchHelper method is a helper function for DFS that recursively uses explores
 * down all paths until it finds a solution.
 * @graph: the graph being searched with DFS
 * @start: the starting vertex for the path
 * @end: the ending vertex for the path
 * @path: the vector of vertex pointers used to store the path currently being explored
 */
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    path.add(start); //adds the new step (choose)
    start->setColor(GREEN);
    start->visited = true;
    if(start == end) return true; //if you're new step leads to the endpoint you've found a path

    for(Vertex* neighbor:graph.getNeighbors(start)) { //otherwise explore all possible paths from that point, one at a time
        if(!(neighbor->visited) && depthFirstSearchHelper(graph, neighbor, end, path)) return true; //recursively explores unexplored paths
    }
    path.get(path.size()-1)->setColor(GRAY); //by this point the new step can't lead anywhere productive so we set it gray and remove
    path.remove(path.size()-1);
    return false;
}

/*
 * The depthFirstSearch method calls the recursive depthFirstSearchHelper to explore down a
 * selected path until that path is found to lead to a solution or to a "dead end" (ie no viable neighbors).
 * The path is stored as a vector of vertex pointers and returned.
 * @graph: the graph being searched with DFS
 * @start: the starting vertex for the path
 * @end: the ending vertex for the path
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    depthFirstSearchHelper(graph, start, end, path);
    return path;
}

/*
 * Recursive helper method used by BFS, Dijkstra and A* to reconstruct the path through the world using previous pointers
 */
void findPath(Vertex* v, Vector<Vertex*>& path) {
    if(v != NULL) { //if there's still more stuff behind it
        findPath(v->previous, path); //find what else is behind it
        path.add(v); //add the newest thing to the end of the path (works backwards)
    }
}

/*
 * The breadthFirstSearch function uses the BFS algorithim to find the shortest length past
 * (if one exists) from the starting vertex to the ending vertex.
 * @start: the starting vertex for the path
 * @end: the ending vertex for the path
 * @path: the vector of vertex pointers used to store the path currently being explored
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> poss; //queue used to keep track of neighbors one step out from current level of exploration
    poss.add(start);
    start->visited = true;

    while(!poss.isEmpty()) { //as long as there are still possible paths to explore outward
        Vertex* v = poss.dequeue(); //take the next thing in the queue
        v->setColor(GREEN);
        if(v == end) { //if the verte is the ending vertex then we've found a path
           findPath(v, path); //calls recursive helper funcrtion to reconstruct with prev pointers
           return path;
        }
        else {
            for(Vertex* neighbor:graph.getNeighbors(v)) { //otherwise loop through possible neighbors to continue to explore one step outward
                if(!neighbor->visited) { //if it hasn't been visited
                    poss.enqueue(neighbor); //add the viable neighbor to the queue
                    neighbor->setColor(YELLOW);
                    neighbor->visited = true;
                    neighbor->previous = v; //set prev pointer
                }
            }
        }
    }
    return path; //will return an empty path at this point because all options have beene exhausted
}

/*
 * dijkstrasAlgorithmHelper acts to implement both the Dijkstra path finding algorithim as well as the A* path finding algorithim.
 * It uses the bool useHeuristic to decide whether or not to add a heuristic when enqueing in the priority queue (true for
 * A*, false for Dijkstra). The helper function algorithim begins by enqueuing all the vertices to cost infinity, then
 * places the first with cost changed to 0 in a priority queue. It then dequeues from the PQ  and enqueues neighbors according
 * to their cost (and, if useHeuristic is true, to their heuristic value).
 * @graph: the graph being searched with DFS
 * @start: the starting vertex for the path
 * @end: the ending vertex for the path
 * @useHeuristic: whether the heuristic is being used to determine priority (A*) or not (Dijkstra)
 */
Vector<Vertex*> dijkstrasAlgorithmHelper(BasicGraph& graph, Vertex* start, Vertex* end, bool useHeuristic) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> poss; //PQ of steps on graph

    for(Vertex* v: graph.getVertexSet()) {
        v->cost = POSITIVE_INFINITY; //enqueue all to pos infinity
    }
    start->cost = 0; //set start cost to 0
    int startCost = 0;
    if(useHeuristic) startCost = heuristicFunction(start, end); //enqueue at heuristic value if it is A*
    poss.enqueue(start, startCost);

    while(!poss.isEmpty()) { //continue as long as there are viable paths
        Vertex* current = poss.dequeue();
        current->visited = true;
        current->setColor(GREEN);
        if(current == end) { //found the path
            findPath(end, path); //reconstruct using prev pointers
            return path;
        }
        for(Vertex* neighbor: graph.getNeighbors(current)) { //check all neighbors
            if(!neighbor->visited) { //if it hasn't been visited yet
                double cost = current->cost + graph.getEdge(current, neighbor)->cost; //calculate neighbor cost
                if(cost < neighbor->cost) { //if this is a path that better optimizes the cost to the neighbor
                    neighbor->cost = cost;
                    neighbor->previous = current;
                    double h = 0;
                    if(useHeuristic) h = heuristicFunction(neighbor, end); //if A*, include hueristic in PQ
                    if(neighbor->getColor() == YELLOW) poss.changePriority(neighbor,neighbor->cost+h);
                    else poss.enqueue(neighbor, neighbor->cost+h);
                    neighbor->setColor(YELLOW);
                }
            }
        }
    }
    return path; //at this point, an empty path is return because none exists
}

/*
 * The method dijkstrasAlgorithm calls dijkstrasAlgorithmHelper with the heuristic boolean set to false in order to
 * execute a Dijkstra path finding search which minimizes cost.
 * @graph: the graph being searched with DFS
 * @start: the starting vertex for the path
 * @end: the ending vertex for the path
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
return dijkstrasAlgorithmHelper(graph, start, end, false); //simply runs the helper algorithim described above without a heuristic
}

/*
 * The method aStar calls dijkstrasAlgorithmHelper with the heuristic boolean set to true in order to
 * execute a Dijkstra path finding search which minimizes cost and directs the path towards paths more likely to yield results.
 * @graph: the graph being searched with DFS
 * @start: the starting vertex for the path
 * @end: the ending vertex for the path
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    return dijkstrasAlgorithmHelper(graph, start, end, true);//simply runs the helper algorithim described above with a heuristic
}

/*
 * The makeSet method takes a Vertex and modifies its previous and cost fields to create height and parent pointers.
 * The parent pointer (which points to the root's self) will be the representative member of the set, i.e. the reference
 * member to see what set any other element of the set belongs to. The height pointer (a hacking of the cost pointer)
 * is initialized to 0 and keeps track of the height of the disjoint set tree for efficient unioning later on in the
 * Union set method.
 * @root: the root member fo the disjoint set
 */
void makeSet(Vertex* root)
{
    root->previous = root; //have the root point to itslef
    root->cost = 0; //make the height 0
}

/*
 * The findSet function recursively looks through a member of a set until it finds the representative member.
 * @node: the element of the set whose set is being searched for
 */
Vertex* findSet(Vertex* node)
{
    if(node->previous == node) return node;
    else return findSet(node->previous);
}

/*
 * The unionSet method unions two disjoint sets (trees) by having the parent of the "bigger" of the two sets
 * (the one with the greater height) be the parent of the unioned set.
 * @x: the element of one set being unioned
 * @y: the element of the other set being unioned
 */
void unionSet(Vertex* x, Vertex* y) {
    Vertex* xRoot = findSet(x); //find the representative members of the respective sets
    Vertex* yRoot = findSet(y);

    if(xRoot->cost < yRoot->cost) xRoot->previous = yRoot; //if the second set has a greater height make its representative member the total representative member
    else if(xRoot->cost > yRoot->cost) yRoot->previous = xRoot; //if the first set has a greater height make its representative member the total representative member
    else { //if the heights are equal
        yRoot->previous = xRoot; //make the first set's representative member the representative member of the unioned set
        xRoot->cost++; //increment the unioned representative member's height
    }
}

/*
 * The kruskal method is an implementation of Kruskal's minimum spanning tree algorithim via
 * a disjoint-set data structure. The vertex pointer is "hacked" in that the previous pointer
 * becomes a parent pointer (that is, the representative member of a set) and the cost field
 * becomes the height of the tree -- since neither of these fields were used in the algorithim
 * putting them to "peverse" use allowed me to implement a disjoint-set data structure without
 * having to create a new kind of pointer.
 * @graph: the graph whose minimum spanning tree is being found
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst; //the set of edges that make up the minimum spanning tree
    PriorityQueue<Edge*> pq; //uses pq to go through edge weights in cost order
    graph.resetData();

    for(Edge* edge: graph.getEdgeSet()) {
        pq.enqueue(edge, edge->cost); //enqueue the edges according to their cost
    }

    for(Vertex* v: graph.getVertexSet()) { //makes a new disjoint set for each vertex
        makeSet(v);
    }

    while(!pq.isEmpty()) { //continue as long as there is stuff in the PQ
        Edge* current = pq.dequeue();
        if(findSet(current->start) != findSet(current->finish)) { //if the two vertices are not in the same set
            unionSet(current->start, current->finish); //union the two sets
            mst.add(current); //add the edge to the mst
        }
    }
    return mst;
}

/*
 * The isConnected function is a helper function for kruskal. It takes in two vertices and the set of all vertex clusters and sees
 * whether the two vertices exist in the same cluster and, if not, combines the two.
 * @v1: one of the two vertices being checked to see if it is connected to the other (v2)
 * @v2: one of the two vertices being checked to see if it is connected to the other (v1)
 * @vertexClusters: the set of all vertex clusters (that is, groups of vertices that are connected)
 */
bool isConnected(Vertex* v1, Vertex* v2, Vector<Set<Vertex*>>& vertexClusters) {
    int first = -1;
    int second = -2;
    for(int i = 0; i < vertexClusters.size(); i++) {
        if(vertexClusters[i].contains(v1)) first = i;
        if(vertexClusters[i].contains(v2)) second = i;
        if(first==second) return true;
    }
    vertexClusters[first]+=vertexClusters[second];
    vertexClusters.remove(second);
    return false;
}

/*
 * The kruskalDataSet function uses Kruskal's algorithim to find a minimum spanning tree for a given graph which is then used by the
 * rest of the Trailblazer program to create a maze via inversion. Unlike the kruskal function, it uses expensive data sets and helper
 * functions to keep track of the various sets of edges and vertices rather than a disjoint set.
 * @graph: the graph whose minimum spanning tree is being found
 */
Set<Edge*> kruskalDataSet(BasicGraph& graph) {
    Set<Edge*> mst; //the set of edges that make up the minimum spanning tree
    graph.resetData();
    Vector<Set<Vertex*>> vertexClusters; //the set of vertex clumps that are mutually connected
    PriorityQueue<Edge*> pq;

    for(Edge* edge: graph.getEdgeSet()) {
        pq.enqueue(edge, edge->cost); //enqueue the edges according to their cost
    }

    for(Vertex* v: graph.getVertexSet()) { //put all vertices in separate clusters to begin since none have been connected yet
        Set<Vertex*> s;
        s.add(v);
        vertexClusters.add(s);
    }

    while(!pq.isEmpty()) { //continue as long as there is stuff in the PQ
        Edge* current = pq.dequeue();
        if(!isConnected(current->start, current->end, vertexClusters)) mst.add(current); //if the edge connects two separate vertex clusters add it to the minimum spanning path
    }
    return mst;
}

/*
 * prims is a bonus function with implements a random maze using Prims' algorithim for minimum spanning trees
 * rather than Kruskal's algorithim. It begins be picking a single, random vertex and then connecting unconnected vertices
 * using the minimum cost edges available. The data structures mostly are implemented as pointers to data structures
 * in order to optimize speed. The implementation of small and medium mazes is very fast, large mages moderately fast
 * (under 30 seconds), and huge mazes rather slow (because Prims requires looping).
 * @graph: the graph whose minimum spanning tree is being found
 */
Set<Edge*> prims(BasicGraph& graph) {
    Set<Edge*> mst;
    Set<Edge*>* remainingEdges = new Set<Edge*>(); //the remaining edges to be chosen from
    for(Edge* e: graph.getEdgeSet()) { //starts with all possible edges
        remainingEdges->add(e);
    }

    Set<Vertex*>* treeVertices = new Set<Vertex*>();
    graph.resetData();
    treeVertices->add(graph.getVertexSet().first()); //add random starting vertex

    while(treeVertices->size() < graph.getVertexSet().size()) { //continue as long as not all vertices are added
        double minCost = POSITIVE_INFINITY;
        Edge* minEdge = NULL;
        Vertex* minNeighbor = NULL;
        for(Edge* e: *remainingEdges) { //go through edges that haven't already been used
            if((!(treeVertices->contains(e->start)) ^ !(treeVertices->contains(e->finish))) && e->cost < minCost) { //if one of the edges' vertices is not in the mst already
                minEdge = e;  //add the correct neighbor and edge
                if(treeVertices->contains(e->start)) minNeighbor = e->finish;
                if(treeVertices->contains(e->finish)) minNeighbor = e->start;
                minCost = e->cost; //lower the minimum cost
            }
        }
        if(minEdge != NULL && minNeighbor!=NULL) { //add whatever minimum has been found
            treeVertices->add(minNeighbor);
            mst.add(minEdge);
            remainingEdges->remove(minEdge);
        }
    }
    delete remainingEdges; //prevent memory leak
    delete treeVertices;
    return mst;
}

/*
 * A recursive helper function for the PQ implementation of Prims. It dequeues
 * from the PQ until it finds an edge with a vertex in the mst so far and a vertex
 * outside of it (the first one it finds, by nature of a PQ, will be the minimum cost edge).
 */
void findEdge(PriorityQueue<Edge*>& pq, Set<Vertex*>& treeVertices, Set<Edge*>& mst) {
    if(pq.isEmpty()) throw ("no possible maze"); //the graph is not connected
    Edge* current = pq.dequeue(); //dequeue next edge (minimum cost of viable possibility)
   if(treeVertices.contains(current->start) ^ treeVertices.contains(current->end)){ //if one vertex is in the graph and one isn't
        mst.add(current); //add it to the mst
        if(treeVertices.contains(current->start)) treeVertices.add(current->finish); //add the correct vertex to the treeVertices;
        if(treeVertices.contains(current->finish)) treeVertices.add(current->start);
    }
    else { //otherwise call the function again (so a viable edge can be found) -- recursion necessary to prevent going into an infinite loop or losing the edge
        findEdge(pq, treeVertices, mst);
        pq.add(current, current->cost); //add it back to the pq
    }
}

/*
 * Another implementation of prims which uses a priority queue and recursive helper functio, rather than
 * a loop, to find the minimum possible edge to add to the mst.
 * @graph: the graph whose minimum spanning tree is being found.
 */
Set<Edge*> primsPQ(BasicGraph& graph) {
    Set<Edge*> mst; //the set of edges that make up the minimum spanning tree
    graph.resetData();
    PriorityQueue<Edge*> pq;
    Set<Vertex*> treeVertices;
    treeVertices.add(graph.getVertexSet().first()); //add random

    for(Edge* edge: graph.getEdgeSet()) {
        pq.enqueue(edge, edge->cost); //enqueue the edges according to their cost
    }

    while(treeVertices.size() < graph.getVertexSet().size()) { //continue as long as there is stuff in the PQ
        findEdge(pq, treeVertices, mst);
    }
    return mst;
}



