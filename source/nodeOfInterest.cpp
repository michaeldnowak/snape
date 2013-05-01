#include "nodeOfInterest.h"
#include "lineage.h"
#include "occurrence.h"
#include "timeinterval.h"
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

extern TimeIntervalSet timeIntervals;

double fact(int i);
double choose(int i, int j);

void NodeOfInterest::AssignOccurrencesToLineages(vector<Occurrence> &occs){
	//declare a vector<Occurrence>::iterator and set it equal to 
	//the start of the occs vector
	vector<Occurrence>::iterator curOcc = occs.begin(); 
	int tossedOccs = 0;
	while(!((*curOcc).TrueGenus()) || !((*curOcc).PeriodPresent())){
		curOcc++;
		tossedOccs++;
	}
	lineages.push_back( Lineage( (*curOcc).TaxonAssignment() ) );
	Lineage *curLin = &lineages.back();
	//now, add curOcc to the vector of Occurrences in curLin
	curLin->AddOccurrence(*curOcc);
	//we've now dealt with the first Occurrence, so move the iterator forward
	curOcc++;
	for(;curOcc != occs.end();curOcc++){
		if(((*curOcc).TrueGenus()) && ((*curOcc).PeriodPresent())){
			//This if is testing for True genera and occs with period data present
			if(!strcmp((*curOcc).TaxonAssignment(), (*curLin).TaxName())){ 
				curLin->AddOccurrence(*curOcc); //if the names are the same
			}
			else{
				//create a new Lineage				
				lineages.push_back( Lineage( (*curOcc).TaxonAssignment() ) );
				curLin = &lineages.back();
				curLin->AddOccurrence(*curOcc);
			}
		}
		else{tossedOccs++;}
	}
	//calcuate the first and last bin of each lineage and of the whole node of interest
	firstBinOfNode=0;
	lastBinOfNode=48;
	for(int i=0; i < lineages.size(); i++){
		lineages[i].CalcStratRange();
		if(lineages[i].FirstBin() > firstBinOfNode) firstBinOfNode = lineages[i].FirstBin();
		if(lineages[i].LastBin() < lastBinOfNode) lastBinOfNode = lineages[i].LastBin(); 
	}
	
	cout << "Number of lineages found: " << lineages.size() << endl;
	cout << "Number of Occurrences removed for not being asignable\nto a genus, or missing temporal information: " << tossedOccs << endl;
	cout << "Lineages retained:" << endl;
	cout << "Lineage#\tLineage name\t Range\t\t\t   #Occurrences\n";
}

void NodeOfInterest::OutputLineages(){
	for(int i=0; i < lineages.size(); i++){
		printf("%-10d%-15s%3.1f - %3.1f\t\t%zu\n", i, lineages[i].TaxName(), timeIntervals.Base(lineages[i].FirstBin()), timeIntervals.Base(lineages[i].LastBin()) - timeIntervals.Length(lineages[i].LastBin()), lineages[i].NumberOfOccurrences());
	}
}

void NodeOfInterest::FindFirstBinOfNode(){
	firstBinOfNode=0;
	lastBinOfNode=48;
	for(int i=0; i < lineages.size(); i++){
		lineages[i].CalcStratRange();
		if(lineages[i].FirstBin() > firstBinOfNode) firstBinOfNode = lineages[i].FirstBin();
		if(lineages[i].LastBin() < lastBinOfNode) lastBinOfNode = lineages[i].LastBin(); 
	}
}

void NodeOfInterest::EraseLineage(int lineage2Edit){
	assert(lineage2Edit < lineages.size());
	vector<Lineage>::iterator p = lineages.begin();
	p += lineage2Edit;
	lineages.erase(p, p+1);
}


void NodeOfInterest::EditDataSet(){
	//This function allows the user to edit the data set with simple keyboard commands
	int lineage2Edit = 0;
	int eraseOrEdit = 0;
	int editYN = 0;
	
	do{
		OutputLineages();
		cout << "Would you like to remove lineages or occurrences from the PBDB data set currently in memory?" << endl;
		cout << "1. Yes, some of these data are ridiculous!" << endl << "2. No, I would like to keep the data set as it is and run the analysis." << endl;
		cin >> editYN;
		switch(editYN){
			case 1:
				do{
					cout << "Please enter a single lineage number from the above list that you would like to edit or delete: ";
					cin >> lineage2Edit;
					if(lineage2Edit < lineages.size()){
						do{
							cout <<"Current occurences in lineage " << lineages[lineage2Edit].TaxName() << endl;
							lineages[lineage2Edit].OutputOccurences();
							cout << "Would you like to completely erase the lineage " << lineages[lineage2Edit].TaxName() << ", or edit the occurrence list that composes this lineage?" << endl;
							cout << "1. Erase the whole lineage" << endl << "2. Edit occurrence list" << endl << "3. Return to lineage selection" << endl;
							cin >> eraseOrEdit;
							switch(eraseOrEdit){
								case 1:
									EraseLineage(lineage2Edit);
									eraseOrEdit = 3;
									break;
								case 2:
									lineages[lineage2Edit].EditLineage();
									eraseOrEdit = 3;
									break;
								case 3: 
									break;
								default :
									cout << "Not a valid entry, please select option 1 or 2." << endl;
									break;
							}
						}while(eraseOrEdit != 3);
					}
					else{ 
						cout << "Sorry, this is not a valid lineage number. Please check the list of lineages above and try again." << endl;
					}
				} while(lineage2Edit > lineages.size());
				break;
			case 2:
				break;
			default:
				cout << "Not a valid entry, please select option 1 or 2." << endl;
				break;
		}
	}while(editYN != 2);
	//Now that the dataset has been altered, we need to recalculate the first bin of node
	FindFirstBinOfNode();
}


void NodeOfInterest::CalcPerBinR8s(){
	//This function calculates the per-bin origination and extinction rates using the 
	//method outlined by Foote (2000). See README.txt for a complete citation.
	for(int i=0; i < 85; i++){
		int lineagesConfined2Interval = 0; //Foote's NFL
		int lineagesBottomBoundaryXed = 0; //Foote's NbL
		int lineagesTopBoundaryXed = 0; //Foote's NFt
		int lineagesBothBoundariesXed = 0; //Foote's Nbt
		int totalTopBoundaryXers = 0; //Foote's Nt
		int totalBottomBoundaryXers = 0; //Foote's Nb
		for(int j=0; j < lineages.size(); j++){
			if(lineages[j].FirstBin() == i){
				if(lineages[j].LastBin() == i){
					lineagesConfined2Interval++;
				}
				else lineagesTopBoundaryXed++;
			}
			else if(lineages[j].LastBin() == i){
				lineagesBottomBoundaryXed++;
			}
			else if((lineages[j].LastBin() < i) && (i < lineages[j].FirstBin())){
				lineagesBothBoundariesXed++;
			}
		}
		totalTopBoundaryXers = lineagesTopBoundaryXed + lineagesBothBoundariesXed;
		totalBottomBoundaryXers = lineagesBottomBoundaryXed + lineagesBothBoundariesXed;
		if(lineagesBothBoundariesXed > 0 && (lineagesBothBoundariesXed != totalBottomBoundaryXers)){
			// calculate Foote's estimate of q = -ln(Nbt / Nb) / dT
			perBinDeathRate[i] = -log((double)lineagesBothBoundariesXed / (double)totalBottomBoundaryXers)/timeIntervals.Length(i);
		}
		else{
			perBinDeathRate[i] = 0;
		}
		if(lineagesBothBoundariesXed > 0 && (lineagesBothBoundariesXed != totalTopBoundaryXers)){
			// calculate Foote's estimate of p = -ln(Nbt / Nt) / dT
			perBinOrigRate[i] = -log((double)lineagesBothBoundariesXed / (double)totalTopBoundaryXers)/timeIntervals.Length(i);
		}
		else{
			perBinOrigRate[i] = 0;
		}
		perBinDiversity[i] = lineagesConfined2Interval + lineagesBottomBoundaryXed + lineagesTopBoundaryXed + lineagesBothBoundariesXed;
	}
	
	lineagesInFirstBin = 0;
	for(int i = lastBinOfNode;i <= firstBinOfNode;i++){
		if(lineages[i].FirstBin() == firstBinOfNode) lineagesInFirstBin++; 
	}
	
	cout << "per bin rates:" << endl;
	cout << "name\t\t\tbase (Ma)\t#lineages\tbirth\t\tdeath" << endl;
	for(int i = lastBinOfNode;i <= firstBinOfNode;i++){
		printf("%-18s\t%3.2f\t\t%-5d\t\t%1.5f\t%1.5f\n", timeIntervals.Name(i).c_str(), timeIntervals.Base(i), perBinDiversity[i], perBinOrigRate[i],  perBinDeathRate[i]);
	}
	
	double sumOrigRates = 0.0;
	double sumDeathRates = 0.0;
	int numBinsOrigRates = 0;
	int numBinsDeathRates = 0;
	for(int i = lastBinOfNode;i <= firstBinOfNode;i++){
		//DEBUG correcting for averaging over bins with 0 orig and extinction rates
		if(perBinOrigRate[i] > 0.0){
			sumOrigRates = sumOrigRates + perBinOrigRate[i];
			numBinsOrigRates++;
		}
		if(perBinDeathRate[i] >0.0){
		sumDeathRates = sumDeathRates + perBinDeathRate[i];
		numBinsDeathRates++;
		}
	}
	meanOriginationRate = sumOrigRates/(double)(numBinsOrigRates);
	meanExtinctionRate = sumDeathRates/(double)(numBinsDeathRates);
	
	cout << "mean origination rate:\t" << meanOriginationRate << endl;
	cout << "mean extinction rate:\t" << meanExtinctionRate << endl;
}


void NodeOfInterest::CalcPreservationProb(){
	//estimate the preservation probability (R) using the FreqRat method developed by Foote & Raup (1997)
	//See README.txt for full citation
	preservationProb = 0;
	for(int w=0; w < 85; w++) timers[w] = 0;
	double sumDurations = 0.0; 	
	for(int i=0; i < lineages.size(); i++){
		if(lineages[i].FirstBin() != lineages[i].LastBin()){
			//calculate the sum of all durations for later calculations
			sumDurations += (double)timeIntervals.Base(lineages[i].FirstBin()) - ((double)timeIntervals.Base(lineages[i].LastBin()) + (double)timeIntervals.Length(lineages[i].LastBin()));
		}
		for(int j=0; j < 85; j++){
			//fill timers array
			if(lineages[i].FirstBin() == lineages[i].LastBin() + j) timers[j]++;
		}
	}
	

	int numLineagsMinusSingletons = 0;
	for(int i=1; i<85; i++){
	//Calculate the number of lineages minus singletons
		numLineagsMinusSingletons += timers[i]; 
	}
	
	double meanBinLength = 0.0;
	double sumBinLengths = 0.0;
	int totalBins = 0;
	for(int i = lastBinOfNode;i <= firstBinOfNode;i++){
		sumBinLengths += timeIntervals.Length(i);
		totalBins++;
	}	
	meanBinLength = sumBinLengths / totalBins;
	
	//must exclude singletons when calculating meanDuration for estimating #3timers or #2 timers !!
	double meanDuration = sumDurations / (double)numLineagsMinusSingletons;
	cout << "Mean Stratigraphic Range of Lineages (excluding singletons): " << meanDuration << endl;

	if(timers[1] > timers[2] && timers[2] > 1){
		cout << "Frequency of One-Timers: " << (double)timers[0]/lineages.size() << endl;
		cout << "Frequency of Two-Timers: " << (double)timers[1]/lineages.size() << endl;
		cout << "Frequency of Three-Timers: " << (double)timers[2]/lineages.size() << endl;
		cout << "Using Standard FreqRat  f(2)^2 / f(1) * f(3) " << endl;
		preservationProb = pow(((double)timers[1]/lineages.size()), 2)/(((double)timers[0]/lineages.size()) * ((double)timers[2]/lineages.size()));
		//DEBUG
		cout << "Calculated Preservation Prob: " << preservationProb << endl;
		
		if(preservationProb < 1.0 && preservationProb > 0.0){
			cout << "Preservation Probability (FreqRat) f(2)^2 / f(1) * f(3) = R = " << preservationProb << endl;
		}
		else if(preservationProb >= 1.0){
		//if R is greater than 1, then fix it to 0.7, i.e. the fossil record is pretty good
			preservationProb = 0.1;
			//cout << "Preservation Probability (FreqRat) f(2)^2 / f(1) * f(3) = R = " << preservationProb << endl;
			cout << "Cannot calculate preservation probability for this data set!\nPreservation probability will be arbitrarily set to 0.1.\nPlease Restart the program and provide a suitable preservation rate estimate." << endl;
		}
		else { //i.e. it's negative
			preservationProb = 0.1;
			cout << "Cannot calculate preservation probability for this data set!\nPreservation probability will be arbitrarily set to 0.1.\nPlease Restart the program and provide a suitable preservation rate estimate." << endl;		
			}
	}
	else if(timers[1] > 1 >= timers[2]){ //1 or 0 3timers, so 3timers must be estimated by frequency of 2timers
		cout << "Frequency of One-Timers: " << (double)timers[0]/lineages.size() << endl;
		cout << "Frequency of Two-Timers: " << (double)timers[1]/lineages.size() << endl;
		cout << "Frequency of Three-Timers: " << (double)timers[2]/lineages.size() << endl;
		cout << "Using FreqRat to estimate 3timers f(2)^2 / f(1) * f'(3) " << endl;
		double fn2 = (double)timers[1]/lineages.size();
		//ERROR ASSUMPTION OF 10 MY BINS!!!!!
		double fn3 = fn2 - (1/meanDuration * meanBinLength);
		preservationProb = pow(fn2, 2)/(((double)timers[0]/lineages.size()) * fn3);
		cout << "Calculated Preservation Prob: " << preservationProb << endl;
		//if R is greater than 1, then fix it to 1, i.e. the fossil record is pretty good
		if(preservationProb < 1.0 && preservationProb > 0){
			cout << "Preservation Probability (FreqRat ESTIMATED) f(2)^2 / f(1) * f'(3) = R = " << preservationProb << endl;
		}
		else if(preservationProb >= 1.0){
			preservationProb = 0.1;
			cout << "Cannot calculate preservation probability for this data set!\nPreservation probability will be arbitrarily set to 0.1.\nPlease Restart the program and provide a suitable preservation rate estimate." << endl;
		}
		else {
			preservationProb = 0.1;
			cout << "Cannot calculate preservation probability for this data set!\nPreservation probability will be arbitrarily set to 0.1.\nPlease Restart the program and provide a suitable preservation rate estimate." << endl;		}
	}
	else if(timers[2] > 1 >= timers[1] || timers[2] > timers[1]){ //1 or 0 2timers, or less 2timers than 3 timers, so 2timers must be estimated
		cout << "Frequency of One-Timers: " << (double)timers[0]/lineages.size() << endl;
		cout << "Frequency of Two-Timers: " << (double)timers[1]/lineages.size() << endl;
		cout << "Frequency of Three-Timers: " << (double)timers[2]/lineages.size() << endl;
		cout << "Using FreqRat to estimate 2timers f'(2)^2 / f(1) * f(3) " << endl;
		double fn3 = (double)timers[2]/lineages.size();
		//ERROR!!! NO MORE 10 MY bins!!
		double fn2 = (1/meanDuration * meanBinLength) + fn3;
		preservationProb = pow(fn2, 2)/(((double)timers[0]/lineages.size()) * fn3);
		cout << "Calculated Preservation Prob: " << preservationProb << endl;
		
		if(preservationProb < 1.0 && preservationProb > 0){
			cout << "Preservation Probability (FreqRat ESTIMATED) f'(2)^2 / f(1) * f(3) = R = " << preservationProb << endl;
		}
		else if(preservationProb >= 1.0){
		//if R is greater than 1, then it is not estimable, fix it to 0.55 arbitrarily
			preservationProb = 0.1;
			cout << "Cannot calculate preservation probability for this data set!\nPreservation probability will be arbitrarily set to 0.1.\nPlease Restart the program and provide a suitable preservation rate estimate." << endl;
		}
		else {
			preservationProb = 0.1;
			cout << "Cannot calculate preservation probability for this data set!\nPreservation probability will be arbitrarily set to 0.1.\nPlease Restart the program and provide a suitable preservation rate estimate." << endl;
		}
	}
	//If 1 is not estimable, set it to 0.55, but tell the user that this may make for a less accurate estimate
	//also, in the future need to provide a method for the user to arbitrarily set the preservation rate manually
	else { 
		cout << "Not Using FreqRat" << endl;
		preservationProb = 0.1;
		cout << "Cannot calculate preservation probability for this data set!\nPreservation probability will be arbitrarily set to 0.1.\nPlease Restart the program and provide a suitable preservation rate estimate." << endl;
	}	
}


void NodeOfInterest::CalcPrior(){
	double observed_interval = timeIntervals.Base(firstBinOfNode) - (0.5 * timeIntervals.Length(firstBinOfNode));
	double current_diversity = 100;
	double diversity_at_first_obs = lineagesInFirstBin;
	double q = meanExtinctionRate;
	double p = meanOriginationRate;
	
	for(double missing_interval=0.0;missing_interval<200.0;missing_interval+=5.0){
		double delta1 = missing_interval * (p - q);
		double A1 = q * (exp(delta1) - 1.0) / (p * exp(delta1) - q);
		double B1 = p * A1 / q;
		
		cout << "t = " << missing_interval;
		
		double delta2 = observed_interval * (p - q);
		double A2 = q * (exp(delta2) - 1.0) / (p * exp(delta2) - q);
		double B2 = p * A2 / q;	
		
		double delta3 = (missing_interval + observed_interval) * (p - q);
		double A3 = q * (exp(delta3) - 1.0) / (p * exp(delta3) - q);
		double B3 = p * A3 / q;
		double total_interval_prob = (1.0-A3)*(1.0-B3)*pow(B3,(current_diversity - 1.0));

		double totalProb = 1.0;


		cout <<"\ttotal_int\t" << total_interval_prob << "\t";
		int i=diversity_at_first_obs;
		double missing_interval_prob = (1.0-A1)*(1.0-B1)*pow(B1,(i - 1.0));
		cout << "n = " << i << "\tmiss_prob\t" << missing_interval_prob << "\t";
		double sum = 0.0;
		for(int j=1;j<=i;j++){
			sum += choose(i, j) * choose(current_diversity - 1.0, j-1) * pow(A2, i-j) * pow(B2, current_diversity-j) * pow((1.0-A2)*(1.0-B2), j);		
		}
		double observed_interval_prob = sum;
		cout << "\tobs_prob\t" << observed_interval_prob << "\t";
		totalProb = (missing_interval_prob * observed_interval_prob) / total_interval_prob;
		cout << missing_interval+observed_interval << "\t" << totalProb << endl;
	}
}

double NodeOfInterest::ProbOfMissingIntervalGivenFirstAndCurrentDiversity(double missing_interval, int diversity_at_first_obs, int current_diversity){
	double observed_interval = timeIntervals.Base(firstBinOfNode) - (0.5 * timeIntervals.Length(firstBinOfNode));

	double q = meanExtinctionRate;
	double p = meanOriginationRate;
	
	double delta1 = missing_interval * (p - q);
	double A1 = q * (exp(delta1) - 1.0) / (p * exp(delta1) - q);
	double B1 = p * A1 / q;
	
	double delta2 = observed_interval * (p - q);
	double A2 = q * (exp(delta2) - 1.0) / (p * exp(delta2) - q);
	double B2 = p * A2 / q;	

	double missing_interval_prob = (1.0-A1)*(1.0-B1)*pow(B1,(diversity_at_first_obs - 1.0));
	
	double observed_interval_prob = 0.0;
	for(int j=1;j<=diversity_at_first_obs;j++){
		observed_interval_prob += choose(diversity_at_first_obs, j) * choose(current_diversity - 1.0, j-1) * pow(A2, diversity_at_first_obs-j) * pow(B2, current_diversity-j) * pow((1.0-A2)*(1.0-B2), j);		
	}

	double totalProb = (missing_interval_prob * observed_interval_prob);
	return totalProb;
}
