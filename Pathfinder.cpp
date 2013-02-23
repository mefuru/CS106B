/*
 * File: Pathfinder.cpp
 * --------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Pathfinder application
 * on Assignment #6.
 */
 
// [TODO: complete this implementation]

#include <iostream>
#include <string>
#include <fstream> 
#include <cctype>
#include "console.h"
#include "graphtypes.h"
#include "gpathfinder.h"
#include "graph.h"
#include "console.h"
#include "simpio.h" // for getLine
#include "strlib.h" // for stringToInteger
#include "tokenscanner.h"
#include "pqueue.h"
#include "path.h"

using namespace std;

/* Function prototypes */

void quitAction();

// Run fns readFileContents and display map
void initialiseProgram(Graph<Node,Arc> &graphData);
// Read data from input file
void readFileContents(Graph<Node,Arc> &graphData);
// Create Nodes as per line in input file
void addNodeData(string nodeData, Graph<Node,Arc> &graphData);
// Update graphData with Arcs data
void addArcData(string arcData, Graph<Node,Arc> &graphData);
// display Map and Graph
void displayMapGraph(Graph<Node,Arc> &graphData);
// run Dijkstra
void runDijkstra(Graph<Node,Arc> &graphData);
// run MST
void runKruskal(Graph<Node,Arc> &graphData);
// implements shortest path alogorithm using Vector <Arc*>
Vector<Arc *> findShortestPath(Node *start, Node *finish);
// implements shortest path alogorithm using Path
Path findShortestPath2(Node *start, Node *finish);
// returns true if clickLocation is inside a node's boundaries
bool insideNode(GPoint clickLocation, Node*node);

double getPathCost(const Vector<Arc *> &path) ;

int main() {
    initPathfinderGraphics();
    // Graph that holds Node and Arc data
    Graph<Node,Arc> graphData;
    initialiseProgram(graphData);
    addButton("Map", initialiseProgram,graphData);
    addButton("Dijkstra", runDijkstra,graphData);
    addButton("Kruskal", runKruskal, graphData);
    addButton("Quit", quitAction);
    pathfinderEventLoop();
    return 0;
}

/* Sample callback function */

void quitAction() {
   exit(0);
}

// asks for file input name and displays image and graph
void initialiseProgram(Graph<Node,Arc> &graphData){
    readFileContents(graphData);
    displayMapGraph(graphData);
}

void readFileContents(Graph<Node,Arc> &graphData){
    graphData.clear();
    string filename;
    filename = getLine("Enter filename: ");
    ifstream infile;
    infile.open(filename.c_str());
    if (infile.fail()){
        cout << "Unable to open that file, try again" << endl;
        readFileContents(graphData);
    }
    int lineCounter = 0; // used to keep track of the part of the text file we are reading
    while (true){
        string line;
        getline (infile,line);
        if(infile.fail()) break;
        // Read first line and set as image name
        if (lineCounter==0) {
            drawPathfinderMap(line);
            lineCounter++;
        }
        // Skip next line
        else if (line=="NODES"){
            lineCounter++;
        }
        // read node data until line reads "ARCS"
        else if (lineCounter==2){
            if (line!="ARCS") {
                addNodeData(line, graphData);
            } else lineCounter++;
        }
        else if (lineCounter==3){
            addArcData(line, graphData);
        }
    }
}
    
void addNodeData(string nodeData, Graph<Node,Arc> &graphData){
    // Obtain name and location data from string
    TokenScanner scanner(nodeData);
    scanner.ignoreWhitespace();
    string name = scanner.nextToken();
    int xCoordinate = stringToInteger(scanner.nextToken());
    int yCoordinate = stringToInteger(scanner.nextToken());
    GPoint location (xCoordinate,yCoordinate);
    
    Node *node = new Node;
    node->name = name;
    node->loc = location;
    graphData.addNode(node);
}

void addArcData(string arcData, Graph<Node,Arc> &graphData){
    TokenScanner scanner(arcData);
    scanner.ignoreWhitespace();
    
    string node1 = scanner.nextToken();
    string node2 = scanner.nextToken();
    int cost = stringToInteger(scanner.nextToken());
    
    Arc *arc1 = new Arc;
    arc1->start = graphData.getNode(node1);
    arc1->finish = graphData.getNode(node2);
    arc1->cost = cost;
    
    Arc *arc2 = new Arc;
    arc2->start = graphData.getNode(node2);
    arc2->finish = graphData.getNode(node1);
    arc2->cost = cost;
    
    graphData.addArc(arc1);
    graphData.addArc(arc2);
    
}

void displayMapGraph(Graph<Node,Arc> &graphData){
    // Draw nodes for each node in graphData's node set
    for (Node *node : graphData.getNodeSet()){
        drawPathfinderNode(node->loc, "black", node->name);
    }
    // Draw arcs for each arc in graphData's arc set, passing points for start and finish nodes
    for (Arc *arc : graphData.getArcSet()){
        drawPathfinderArc(arc->start->loc, arc->finish->loc, "black");
    }
}

void runDijkstra(Graph<Node,Arc> &graphData){
    // unhighlight any selected nodes, by redisplaying data
    displayMapGraph(graphData);
    Node *start = NULL;
    Node *finish = NULL;
    
    while (true){
        cout << "Click on starting location... ";
        GPoint clickLocation = getMouseClick();
        for (Node *node : graphData.getNodeSet()){
            if (insideNode(clickLocation, node)){
                start=node;
                drawPathfinderNode(node->loc, "red", node->name);
                cout << node->name << endl;
            }
        }
        if (start!=NULL) break;
        cout << "Invalid click, try again" <<endl;
    }
    
    while (true){
        cout << "Click on final location... ";
        GPoint clickLocation = getMouseClick();
        for (Node *node : graphData.getNodeSet()){
            if (insideNode(clickLocation, node)){
                if (node->name == start->name) cout << "Click another node!" << endl;
                else {
                    finish=node;
                    drawPathfinderNode(node->loc, "red", node->name);
                    cout << node->name << endl;
                }
            }
        }
        if (finish!=NULL) break;
        cout << "Invalid click, try again" <<endl;
    }
    
    cout << "Finding shortest path using Dijkstra" << endl;
    Path shortestPath = findShortestPath2(start, finish);
    
    // Draw arcs and nodes that make up shortest path in red
    foreach (Arc *arc in shortestPath.returnPath()){
        drawPathfinderNode(arc->start->loc, "red", arc->start->name);
        drawPathfinderNode(arc->finish->loc, "red", arc->finish->name);
        drawPathfinderArc(arc->start->loc, arc->finish->loc, "red");
    }
}

Vector<Arc *> findShortestPath(Node *start, Node *finish){
    // USING VECTOR<ARC *>
    Vector<Arc *> path;
    PriorityQueue < Vector <Arc *> > queue;
    Map<string,double> fixed;
    while (start!=finish){
        if(!fixed.containsKey(start->name)) {
            fixed.put(start->name, getPathCost(path));
            for (Arc *arc : start->arcs)    {
                if (!fixed.containsKey(arc->finish->name))  {
                    path.add(arc);
                    queue.enqueue(path, getPathCost(path));
                    path.removeAt(path.size()-1);
                }
            }
        }
        if (queue.isEmpty())    {
            path.clear();
            return path;
        }
        path = queue.dequeue();
        start = path[path.size()-1]->finish;
    }
    return path;
}

double getPathCost(const Vector<Arc *> &path) {
    double cost = 0;
    for (Arc *arc : path)   {
        cost += arc->cost;
    }
    return cost;
}

bool insideNode(GPoint clickLocation, Node*node){
    double xLocation = clickLocation.getX();
    double yLocation = clickLocation.getY();
    double xLowerBounds = node->loc.getX()-NODE_RADIUS;
    double xUpperBounds = node->loc.getX()+NODE_RADIUS;
    double yLowerBounds = node->loc.getY()-NODE_RADIUS;
    double yUpperBounds = node->loc.getY()+NODE_RADIUS;
    if (xLocation >= xLowerBounds  && xLocation <= xUpperBounds && yLocation >= yLowerBounds && yLocation <= yUpperBounds) return true;
    else return false;
    
}

Path findShortestPath2(Node *start, Node *finish){
    // USING Path class instead of Vector<Arc>
    Path path;
    PriorityQueue <Path> queue;
    Map<string,double> fixed;
    while (start!=finish){
        if(!fixed.containsKey(start->name)) {
            fixed.put(start->name, path.pathCost());
            foreach(Arc *arc in start->arcs)    {
                if (!fixed.containsKey(arc->finish->name))  {
                    path.add(arc);
                    queue.enqueue(path, path.pathCost());
                    path.removeLastArcTraversed();
                }
            }
        }
        if (queue.isEmpty())    {
            return Path();
        }
        path = queue.dequeue();
        Vector <Arc *> arcsTraversed = path.returnPath();
        start = path.getFinalArc()->finish;
    }
    cout << path.toString() << " with mileage of " << path.pathCost() <<endl;
    return path;
}

void runKruskal(Graph<Node,Arc> &graphData){
    // unhighlight any selected nodes, by redisplaying data
    displayMapGraph(graphData);
    
    PriorityQueue <Arc *> arcs;
    Vector<Arc *> MST;
    
    // add each arc to pqueue, along with its cost
    for (Arc *arc : graphData.getArcSet()){
        arcs.enqueue(arc, arc->cost);
    }
    
    // For each node, maintain the set of the nodes that are connected to it.  At the start, each node is connected only to itself.
    Map <string, Set < Node * > > nodesAndConnections;
    foreach (Node *node in graphData.getNodeSet()){
        Set<Node *> set;
        set.add(node);
        nodesAndConnections.put(node->name, set);
    }
    
    /* When considering an arc, if its two endpoints already belong to the same connected set, there is no point in adding that arc and thus you skip it. You continue considering arcs and merging connected sets until all nodes are joined into one set*/
    while (arcs.size()>0) {
        Arc *arc = arcs.dequeue();
        Set < Node * > arcStartConnections = nodesAndConnections.get(arc->start->name);
        Set < Node * > arcFinishConnections = nodesAndConnections.get(arc->finish->name);
        if (!arcStartConnections.contains(arc->finish)){
            Set < Node * > mergedConnections = arcStartConnections + arcFinishConnections;
            // have to merge the connected nodes set for all of the nodes that are connected to merged connections
            foreach(Node* node in mergedConnections){
                // get its set of connected arcs and merge its connections with mergedConnections and update Map
                Set < Node * > nodesConnections = nodesAndConnections.get(node->name);
                nodesConnections += mergedConnections;
                nodesAndConnections.put(node->name, nodesConnections);
            }
            MST.add(arc);
            cout << arc->start->name <<  " " << arc -> finish -> name  << " " << arc -> cost  << " " << "is needed." << endl;
        } else cout << arc->start->name  << " " << arc -> finish -> name  << " " << arc -> cost  << " " << "is NOT needed." << endl;
    }

    int totalMileageForMST=0;
    foreach(Arc* arc in MST){
        drawPathfinderNode(arc->start->loc, "red", arc->start->name);
        drawPathfinderNode(arc->finish->loc, "red", arc->finish->name);
        drawPathfinderArc(arc->start->loc, arc->finish->loc, "red");
        totalMileageForMST+=arc->cost;
    }
    cout << "Total mileage for MST: " << totalMileageForMST << endl;

}
