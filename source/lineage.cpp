#include <iostream>

#include "lineage.h"
#include "timeinterval.h"

extern TimeIntervalSet timeIntervals;

Lineage::Lineage(const Lineage &rhs){
	for(vector<Occurrence>::const_iterator it=rhs.occ.begin();it!=rhs.occ.end();it++){
		occ.push_back(*it);
		}
	taxName = rhs.taxName;
	firstBin = rhs.firstBin;
	lastBin = rhs.lastBin;
	
	for(int i=0;i<85;i++)
		diversity[i] = rhs.diversity[i];
}

void Lineage::CalcStratRange(){
	//fill the firstBin and lastBin from the sorted occ array here
	firstBin = occ[occ.size() - 1].StageNum();
	lastBin = occ[0].StageNum();
	for(int i=0;i<85;i++) 
		diversity[i] = 0;
	for(vector<Occurrence>::iterator occit = occ.begin() ; occit != occ.end() ; occit++){
		diversity[(*occit).StageNum()]++;
		}
}

void Lineage::OutputOccurences(){
	cout << "Current occurrence list for " << TaxName() << endl;
	cout << "Occurrence#\tPBDB collection#\tAge of occurrence" << endl;
	for(int r=0; r<occ.size(); r++){
		cout << r << "\t\t\t\t" << occ[r].CollectionNum() << "\t\t\t\t" << timeIntervals.Base(occ[r].StageNum()) << endl;
	}
}

void Lineage::EraseOccurence(int occ2Edit){
	vector<Occurrence>::iterator p = occ.begin();
	p += occ2Edit;
	occ.erase(p, p+1);
}

void Lineage::EditLineage(){
	int occ2Edit;
	do{
		cout << "Please enter a single occurrence number to delete, or -1 to return to lineage list: ";
		cin >> occ2Edit;
		if(occ2Edit > -1 && occ2Edit < occ.size()){
			EraseOccurence(occ2Edit);
			OutputOccurences();
			CalcStratRange();
		}
		else if(occ2Edit != -1){ 
			cout << "Sorry, this is not a valid occurrence number. Please check the list of occurrences above and try again." << endl;
		}
	}while(occ2Edit != -1);
}