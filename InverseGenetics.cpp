/* File: InverseGenetics.cpp
 *
 * A program that, given a sequence of amino acids, returns all possible
 * RNA strands that could encode that amino acid sequence.
 */
#include <iostream>
#include <string>
#include <fstream>
#include "set.h"
#include "map.h"
#include "simpio.h"
#include "console.h"
#include "foreach.h"
using namespace std;

/* Function: allRNAStrandsFor(string protein,
 *                            Map<char, Set<string> >& codons);
 * Usage: foreach (string rna in allRNAStrandsFor("PARTY", codons);
 * ==================================================================
 * Given a protein and a map from amino acid codes to the codons for
 * that code, returns all possible RNA strands that could generate
 * that protein.
 */
Set<string> allRNAStrandsFor(string protein, Map<char, Set<string> >& codons);

/* Function: loadCodonMap();
 * Usage: Map<char, Lexicon> codonMap = loadCodonMap();
 * ==================================================================
 * Loads the codon mapping table from a file.
 */
Map<char, Set<string> > loadCodonMap();
int numberOfStrands;

int main() {
  /* Load the codon map. */
	Map<char, Set<string> > codons = loadCodonMap();
    // Ask user to enter a protein (represented as a string of letters for its amino acids)
    string protein = getLine("Enter a protein, as a combination of the following amino acids: ; A C D E F G H I K L M N P Q R S T V W Y: ");
	Set<string> allRNAStrands = allRNAStrandsFor(protein, codons);
    cout << "Here are all of the possible RNA strands that could generate protein " << protein << endl;
    foreach (string strand in allRNAStrands){
        cout << strand << endl;
    }
    cout<<"There are " << numberOfStrands << " strands in total";
	return 0;
}

Set<string> allRNAStrandsFor(string protein, Map<char, Set<string> >& codons) {
    Set<string> allRNAStrands;
    // BASE CASE: only one amino acid in the protein
    if (protein.length()==1){
        return allRNAStrands=codons.get(protein[0]);
    }
    
    // RECURSIVE STEP
	Set<string> RNAStrands = codons.get(protein[0]);
    Set<string> other = allRNAStrandsFor(protein.substr(1), codons);
    numberOfStrands=0;
    foreach (string RNAStrand in RNAStrands){
        foreach(string RNAStrand2 in other){
            string entry = RNAStrand+RNAStrand2;
            allRNAStrands.add(entry);
            numberOfStrands++;
        }
    }
    return allRNAStrands;
}

/* You do not need to change this function. */
Map<char, Set<string> > loadCodonMap() {
	ifstream input("codons.txt");
	Map<char, Set<string> > result;
    
	/* The current codon / protein combination. */
	string codon;
	char protein;
	
	/* Continuously pull data from the file until all data has been
	 * read.  We did not cover this syntax in class, but it means
	 * "while reading a codon/protein pair succeeds, add the pair to
	 * the map."
	 */
	while (input >> codon >> protein) {
		result[protein] += codon;
	}
	return result;
}

