#ifndef NODEOFINTEREST
#define NODEOFINTEREST

#include <string>
#include <vector>
#include "lineage.h"

using namespace std;

class NodeOfInterest{
	vector<Lineage> lineages;
	double perBinOrigRate[85];
	double perBinDeathRate[85];
	int perBinDiversity[85];
	double meanOriginationRate;
	double meanExtinctionRate;
	int tossedOccs;
	int firstBinOfNode;
	int lastBinOfNode;
	int lineagesInFirstBin;
	int timers[85];
	double preservationProb;
	public:
	NodeOfInterest(){
			firstBinOfNode=lastBinOfNode=-1;
	}
	void AssignOccurrencesToLineages(vector<Occurrence> &occs);
	int LineagesInFirstBin() {return lineagesInFirstBin;}
	int FirstBinOfNode() {return firstBinOfNode;}
	double PreservationProbability() {return preservationProb;}
	double OriginationRate() {return meanOriginationRate;}
	double ExtinctionRate() {return meanExtinctionRate;}
	void EditDataSet();
	void CalcPerBinR8s();
	void CalcPreservationProb();
	void CalcPrior();
	double ProbOfMissingIntervalGivenFirstAndCurrentDiversity(double missing_interval, int diversity_at_first_obs, int current_diversity);
	
	void OutputLineages();
	void FindFirstBinOfNode();
	void EraseLineage(int);
	void EditLineage(int);

};

#endif
