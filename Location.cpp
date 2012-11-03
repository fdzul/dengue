// Location.cpp

#include <cstdlib>
#include <cstring>
#include <climits>
#include <assert.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "Mosquito.h"
#include "Person.h"
#include "Location.h"
#include "Parameters.h"

using namespace dengue::standard;

int Location::_nNextID = 0;
int Location::_nDefaultMosquitoCapacity = 20;

Location::Location()
    : _person(3, vector<Person*>(0) ) {
    _nID = _nNextID++;
    _neighbors = NULL;
    _nNumNeighbors = _nMaxNeighbors = 0;
    _nBaseMosquitoCapacity = _nDefaultMosquitoCapacity;
    _bUndefined = false;
}


Location::~Location() {
    if (_person.size()) {
        for (unsigned int i = 0; i<_person.size(); i++) {
            for (unsigned int j = 0; j<_person[i].size(); j++) {
                delete _person[i][j]; 
            }
        }
    }
    if (_neighbors)
        delete [] _neighbors;
}


void Location::addPerson(Person *p, int t) {
    //assert((unsigned) t < _person.size());
    _person[t].push_back(p);
}


bool Location::removePerson(Person *p, int t) {
    //assert((unsigned) t < _person.size());
    for (unsigned int i=0; i<_person[t].size(); i++) {
        if (_person[t][i] == p) {
            _person[t][i] = _person[t].back();
            _person[t].pop_back();
            return true;
        }
    }
    return false;
}


Person *Location::getPerson(int idx, int timeofday) {
    //assert((unsigned) timeofday < _person.size());
    //assert((unsigned) idx < _person[timeofday].size());
    return _person[timeofday][idx];
}


// addNeighbor - adds location p to the location's neighbor list.
// Note that this relationship is one-way.
void Location::addNeighbor(Location *p) {
    if (_neighbors==NULL) {
        _nMaxNeighbors = 20;
        _neighbors = new Location *[_nMaxNeighbors];
    }
    for (int i=0; i<_nNumNeighbors; i++)
        if (_neighbors[i]==p)
            return;                                                   // already a neighbor
    if (_nNumNeighbors>=_nMaxNeighbors) {
        Location **temp = new Location *[_nMaxNeighbors*2];
        for (int i=0; i<_nNumNeighbors; i++)
            temp[i] = _neighbors[i];
        delete [] _neighbors;
        _neighbors = temp;
    }
    _neighbors[_nNumNeighbors++] = p;
}