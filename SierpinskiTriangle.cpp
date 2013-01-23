/*
 * File: SierpinskiTriangle.cpp
 */

#include <iostream>
#include <cmath>
#include "simpio.h" // used for getInteger fn
#include "gwindow.h" // used for graphics disp
#include "gtypes.h" // for GPoint
#include "console.h" // required of all files that contain the main function
#include "graphics.h" // required for initGraphics();
using namespace std;

static GPoint drawSierpinskiTriangle (int edgeLength, int fractalOrder, double cx, double cy, GWindow &window){
    
    // BASE CASE: ORDER = 0
    if (fractalOrder==0){
        int outlineTriangleHeight = sqrt(edgeLength*edgeLength - (edgeLength/2)*(edgeLength/2));
        GPoint pt0 (cx - edgeLength/2, cy + outlineTriangleHeight/2);
        pt0 = window.drawPolarLine(pt0, edgeLength, 0);
        pt0 = window.drawPolarLine(pt0, edgeLength, +120);
        pt0 = window.drawPolarLine(pt0, edgeLength, -120);
        return pt0;
    }
    
    // RECURSIVE STEP
    edgeLength = edgeLength/2;
    int triangleHeight = sqrt(edgeLength*edgeLength - (edgeLength/2)*(edgeLength/2));
    // Three bottom left points for three inner triangles
    GPoint pt1 (cx - edgeLength, cy + triangleHeight);
    GPoint pt2 (cx, cy + triangleHeight);
    GPoint pt3 (cx - edgeLength/2, cy);
    
    // Draw bottom left triangle
    pt1 = window.drawPolarLine(pt1, edgeLength, 0);
    pt1 = window.drawPolarLine(pt1, edgeLength, +120);
    pt1 = window.drawPolarLine(pt1, edgeLength, -120);
    int bottomLeftCentreXDimension = cx-edgeLength/2;
    int bottomLeftCentreYDimension = cy+triangleHeight/2;
    drawSierpinskiTriangle (edgeLength, fractalOrder-1,  bottomLeftCentreXDimension,  bottomLeftCentreYDimension, window);
    
    // Draw bottom right triangle
    pt2 = window.drawPolarLine(pt2, edgeLength, 0);
    pt2 = window.drawPolarLine(pt2, edgeLength, +120);
    pt2 = window.drawPolarLine(pt2, edgeLength, -120);
    int bottomRightCentreXDimension = cx+edgeLength/2;
    int bottomRightCentreYDimension = cy+triangleHeight/2;
    drawSierpinskiTriangle (edgeLength, fractalOrder-1,  bottomRightCentreXDimension,  bottomRightCentreYDimension, window);
    
    // Draw top triangle
    pt3 = window.drawPolarLine(pt3, edgeLength, 0);
    pt3 = window.drawPolarLine(pt3, edgeLength, +120);
    pt3 = window.drawPolarLine(pt3, edgeLength, -120);
    int topXDimension = cx;
    int topYDimention = cy - triangleHeight/2;
    drawSierpinskiTriangle (edgeLength, fractalOrder-1,  topXDimension,  topYDimention, window);
    return pt1;
}


static int getEdgeLength(){
    cout << "Enter outer edge length: (>0) " << endl;
    int edgeLength = getInteger();
    if (edgeLength<1) {
        cout << "Edge length has to be greater than 0 and Fractal order has to be greater than or equals to 0" << endl;
        getEdgeLength();
    }
    return edgeLength;
}

static int getFractalOrder(){
    cout << "Enter fractal order: (>=0) " << endl;
    int fractalOrder = getInteger();
    if (fractalOrder<0) {
        cout << "Edge length has to be greater than 0 and Fractal order has to be greater than or equals to 0" << endl;
        getFractalOrder();
    }
    return fractalOrder;
}

int main() {

    initGraphics();
    // I get an error msg if I don't specify the window's dimensions
    GWindow window(1000,1000);
    cout << "Program to make Sierpinski Triangle fractal" << endl;
    int edgeLength = getEdgeLength();
    int fractalOrder = getFractalOrder();
    double cx = window.getWindowWidth()/2;
    double cy = window.getWindowHeight()/2;
    drawSierpinskiTriangle (edgeLength, fractalOrder, cx, cy, window);
    return 0;
}
