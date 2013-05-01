#ifndef LINEAGE
#define LINEAGE

#include <string>
#include <vector>
#include "occurrence.h"

using namespace std;

class Lineage{

	string taxName;
	int firstBin;
	int lastBin;
	int diversity[85];
	int totalNumBins;	
	vector<Occurrence> occ;

	public:
	Lineage(const char *n){
		taxName = n;
		firstBin = -1;
		lastBin = -1;
		totalNumBins = -1;
		
	}
	//Copy constructor
	Lineage(const Lineage &rhs);
	void AddOccurrence(Occurrence &o){
		occ.push_back(o);
	}
	const char *TaxName() const {return taxName.c_str();}
	size_t NumberOfOccurrences() const {return occ.size();}
	int FirstBin(){return firstBin;}
	int LastBin(){return lastBin;}
	int TotalNumBins(){
		for(int i = 0; i < 85; i++){
			if(diversity[i] > 0){
				totalNumBins++;
			}
		}
	return totalNumBins;
	}

	void CalcStratRange();
	void EditLineage();
	void EraseOccurence(int);
	void OutputOccurences();

};

#endif
