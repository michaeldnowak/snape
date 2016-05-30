#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "occurrence.h"
#include "parser.h"
#include "timeinterval.h"
#include <cstdlib>
using namespace std;

extern TimeIntervalSet timeIntervals;

int Occurrence::Read(ifstream & input){
	Parser myParser(',');
	string line;
	char buf[1000];
		
	input.getline(buf, 1000);
	line = buf;
	int err = myParser.Parse(line, 8);

	if(strcmp(myParser.GetChunk(0), "collection_no") == 0){
		//toss the first line if it contains the column headings
		myParser.Empty();
		input.getline(buf, 1000);
		line = buf;
		err = myParser.Parse(line, 8);	
		//number of expected chunks changed from 6 to 8 to accommodate max and min ages of each occurrence
	}
	
	if(err == 0){
		const char *thisChunk = myParser.GetChunk(0);
			
		if(isdigit(thisChunk[0])){
			collectionNum = atoi(thisChunk);
			thisChunk = myParser.GetChunk(1);//second chunk, occurrences.genus_reso
			if(strlen(thisChunk) == 0){
				//this is a good entry, with a true genus resolution *hopefully
				thisChunk = myParser.GetChunk(2);//third chunk, occurrences.genus_name
				taxonAssignment = thisChunk;
				truGenus = true;
			}
			else{
				truGenus = false;
				thisChunk = myParser.GetChunk(2);//third chunk, occurrences.genus_name
				if(strlen(thisChunk) > 0){
					taxonAssignment = thisChunk;
				}
			}
		}
		else{
			thisChunk = myParser.GetChunk(3);//fourth, original.genus_reso
			//assert(strlen(thisChunk) > 0);
			taxonAssignment = thisChunk;
		}
		thisChunk = myParser.GetChunk(5); //sixth, collections_stage
		if(strlen(thisChunk) == 0){
			//This occurrence has no collection stage data
			periodPresent = false;
		}
		else{
			//This occurrence has viable collection period data
			periodPresent = true;
			stageNum = timeIntervals.GetIntervalNum(thisChunk);
		}
		thisChunk = myParser.GetChunk(6); //seventh, collections_ma_max
		if(isdigit(thisChunk[0])){
			//This occurrence has a viable maximum collection age
			occMax = atof(thisChunk);
			maxOccAge = true;
		}
		else{
			//This occurrence does not have a viable maximum collection age
			maxOccAge = false;
		}
		thisChunk = myParser.GetChunk(7); //eigth, collections_ma_min
		if(isdigit(thisChunk[0])){
			//This occurrence has a viable minimum collection age
			occMin = atof(thisChunk);
			minOccAge = true;
		}
		else{
			//This occurrence does not have a viable minimum collection age
			minOccAge = false;
		}
	}
	else{
		if(input.eof() == false){
			cout << "Number of chunks is not equal to expected number"  << endl;
		}
	}
	myParser.Empty();
	return err;
}
void Occurrence::Output(ostream &out){
	out << "Collection num: " << collectionNum << "\t";
	out << "Taxon name: " << taxonAssignment << "\t";
	if(!truGenus) out << "Note: not a true genus" << "\t";
	out << "Time interval from PBDB: " << timeIntervals.Name(stageNum) << "\n";
}
