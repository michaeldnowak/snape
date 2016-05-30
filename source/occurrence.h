#ifndef OCCURRENCE
#define OCCURRENCE

#include <map>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

class Occurrence{
	int collectionNum;
	string taxonAssignment;
	bool truGenus;
	bool periodPresent;
	bool maxOccAge;
	bool minOccAge;
	//int pbdbBinNum;
	int stageNum;
	double occMax;
	double occMin;
	public:	
	Occurrence(){stageNum=-1;}
	const char *TaxonAssignment() const{
		return taxonAssignment.c_str();
	}
	const bool TrueGenus(){return truGenus;}
	const bool PeriodPresent(){return periodPresent;}
	const bool MaxOccAgePresent(){return maxOccAge;}
	const bool MinOccAgePresent(){return minOccAge;}
	//int BinNum() {return pbdbBinNum;}
	int StageNum() {return stageNum;}
	int CollectionNum() {return collectionNum;}
	double OccMaxAge() {return occMax;}
	double OccMinAge() {return occMin;}
	int Read(ifstream & input);
	void Output(ostream &out);
	bool operator<(const Occurrence &rhs)const{
		int nameCompare = strcmp(TaxonAssignment(), rhs.TaxonAssignment());
		if(nameCompare != 0) return (nameCompare < 0);
		else return (stageNum < rhs.stageNum);
	}
};

#endif 
