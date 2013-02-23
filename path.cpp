/*
 * File: path.cpp
 * --------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file implements the path.h interface.
 */

/////////////////////////////////////////////////////////////////////
// Your task in this part of the assignment is to write the code   //
// that implements your path.h interface.  For the benefit of      //
// future programmers who will need to work on this interface,     //
// you must also write an extended comment (presumably replacing   //
// this to-do box) that documents the design decisions you made    //
// in structuring the Path class as you did.                       //
/////////////////////////////////////////////////////////////////////

// [TODO: implement the methods in your Path class]


#include "path.h"
#include "gpathfinder.h"

Path::Path() {
  totalCost = 0;
}


Path::~Path() {
}

double Path::pathCost() {
	return totalCost;
}

string Path::toString() {
    string pathString = "";
    if (path.size()==0) return pathString;
    pathString = path[0]->start->name;
	foreach (Arc *arc in path) {
        pathString += "->";
        pathString += arc->finish->name;
    }
	return pathString;
}

void Path::add(Arc* arc) {
	path.add(arc);
	totalCost += arc->cost;
}
    
bool Path::isEmpty() {
	return path.isEmpty();
}

void Path::removeLastArcTraversed(){
    totalCost = totalCost - (path[path.size()-1]-> cost);
    path.removeAt(path.size()-1);
}

void Path::clear(){
    path.clear();
}

Vector<Arc *> Path::returnPath(){
    return path;
}

Arc *Path::getFinalArc() {
	return path[path.size() - 1];
}
