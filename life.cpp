/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>
#include <fstream>  // for reading config .txt files
#include <string>
#include <cctype>
using namespace std;
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay
#include "random.h"
#include "gevents.h" // for mouse click use
#include "grid.h"
#include "vector.h"

int speed;
string animationType;
static void askUserToSelectInput (LifeDisplay &display, Grid<int> &board, Grid<int> &nextBoard);

static void waitForEnter(string message) {
    cout << message;
    (void) getLine();
}

static void welcome() {
  cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
	cout << "Cells live and die by the following rules:" << endl << endl;
	cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
	cout << "\tLocations with 2 neighbors remain stable" << endl;
	cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
	cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
	cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    waitForEnter("Hit [enter] to continue....   ");
}

void initialiseRandomDisplay (LifeDisplay &display, Grid<int> &board, Grid<int> &nextBoard) {
    // initialise random dimensions, between 40 and 60 inclusive
    int xDimension = randomInteger(40,60);
    int yDimension = randomInteger(40,60);
    // resize Grid
    board.resize(xDimension, yDimension);
    nextBoard.resize(xDimension, yDimension);
    // populate grid with either 0 or 1 for age. 50/50 chance
    for (int i = 0; i<xDimension;i++){
        for (int j = 0; j <yDimension; j++){
            board.set(i,j,randomInteger(0,1));
        }
    }
    // Update display, using data from board
    display.setDimensions(xDimension, yDimension);
    for (int i = 0; i<xDimension;i++){
        for (int j = 0; j <yDimension; j++){
            display.drawCellAt(i,j,board.get(i,j));
        }
    }
}

void initialiseDisplayFromFile (LifeDisplay &display, Grid<int> &board, Grid<int> &nextBoard, string &input){
    ifstream infile;
    // have to convert input into C-style string before we can establish an association between that variable and an actual file.
    infile.open(input.c_str());
    // failure checking
    if (infile.fail()){
        cout << "Unable to open that file, try again" << endl;
        askUserToSelectInput(display, board, nextBoard);
    }
    int xDimension;
    int yDimension=0;
    Vector<string> colonyData;
    while (true){
        string line;
        getline (infile,line);
        if(infile.fail()) break; // checks last line correctly
        // If the line contains either 'X' or '-', use the size as xDim and increment the yDimension for each row that contains 'X' or '-'. Also add row to vector colonyData
        if (line.find('-')||line.find('X')){
            xDimension=line.size();
            yDimension++;
            colonyData.add(line);
        }
    }
    
    // update dimensions for board, nextBoard and display
    board.resize(xDimension, yDimension);
    nextBoard.resize(xDimension, yDimension);
    display.setDimensions(xDimension, yDimension);
    // update board with data from colonyData vector
    for (int i = 0; i<xDimension; i++){
        for (int j = 0; j < yDimension; j++){
            string rowData = colonyData.get(i);
            char cellData = rowData[j];
            if (cellData=='X') board.set(i, j, 1);
            else   board.set(i, j, 0);
        }
    }
    // Update display, using data from board
    display.setDimensions(xDimension, yDimension);
    for (int i = 0; i<xDimension;i++){
        for (int j = 0; j <yDimension; j++){
            display.drawCellAt(i,j,board.get(i,j));
        }
    }
}

static void askUserToSelectInput (LifeDisplay &display, Grid<int> &board, Grid<int> &nextBoard) {
    cout << "You can either initialise a random grid, or initialise based on contents of a data file";
    cout << "Type 'rg' for random grid or enter filename, e.g. files/Diehard" << endl;
    string input;
    input = "";
    // had to switch generate debug symbols to off in Xcode
    input = getLine();
    if (input == "rg") initialiseRandomDisplay(display, board, nextBoard);
    else    initialiseDisplayFromFile(display, board, nextBoard, input); 
}

bool boardStagnent (LifeDisplay &display, Grid<int> &board, Grid<int> &nextBoard) {
    // this function updates the board and if all elements are the same after the update, return true
    for (int i = 0; i<board.numRows();i++){
        for (int j = 0; j <board.numCols(); j++){
            int counter = 0;
            // for the 8 cells around the element [i,j], if the age !0 and is in bounds, inc counter by 1
            if (board.inBounds(i-1, j-1) && board.get(i-1,j-1)!=0)  counter++;
            if (board.inBounds(i-1, j) && board.get(i-1,j)!=0)  counter++;
            if (board.inBounds(i-1, j+1) && board.get(i-1,j+1)!=0)  counter++;
            if (board.inBounds(i, j-1) && board.get(i,j-1)!=0)  counter++;
            if (board.inBounds(i, j+1) && board.get(i,j+1)!=0)  counter++;
            if (board.inBounds(i+1, j-1) && board.get(i+1,j-1)!=0)  counter++;
            if (board.inBounds(i+1, j) && board.get(i+1,j)!=0)  counter++;
            if (board.inBounds(i+1, j+1) && board.get(i+1,j+1)!=0)  counter++;
            // update nextBoard according to counter value if less than max age
            if (counter ==0 || counter == 1 || counter > 3)    nextBoard.set(i,j,0);
            if (counter == 2 && board.get(i,j) !=0 && board.get(i, j) <=kMaxAge)   nextBoard.set(i,j,board.get(i, j)+1);
            if (counter == 2 && board.get(i,j) ==0)   nextBoard.set(i,j,0);
            if (counter ==3 && board.get(i, j) <=kMaxAge)    nextBoard.set(i,j,board.get(i, j)+1);
        }
    }
    
    // if any elements btw grids are diff return false
    
    for (int i = 0; i<board.numRows();i++){
        for (int j = 0; j <board.numCols(); j++){
            if (board.get(i,j)!= nextBoard.get(i,j)) return false;
        }
    }
    return true;
}

void advanceBoard (LifeDisplay &display, Grid<int> &board, Grid<int> &nextBoard) {
    // this function updates the board for next round and updates the display accordingly
    board = nextBoard;
    // update display
    for (int i = 0; i<board.numRows();i++){
        for (int j = 0; j <board.numCols(); j++){
            display.drawCellAt(i,j,board.get(i,j));
        }
    }
}

void chooseSpeed() {
    cout << "You can choose how fast you run Life, select a number between 1-100, 100 for fastest " << endl;
    int speedChoice = getInteger();
    if (speedChoice<1 || speedChoice > 100) {
        cout << "You have to choose a number between 1-100, try again" << endl;
        chooseSpeed();
    }
    speed = 1000 / speedChoice;
}

void manualOrAuto () {
    cout << "If you want the animation to run manually, type 'M', otherwise hit return for auto animation" << endl;
    string input;
    input = "";
    // had to switch generate debug symbols to off in Xcode
    input = getLine();
    if (input == "M") animationType = "manual";
    else    animationType = "auto";
}

int main() {
    
    // create an empty LifeDisplay
    LifeDisplay display;
    display.setTitle("Game of Life");
    // create an empty Grid called board and nextBoard
    Grid <int> board;
    Grid <int> nextBoard;
    // welcome text
    welcome();
    manualOrAuto();
    if (animationType == "auto") chooseSpeed();
    askUserToSelectInput(display, board, nextBoard);
    int rounds = 0;
    if (animationType == "auto") {
        while (boardStagnent(display, board, nextBoard) == false) {
            pause(speed);
            advanceBoard(display, board, nextBoard);
            rounds++;
            cout << "Round: " << integerToString(rounds) << endl;
        }
    }
    if (animationType != "auto"){
        while (boardStagnent(display, board, nextBoard) == false) {
            waitForClick();
            advanceBoard(display, board, nextBoard);
            rounds++;
            cout << "Round: " << integerToString(rounds) << endl;
        }
    }
   
    cout << "The board is stable after " <<  integerToString(rounds) << " rounds. Simulation is over" << endl;
    return 0;
}
