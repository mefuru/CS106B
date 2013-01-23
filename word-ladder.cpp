/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
#include <string>
#include <cctype>
using namespace std;
#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "vector.h"
#include "queue.h"

static void playWordLadder();

static int generateLadder(string start, string end) {
    // A queue of vector <string> holds data from breadth first search
    Queue < Vector <string> > wordLadder;
    // add a base case which just includes the start word
    Vector <string> base;
    base.add(start);
    wordLadder.enqueue(base);
    // Lexicon from data file
    Lexicon english("EnglishWords.dat");
    // Lexicon holds words previously encountered
    Lexicon previousWords;
    previousWords.add(start);
   
    while(!wordLadder.isEmpty()){
        Vector<string> currentVector = wordLadder.dequeue();
        // if the last string in the Vector equals the end word, we have found a complete ladder
        if (currentVector[currentVector.size()-1]==end) {
            cout << "Found ladder: ";
                for (int i = 0; i < currentVector.size()-1; i++){
                    cout << currentVector[i] << " ";
                }
                cout << endl;
                return 1;
        }
            else {
                
                // for loop that goes through each character position in the word
                for (int i = 0; i < start.length(); i ++){
                    // loops through the letters of the alphabet
                    for (char alphabet = 'a'; alphabet < '{'; alphabet++){
                        string wordAltered = currentVector[currentVector.size()-1];
                        // replacing the character in that index position with each of the 26 letters in turn
                        wordAltered[i]=alphabet;
                        // if end word isn't in dictionary but is found, cout ladder.
                            if (wordAltered==end){
                                // N.B. don't use currentVector.size()-1 because the currentVector hasn't been updated with end word
                                for (int i = 0; i < currentVector.size(); i++){
                                    cout << currentVector[i] << " ";
                                }
                                cout << end << endl;
                                return 1;
                            }
                            // if wordAltered is in dictionary and hasn't been reached before, update word ladder
                            if (english.contains(wordAltered.c_str()) && !previousWords.contains(wordAltered.c_str())){
                                // copy currentVector, add the wordAltered to the newWords vector, update previous words Lexicon, enqueue newWords vector
                                Vector <string> newWords = currentVector;
                                newWords.add(wordAltered);
                                previousWords.add(wordAltered);
                                wordLadder.enqueue(newWords);
                            }
                    }
                }
            }
    }
    
    // if queue becomes empty, can't create a ladder. Ask user to play again
    if(wordLadder.isEmpty()){
        cout << "Can't create a word ladder between " << start << " and " << end << endl;
        cout << "Try again" << endl;
        playWordLadder();
    }
    return 0;
}


static void playWordLadder() {
    while (true) {
        cout << "Please enter start word in lower case " << endl;
        string start = getLine();
        if (start.empty()) break;
        cout << "Please enter end word in lower case " << endl;
        string end = getLine();
        if (end.empty()) break;
        if (start.length() != end.length()) {
            cout << "Those words aren't the same length, pls choose again." << endl;
            playWordLadder();
        }
        generateLadder(start, end);
    }
}

int main() {
    cout << "Welcome to the CS106B word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
