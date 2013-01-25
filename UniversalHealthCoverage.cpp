/* File: UniversalHealthCoverage.cpp
 *
 * A program that determines whether or not it is possible to provide
 * coverage to everyone using a fixed number of hospitals.
 */

#include <iostream>
#include <string>
#include "set.h"
#include "vector.h"
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function: canOfferUniversalCoverage(Set<string>& cities,
 *                                     Vector< Set<string> >& locations,
 *                                     int numHospitals,
 *                                     Vector< Set<string> >& result);
 * Usage: if (canOfferUniversalCoverage(cities, locations, 4, result)
 * ==================================================================
 * Given a set of cities, a list of what cities various hospitals can
 * cover, and a number of hospitals, returns whether or not it's
 * possible to provide coverage to all cities with the given number of
 * hospitals.  If so, one specific way to do this is handed back in the
 * result parameter.
 */
bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
                               Vector< Set<string> >& result);


// ASK USER TO INPUT MULTIPLE STRINGS, EACH WILL BE ADDED TO SET<STRING>
static Set<string> inputCities();
// I couldn't be arsed to update the locations vector every time I ran the program, so I stored them in manually
static Vector< Set<string> > inputLocations();
// ASK USER TO INPUT MAX. NO. OF HOSPITALS
static int inputNumHospitals();

int main() {
  Set<string> cities = inputCities();
    Vector< Set<string> > locations = inputLocations();
    int numHospitals = inputNumHospitals();
	Vector< Set<string> > result;
    if (canOfferUniversalCoverage(cities,locations,numHospitals,result)==false){
        cout << "Raise some more cash, we can't build enough hospitals to cover all cities" << endl;
    } else {
        cout << "The following hospital combination can cover all cities: " << endl;
        cout << result.size() << " hospital(s) are required" << endl << endl;
        foreach (Set<string> hospitalInResult in result){
            cout << "The following cities are covered by this hospital" << endl<< endl;
            foreach (string cityCovered in hospitalInResult) {
                cout << cityCovered << endl;
            }
        }
    }
    return 0;
}

static Set<string> inputCities() {
    cout << "Please enter the names of all the cities we need coverage for" << endl;
    cout << "Type end to finish" <<  endl;
    Set<string> cities;
    while(!cities.contains("end")){
        string cityEntry;
        cityEntry = getLine();
        cities.add(cityEntry);
    }
    cities.remove("end");
    cout << "Here is a list of all the cities you entered: " << endl;
    foreach (string entry in cities){
        cout << entry << endl;
    }
    return cities;
}

static Vector< Set<string> > inputLocations(){
    cout << "Please enter the names of all the cities each hospital will provide coverage for" << endl;
    Vector< Set<string> > locations;
    Set<string> hospitalData1;
    hospitalData1.add("london");
    hospitalData1.add("manchester");
    hospitalData1.add("birmingham");
    hospitalData1.add("leeds");
    cout << endl;
    Set<string> hospitalData2;
    hospitalData2.add("london");
    hospitalData2.add("manchester");
    hospitalData2.add("birmingham");
    hospitalData2.add("liverpool");
    cout << endl;
    Set<string> hospitalData3;
    hospitalData3.add("phuket");
    hospitalData3.add("tao");
    hospitalData3.add("samui");
    hospitalData3.add("lanta");
    cout << endl;
    Set<string> hospitalData4;
    hospitalData4.add("purley");
    hospitalData4.add("caterham");
    hospitalData4.add("epsom");
    hospitalData4.add("croydon");
    cout << endl;
    locations.add(hospitalData1);
    locations.add(hospitalData2);
    locations.add(hospitalData3);
    locations.add(hospitalData4);
    cout << "Number of hospitals (location size)" << locations.size() << endl;
    return locations;
}

static int inputNumHospitals() {
    int numHospitals = getInteger("Please enter the maximum number of hospitals: ");
    if (numHospitals<1){
        cout << "Maximum no of hospitals has to be greater than or equal to 1" << endl;
        inputNumHospitals();
    }
    return numHospitals;
}

bool canOfferUniversalCoverage(Set<string>& cities, Vector< Set<string> >& locations, int numHospitals, Vector< Set<string> >& result) {
    
    // BASE CASE 1: Only one hospital left. If all remaining cities must be covered by one hospital, add that hospital's details to result and return true
    if (numHospitals==1){
        for (int i = 0; i<locations.size(); i ++){
                if (cities.isSubsetOf(locations[i])){
                result.add(locations[i]);
                return true;
            }
        }
        return false;
    }
    // BASE CASE 2: If all remaining cities can be covered by one hospital, add that hospital's details to result and return true
    for (int i =0; i<locations.size();i++){
        if (cities.isSubsetOf(locations[i])){
            result.add(locations[i]);
            return true;
        }
    // RECURSIVE STEP
    for (int i = 0; i < locations.size();i++){
        // create copies of locations and cities
        Vector< Set<string> > updatedLocations = locations;
        Set <string> updatedCities = cities;
        // updatedCities equals cities minus cities covered by hospital - updatedLocations[j]
        updatedCities = cities - updatedLocations[i];
        // if updatedcities doesn't not equal cities, we update updatedLocations
        if (updatedCities!=cities){
            result.add(updatedLocations[i]);
        }
        // remove current hospital from list of hospitals
        updatedLocations.removeAt(i);
        if (canOfferUniversalCoverage(updatedCities, updatedLocations, numHospitals-1, result)==true){
            return true;
        }
    }
    return false;
}
    
}


