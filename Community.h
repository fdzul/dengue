// Community.h
// Manages relationship between people, locations, and mosquitoes
#ifndef __COMMUNITY_H
#define __COMMUNITY_H
#include <string>
#include <vector>

class Person;
class Mosquito;
class Location;

class Community
{
    public:
        Community();
        virtual ~Community();
        bool loadPopulation(std::string szPop,std::string szImm);
        bool loadLocations(std::string szLocs,std::string szNet);
        int getNumPerson() { return _nNumPerson; }
        Person *getPerson(int n) { return _person+n; }
        int getNumInfected(int day);
        int getNumSymptomatic(int day);
        int getNumSusceptible(Serotype serotype);
        void populate(Person **parray, int targetpop);
        int getNumLocation() { return _nNumLocation; }
        bool infect(int id, Serotype serotype, int day);
        int addMosquito(Location *p, Serotype serotype, int nInfectedByID);
        int getDay() {                                                // what day is it?
            return _nDay;
        }
        void swapImmuneStates(); 
        void updateWithdrawnStatus(); 
        void mosquitoToHumanTransmission();
        void humanToMosquitoTransmission();
        void tick();                                      // simulate one day
        void setBetaPM(double f) { _fBetaPM = f; }
        void setBetaMP(double f) { _fBetaMP = f; }
        void setMosquitoMoveProbability(double f) { _fMosquitoMoveProb = f; }
        void setMosquitoTeleportProbability(double f) { _fMosquitoTeleportProb = f; }
        double getMosquitoMoveProbability() { return _fMosquitoMoveProb; }
        double setMosquitoTeleportProbability() { return _fMosquitoTeleportProb; }
        void setNoSecondaryTransmission() { _bNoSecondaryTransmission = true; }
        void setMosquitoMultiplier(double f) {                        // seasonality multiplier for number of mosquitoes
            _fMosquitoCapacityMultiplier = f;
        }
        double getMosquitoMultiplier() { return _fMosquitoCapacityMultiplier; }
        int getNumInfectiousMosquitoes();
        int getNumExposedMosquitoes();
        void vaccinate(double f, int age=-1);
        void setVES(double f);
        void setVESs(std::vector<double> f);
        void setVEI(double f);
        void setVEP(double f);
        void setPrimaryPathogenicityScaling(std::vector<double> f) { _fPrimarySymptomaticScaling = f; }
        void setSecondaryPathogenicityScaling(std::vector<double> f) { _fSecondarySymptomaticScaling = f; }
        double getSecondaryPathogenicityScaling(Serotype serotype) { return _fSecondarySymptomaticScaling[(int) serotype]; }
        int getMaxInfectionParity() { return _nMaxInfectionParity; }
        void setMaxInfectionParity(int n) { _nMaxInfectionParity=n; }
        Mosquito *getInfectiousMosquito(int n);
        Mosquito *getExposedMosquito(int n);
        const int *getNumNewlyInfected(Serotype serotype) { return _nNumNewlyInfected[(int) serotype]; }
        const int *getNumNewlySymptomatic(Serotype serotype) { return _nNumNewlySymptomatic[(int) serotype]; }

        const static double SYMPTOMATICBYAGE[MAXPERSONAGE];   // for some serotypes, the fraction who are symptomatic upon primary infection

    protected:
        std::string _szPopulationFilename;                                 // population data filename
        std::string _szImmunityFilename;                                   // immune status data filename
        std::string _szLocationFilename;                                   // location filename
        std::string _szNetworkFilename;                                    // location network filename
        //  Mosquito *_mosquito;
        Person *_person;                                              // the array index is equal to the ID
        Person ***_personAgeCohort;                                   // array of pointers to people of the same age
        int _nPersonAgeCohortSizes[MAXPERSONAGE];             // size of each age cohort
        int _nPersonAgeCohortMaxSize;                                 // size of largest cohort
        int _nOriginalPersonAgeCohortSizes[MAXPERSONAGE];     // size of each age cohort at simulation start time
        double *_fMortality;                                          // mortality by year, starting from 0
        Location *_location;                                          // the array index is equal to the ID
        int **_numLocationMosquitoCreated;                            // number of instantiated mosquitoes at this location at time t.  the first 
                                                                      // array index is equal to the location ID, the second to the day
        Person ***_exposedQueue;                                      // queue of people with n days of latency left
        int _nExposedQueueCapacity;                                   // capacity for each day
        std::vector< std::vector<Mosquito*> > _infectiousMosquitoQueue;         // queue of infectious mosquitoes with n days left to live
        std::vector< std::vector<Mosquito*> > _exposedMosquitoQueue;            // queue of exposed mosquitoes with n days of latency left
        int _nMosquitoQueueCapacity;                                  // capacity for each day
        int _nDay;                                                    // current day
        int _nNumPerson;                                              // number of persons in the simulation
        int _nMaxPerson;                                              // max of persons ever in the simulation
        int _nNumLocation;                                            // number of locations in the simulation
        int _nMaxInfectionParity;                                     // maximum number of infections (serotypes) per person
        double _fBetaPM;                                              // scales person-to-mosquito transmission
        double _fBetaMP;                                              // scales mosquito-to-person transmission (includes bite rate)
        double _fMosquitoMoveProb;                                    // daily probability of mosquito migration
        double _fMosquitoTeleportProb;                                // daily probability of mosquito teleportation (long-range movement)
        bool _bNoSecondaryTransmission;
        double _fMosquitoCapacityMultiplier;                          // seasonality multiplier for mosquito capacity
        std::vector<double> _fPrimarySymptomaticScaling;
        std::vector<double> _fSecondarySymptomaticScaling;
        int _nNumNewlyInfected[NUM_OF_SEROTYPES][MAXRUNTIME];
        int _nNumNewlySymptomatic[NUM_OF_SEROTYPES][MAXRUNTIME];

        void expandExposedQueues();
        void expandMosquitoQueues();
        void moveMosquito(Mosquito *m);
        void _advanceTimers();
        void _modelMosquitoMovement();
        double _fDailyBitingPDF[STEPSPERDAY];                         // probability of biting at 3 different times of day (as defined in Location.h)
};
#endif