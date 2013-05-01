#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "occurrence.h"
#include "nodeOfInterest.h"
#include "timeinterval.h"
#include <curl/curl.h>

using namespace std;

//For simulation use the following:
#define POST
#undef USE_CURLLIB
#define DONT_QUERY
//Real Deal
//#define POST
//#define USE_CURLLIB
//#undef DONT_QUERY

TimeIntervalSet timeIntervals;

char file_to_write_to[100] = "\0";
bool DONE_RECEIVING_PAGE = false;

double fact(int i){
	double tot=1.0;
	for(int c=2;c<i+1;c++)
		tot *= c;
	return tot;
}

double choose(int i, int j){
	double tot = 1.0;
	
	if(i == j) tot = 1.0;
	else if(j == 1) tot = (double) i;
	else if(j == 0) tot = tot = 1.0;
	else{
		for(int k=(i-j)+1;k<i+1;k++){
			tot = (tot*k);
		}
		for(int k=2;k<j+1;k++)
			tot = (int) (tot/k);
		}
	
	return tot;
}

#ifdef USE_CURLLIB
size_t write_http_data(void *buffer, size_t size, size_t nmemb, void *userp){
	if(nmemb > 100) cout << "." << flush;

	DONE_RECEIVING_PAGE = false;

	char *my_buf = new char[size * nmemb + 1];

	memcpy(my_buf, buffer, size * nmemb);
	my_buf[size * nmemb] = '\0';

	if(strlen(file_to_write_to) > 0){
		ofstream out_file(file_to_write_to, ios::app);
		if(out_file.good() == false){
			cout << "Problem opening file " << file_to_write_to << " for writing." << endl;
			exit(1);
		}
		out_file << my_buf;
		out_file.close();
	}

	string my_string = my_buf;
	if(my_string.find("</html>") != string::npos){
		DONE_RECEIVING_PAGE = true;
		//cout << "found end of page" << endl;
		}

	delete []my_buf;
	return size * nmemb;
}

void WaitForPageEnd(){
	int c=0;
	while(DONE_RECEIVING_PAGE == false)
		if((c++ % 1000) == 0){
			cout << "." << flush;
		}
}

void GetQueryString(char *buf, string taxon_name){
	char temp[200];
	sprintf(temp, "action=displayDownloadResults&yourname=autoprior&output_data=occurrence+list&output_format=csv&taxon_name=%s&", taxon_name.c_str());
	strcat(buf, temp);
	//Changed Lump genera from NO to YES
	strcat(buf, "exclude_taxon_name=&max_interval_name=&min_interval_name=&time_scale=&binned_field=&authorizer_reversed=&research_group_restricted_to=0&research_group=&created_before_after=before&month=&day_of_month=&year=&published_before_after=after&pubyr=&lithification_lithified=YES&lithification_poorly_lithified=YES&lithification_unlithified=YES&lithification_unknown=YES&lithology_carbonate=YES&lithology_mixed=YES&lithology_siliciclastic=YES&lithology_unknown=YES&include_exclude_lithology1=include&lithology1=&environment_carbonate=YES&environment_siliciclastic=YES&environment_terrestrial=YES&environment_unknown=YES&include_exclude_environment=include&environment=&global=YES&include_exclude_country=include&country=&latmin1=-90&latmax1=90&latmin2=-90&latmax2=90&lngmin1=-180&lngmax1=180&lngmin2=-180&lngmax2=180&paleolatmin1=-90&paleolatmax1=90&paleolatmin2=-90&paleolatmax2=90&paleolngmin1=-180&paleolngmax1=180&paleolngmin2=-180&paleolngmax2=180&geogscale_hand_sample=YES&geogscale_small_collection=YES&geogscale_outcrop=YES&geogscale_local_area=YES&geogscale_basin=YES&geogscale_unknown=YES&stratscale_bed=YES&stratscale_group_of_beds=YES&stratscale_member=YES&stratscale_formation=YES&stratscale_group=YES&stratscale_unknown=YES&pres_mode_cast=YES&pres_mode_adpression=YES&pres_mode_mold%2Fimpression=YES&pres_mode_original+aragonite=YES&pres_mode_replaced+with+silica=YES&pres_mode_trace=YES&pres_mode_charcoalification=YES&pres_mode_coalified=YES&pres_mode_other=YES&collection_type_archaeological=YES&collection_type_biostratigraphic=YES&collection_type_paleoecologic=YES&collection_type_taphonomic=YES&collection_type_taxonomic=YES&collection_type_general_faunal%2Ffloral=YES&collection_type_unknown=YES&lump_by_strat_unit=&collection_no=&occurrence_count_qualifier=less&occurrence_count=&abundance_count_qualifier=less&abundance_count=&exclude_superset=NO&lump_genera=YES&replace_with_reid=YES&replace_with_ss=YES&");
	strcat(buf, "split_subgenera=NO&");
	strcat(buf, "indet=NO&");
	strcat(buf, "sp=YES&");
	strcat(buf, "genus_reso_aff.=NO&");
	strcat(buf, "genus_reso_cf.=NO&");
	strcat(buf, "genus_reso_ex+gr.=NO&");
	strcat(buf, "genus_reso_n.+gen.=NO&");
	strcat(buf, "genus_reso_sensu+lato=NO&");
	strcat(buf, "genus_reso_%3F=NO&");
	strcat(buf, "genus_reso_%22=NO&");
	strcat(buf, "informal=NO&preservation=regular+taxon&preservation=form+taxon&preservation=ichnofossil&compendium_ranges=YES&abundance_required=all&abundance_taxon_include=include&abundance_taxon_name=&incomplete_abundances=NO&classified=&min_mean_abundance=&collections_coords_format=decimal&collections_paleocoords_format=decimal&collections_stage=YES&collections_ma_max=YES&collections_ma_min=YES&ecology1=&ecology2=&ecology3=&ecology4=&ecology5=&ecology6=");
	//&collections_ma_min=YES Use this to get the min age of each collection
	//&collections_ma_max=YES Use this to get the max age of each collection
	//&collections_stage=YES Use this to get the stage names instead of PBDB 10My bins
	//&collections_10mybin=YES Removed because no longer using this 
}

int QueryPBDB(string taxon_name){
	char query_string[3000];
	query_string[0] = '\0';

#ifdef GET
	strcpy(query_string, "http://paleodb.org/cgi-bin/bridge.pl?");
#elif defined POST
	//nothing to do here
#else
	cout << "Error: communication method not defined!" << endl;
	exit(1);
#endif

	GetQueryString(query_string, taxon_name); 
	//get the http handle
	CURL *handle = curl_easy_init( );
	//set the function to handle the http data received
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_http_data);

#ifdef GET
	//with the GET method the entire query will be part of the URL
	curl_easy_setopt(handle, CURLOPT_URL, query_string);
#else //POST
	//with the POST method the url will be short, and the query will be in the page data
	curl_easy_setopt(handle, CURLOPT_URL, "http://paleodb.org/cgi-bin/bridge.pl");
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, query_string);
#endif

	cout << "Querying Paleobiology Database for taxon " << taxon_name << "..." << flush;

	curl_easy_perform(handle);
	
	//I'm not sure if the perform above is guaranteed to block, so wait here until flag is set
	//after finding the </html> tag in write_http_data
	WaitForPageEnd();
	cout << "done." << endl;	
	
	char buff[200];
	sprintf(buff, "http://paleodb.org/public/downloads/autoprior-occs.csv");
	curl_easy_setopt(handle, CURLOPT_URL, buff);
	sprintf(file_to_write_to, "%s.csv", taxon_name.c_str());

	curl_easy_setopt(handle, CURLOPT_POST, 0);
	cout << "Downloading fossil occurrence data..." << flush;
	curl_easy_perform(handle);
	cout << "done." << endl;	
	curl_easy_cleanup(handle);

	/*here is an example of the POST data sent to http://paleodb.org/cgi-bin/bridge.pl

	action=displayDownloadResults&
	yourname=&output_data=occurrence+list&output_format=csv&taxon_name=primates&
	exclude_taxon_name=&max_interval_name=&min_interval_name=&time_scale=&binned_field=&
	authorizer_reversed=&research_group_restricted_to=0&research_group=&created_before_after=before&
	month=&day_of_month=&year=&published_before_after=after&pubyr=&lithification_lithified=YES&
	lithification_poorly_lithified=YES&lithification_unlithified=YES&lithification_unknown=YES&
	lithology_carbonate=YES&lithology_mixed=YES&lithology_siliciclastic=YES&lithology_unknown=YES&
	include_exclude_lithology1=include&lithology1=&environment_carbonate=YES&environment_siliciclastic=YES&
	environment_terrestrial=YES&environment_unknown=YES&include_exclude_environment=include&environment=&
	global=YES&include_exclude_country=include&country=&latmin1=-90&latmax1=90&latmin2=-90&latmax2=90&
	lngmin1=-180&lngmax1=180&lngmin2=-180&lngmax2=180&paleolatmin1=-90&paleolatmax1=90&paleolatmin2=-90&
	paleolatmax2=90&paleolngmin1=-180&paleolngmax1=180&paleolngmin2=-180&paleolngmax2=180&geogscale_hand_sample=YES&
	geogscale_small_collection=YES&geogscale_outcrop=YES&geogscale_local_area=YES&geogscale_basin=YES&
	geogscale_unknown=YES&stratscale_bed=YES&stratscale_group_of_beds=YES&stratscale_member=YES&
	stratscale_formation=YES&stratscale_group=YES&stratscale_unknown=YES&pres_mode_cast=YES&pres_mode_adpression=YES&
	pres_mode_mold%2Fimpression=YES&pres_mode_original+aragonite=YES&pres_mode_replaced+with+silica=YES&
	pres_mode_trace=YES&pres_mode_charcoalification=YES&pres_mode_coalified=YES&pres_mode_other=YES&
	collection_type_archaeological=YES&collection_type_biostratigraphic=YES&collection_type_paleoecologic=YES&
	collection_type_taphonomic=YES&collection_type_taxonomic=YES&collection_type_general_faunal%2Ffloral=YES&
	collection_type_unknown=YES&lump_by_strat_unit=&collection_no=&occurrence_count_qualifier=less&occurrence_count=&
	abundance_count_qualifier=less&abundance_count=&exclude_superset=NO&lump_genera=NO&replace_with_reid=YES&
	replace_with_ss=YES&split_subgenera=NO&indet=NO&sp=YES&genus_reso_aff.=YES&genus_reso_cf.=YES&
	genus_reso_ex+gr.=YES&genus_reso_n.+gen.=YES&genus_reso_sensu+lato=YES&genus_reso_%3F=YES&
	genus_reso_%22=YES&informal=NO&preservation=regular+taxon&preservation=form+taxon&preservation=ichnofossil&
	compendium_ranges=YES&abundance_required=all&abundance_taxon_include=include&abundance_taxon_name=&
	incomplete_abundances=NO&classified=&min_mean_abundance=&collections_coords_format=decimal&
	collections_paleocoords_format=decimal&collections_10mybin=YES&ecology1=&ecology2=&ecology3=&ecology4=&ecology5=&ecology6=
	*/
	return 0;
}
#endif

//  gamma.cpp -- computation of gamma function.
//      Algorithms and coefficient values from "Computation of Special
//      Functions", Zhang and Jin, John Wiley and Sons, 1996.
//
//  (C) 2003, C. Bond. All rights reserved.
//
// Returns gamma function of argument 'x'.
//
// NOTE: Returns 1e308 if argument is a negative integer or 0,
//      or if argument exceeds 171.
//
double gamma(double x)
{
    int i,k,m;
    double ga,gr,r,z;

    static double g[] = {
        1.0,
        0.5772156649015329,
       -0.6558780715202538,
       -0.420026350340952e-1,
        0.1665386113822915,
       -0.421977345555443e-1,
       -0.9621971527877e-2,
        0.7218943246663e-2,
       -0.11651675918591e-2,
       -0.2152416741149e-3,
        0.1280502823882e-3,
       -0.201348547807e-4,
       -0.12504934821e-5,
        0.1133027232e-5,
       -0.2056338417e-6,
        0.6116095e-8,
        0.50020075e-8,
       -0.11812746e-8,
        0.1043427e-9,
        0.77823e-11,
       -0.36968e-11,
        0.51e-12,
       -0.206e-13,
       -0.54e-14,
        0.14e-14};

    if (x > 171.0) return 1e308;    // This value is an overflow flag.
    if (x == (int)x) {
        if (x > 0.0) {
            ga = 1.0;               // use factorial
            for (i=2;i<x;i++) {
               ga *= i;
            }
         }
         else
            ga = 1e308;
     }
     else {
        if (fabs(x) > 1.0) {
            z = fabs(x);
            m = (int)z;
            r = 1.0;
            for (k=1;k<=m;k++) {
                r *= (z-k);
            }
            z -= m;
        }
        else
            z = x;
        gr = g[24];
        for (k=23;k>=0;k--) {
            gr = gr*z+g[k];
        }
        ga = 1.0/(gr*z);
        if (fabs(x) > 1.0) {
            ga *= r;
            if (x < 0.0) {
                ga = -M_PI/(x*ga*sin(M_PI*x));
            }
        }
    }
    return ga;
}

/* For fitting to a log-normal distribution
void FillLogNormalDist(double mean, double sd, int samples, vector<double> &dist){

	if(dist.size() > 0) dist.clear();
	double sqrt2pi = sqrt(2.0 * 3.14159);

	for(int i=0;i<samples;i++){
		if(i > 0)
			dist.push_back( exp(-1.0*pow((log(i)-mean), 2.0)/(2.0*sd*sd)) / (((double)i)*sd*sqrt2pi));
		else
			dist.push_back( exp(-1.0*pow((log(1.0e-4)-mean), 2.0)/(2.0*sd*sd)) / (1.0e-4*sd*sqrt2pi));
	}
}
*/

void FillGammaDist(double shape, double rate, int samples, vector<double> &dist){ 
	//alpha = shape, beta = rate, or inverse scale
	if(dist.size() > 0) dist.clear();
	
	for(int i=0;i<samples;i++){
		if(i > 0)
			dist.push_back(pow(i, (shape - 1))*(pow(rate, shape)*exp((0.0 - rate) * i)) / gamma(shape));
		else
			dist.push_back(pow(1.0e-4, (shape - 1))*(pow(rate, shape)*exp((0.0 - rate) * 1.0e-4))/gamma(shape));
	}
}

/* For fitting to a Log-Normal distribution.
double DistributionLogExpectation(vector<double> &dist){
	double weightedTot = 0.0;
	double tot = 0.0;
	for(int i=0;i<dist.size();i++){
		if(i > 0)
			weightedTot += dist[i]*log(i);
		else 
			weightedTot += dist[i]*log(1.0e-4);
		tot += dist[i];
		}
		
	return weightedTot / tot;
}

double DistributionLogStddev(vector<double> &dist, double mean){
	double weightedTot = 0.0;
	double tot = 0.0;
	for(int i=0;i<dist.size();i++){
		if(i > 0)
			weightedTot += pow((mean - log(i)), 2)*dist[i];
		else 
			weightedTot += pow((mean - log(1.0e-4)), 2)*dist[i];
		tot += dist[i];
	}
	return weightedTot / tot;
}
*/

double LeastSquareDistance(vector<double> &targ, vector<double> &cand){
	assert(targ.size() == cand.size());
	
	double targTot = 0.0;
	double candTot = 0.0;
	
	for(int i=0;i<targ.size();i++){
		targTot += targ[i];
		candTot += cand[i];
		}
	
	double result = 0.0;
	for(int i=0;i<targ.size();i++)
		result += pow(targ[i]*(candTot/targTot) - cand[i], 2);
		
	return result;
}

int main (int argc, char * const argv[]) {
	string taxon_name;
	string freqRatYesNo;
	int currentDiversity;
	int freqRatYN = 0;
	double R;
	double userPresRate;
	char occurrence_file[100];
	bool batch_mode = false;

	//Batchmode check
	if(argc > 1){
		strcpy(occurrence_file, argv[1]);
		cout << occurrence_file << endl;
		batch_mode = true;
		if(argc > 2){
			currentDiversity = atoi(argv[2]);
			cout << "Extant diversity of clade provided by user: " << currentDiversity << endl;
			userPresRate = atof(argv[3]);
			}
		}
	else{
		cout << "please enter the name of your taxon or clade of interest: ";
		cin >> taxon_name;
		cout << endl;
		
		cout << "please enter your estimate of the number of extant genera: ";
		cin >> currentDiversity;
		//currentDiversity = 13;
		
		sprintf(occurrence_file, "%s.csv", taxon_name.c_str());
		
		#ifndef DONT_QUERY
		//clear the occurrence file if it exists
		ofstream dum(occurrence_file);
		dum.close();
		QueryPBDB(taxon_name);
		#endif
	}
	
	ifstream input(occurrence_file);

	if(input.good() == false){
		cout << "Sorry, could not open file!" << endl;
		exit(1);
	}

	//get all of the fossil occurrences out of the file and into a vector
	vector<Occurrence> allOccs;
	Occurrence myOcc;
	while(input.good() && ! input.eof()){
		int err = myOcc.Read(input);
		if(!err) allOccs.push_back(myOcc);
	}
	
	//sort all of the Occurrence objects in the vector using the overloaded < operator, 
	//which orders based on taxon name
	cout << "Number of occurrences read: " << allOccs.size() << endl;

	vector<double> targetDist, candidateDist;

	if(allOccs.size() > 0){
		sort(allOccs.begin(), allOccs.end());
		NodeOfInterest myNode;
		myNode.AssignOccurrencesToLineages(allOccs);
		myNode.OutputLineages();
	//if statement to check for data set editing, not currently working	
	//	if(batch_mode = false){
	//		myNode.EditDataSet();
	//	}
		
		myNode.CalcPerBinR8s();
	//If statement to check for batch mode and turn on/off preservation rate input or FreqRat loop
		if(batch_mode == true){
			R = userPresRate/100;
			cout << "User supplied preservation rate estimate: " << R << endl;
		}
		else {
			do{	
				cout << "Would you like to use the FreqRat method to estimate the preservation rate (R) of this clade? Choose option 1 or 2." << endl;
				cout << "1. Yes, estimate the preservation rate with FreqRat" << endl << "2. No, you would rather provide a specific preservation rate estimate." << endl;
				cin >> freqRatYN;
				cout << "You entered "<< freqRatYN << endl;
				if(freqRatYN == 1){
					myNode.CalcPreservationProb();
					R = myNode.PreservationProbability();
					cout << "FreqRat Estimated Preservation Probability: " << R << endl;
					break;
				}
				else if(freqRatYN == 2){
					cout << "Please provide an estimate of the preservation probability (i.e. 0 < R < 1) for your clade of interest: ";
					cin >> R;
					cout << endl;
					cout << "User Provided Preservation Probability: " << R << endl;
					break;
				}
				else{
					cout << "Not a valid entry, please select option 1 or 2 by entering the appropriate integer." << endl;
				}
			}while(freqRatYN != 1||2);		
		}
		
		
		int firstObserved = myNode.LineagesInFirstBin();
		double maxLike = 0.0;
		for(double missing=0.0;missing <= 9999;missing +=1){
			double totalProb = 0.0;
			//DEBUG
			if(R < 1.0 && R > 0.0){
				for(int i=firstObserved;i<min(firstObserved+50, currentDiversity);i++){
					double probPres = choose(i, firstObserved) * pow(R, firstObserved) * pow((1-R),(i-firstObserved));
					if(missing > 0.0)
						totalProb += probPres * myNode.ProbOfMissingIntervalGivenFirstAndCurrentDiversity(missing, i, currentDiversity);
					else
						totalProb += probPres * myNode.ProbOfMissingIntervalGivenFirstAndCurrentDiversity(1.0e-4, i, currentDiversity);
				}
			}
			else {
				totalProb =  myNode.ProbOfMissingIntervalGivenFirstAndCurrentDiversity(missing, firstObserved, currentDiversity);
			}
			targetDist.push_back(totalProb);
			if(totalProb > maxLike)
				maxLike = totalProb;
			if(totalProb < maxLike/100.0) break; 
		}
		
		cout << "Likelihood calculated.  Fitting prior distribution...." << endl;

/*DEBUG
		FILE *out2 = fopen("targetdist.log", "w");
		for(int i=0;i<targetDist.size();i++)
			fprintf(out2, "missing int=\t%d\t%1.10f\n", i, abs(log(targetDist[i])));  
		fclose(out2);	
*/		
		
		
/* Code for fitting to a lognormal distribution		
		double totalImprove = 0.1;
		double currentMean = DistributionLogExpectation(targetDist);
		double currentStddev = DistributionLogStddev(targetDist, currentMean);
		FillLogNormalDist(currentMean, currentStddev, targetDist.size(), candidateDist);
		double currentDist = LeastSquareDistance(targetDist, candidateDist);
		do{
			double loopStartDist = currentDist;
			double meanToTry = currentMean * ( 1.0 / 1.001); //try a slightly smaller mean
			FillLogNormalDist(meanToTry, currentStddev, targetDist.size(), candidateDist);
			double distWithSmallerMean = LeastSquareDistance(targetDist, candidateDist);
			
			meanToTry = currentMean * (1.001); //try a slightly larger mean
			FillLogNormalDist(meanToTry, currentStddev, targetDist.size(), candidateDist);
			double distWithLargerMean = LeastSquareDistance(targetDist, candidateDist);
			
			if(distWithSmallerMean < currentDist && distWithLargerMean > currentDist){ 
				//move to the slightly smaller mean that we already scored
				double improvement = currentDist - distWithSmallerMean;
				currentMean = currentMean * ( 1.0 / 1.001);
				FillLogNormalDist(currentMean, currentStddev, targetDist.size(), candidateDist); 
				currentDist = distWithSmallerMean;
				do{
					meanToTry = currentMean * ( 1.0 / 1.001); //try a slightly smaller mean
					FillLogNormalDist(meanToTry, currentStddev, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentMean = meanToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			else if(distWithLargerMean < currentDist && distWithSmallerMean > currentDist){
				//move to the slightly larger mean that we already scored
				double improvement = currentDist - distWithLargerMean;
				currentMean = currentMean * (1.001);
				FillLogNormalDist(currentMean, currentStddev, targetDist.size(), candidateDist); 
				currentDist = distWithLargerMean;
				do{
					meanToTry = currentMean * (1.001); //try a slightly larger mean
					FillLogNormalDist(meanToTry, currentStddev, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentMean = meanToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			FillLogNormalDist(currentMean, currentStddev, targetDist.size(), candidateDist);
			currentDist = LeastSquareDistance(targetDist, candidateDist);
			
			totalImprove = loopStartDist - currentDist;
		}while(totalImprove > 0.0);
		
		//Now fit the stddev
		do{
			double loopStartDist = currentDist;
			double stddevToTry = currentStddev * ( 1.0 / 1.001); //try a slightly smaller stddev
			FillLogNormalDist(currentMean, stddevToTry, targetDist.size(), candidateDist);
			double distWithSmallerStddev = LeastSquareDistance(targetDist, candidateDist);
			
			stddevToTry = currentStddev * (1.001); //try a slightly larger stddev
			FillLogNormalDist(currentMean, stddevToTry, targetDist.size(), candidateDist);
			double distWithLargerStddev = LeastSquareDistance(targetDist, candidateDist);
			
			if(distWithSmallerStddev < currentDist && distWithLargerStddev > currentDist){
				//move to the slightly smaller stddev that we already scored
				double improvement = currentDist - distWithSmallerStddev;
				currentStddev = currentStddev * ( 1.0 / 1.001);
				FillLogNormalDist(currentMean, currentStddev, targetDist.size(), candidateDist); 
				currentDist = distWithSmallerStddev;
				do{
					stddevToTry = currentStddev * ( 1.0 / 1.001); //try a slightly smaller stddev
					FillLogNormalDist(currentMean, stddevToTry, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentStddev = stddevToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			else if(distWithLargerStddev < currentDist && distWithSmallerStddev > currentDist){
				//move to the slightly larger stddev that we already scored
				double improvement = currentDist - distWithLargerStddev;
				currentStddev = currentStddev * (1.001);
				FillLogNormalDist(currentMean, currentStddev, targetDist.size(), candidateDist); 
				currentDist = distWithLargerStddev;
				do{
					stddevToTry = currentStddev * (1.001); //try a slightly larger mean
					FillLogNormalDist(currentMean, stddevToTry, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentStddev = stddevToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			FillLogNormalDist(currentMean, currentStddev, targetDist.size(), candidateDist);
			currentDist = LeastSquareDistance(targetDist, candidateDist);
			
			totalImprove = loopStartDist - currentDist;
		}while(totalImprove > 0.0);
		
		cout << "Target Distribution Mean\tTarget Distribution Stddev" << endl;
		printf("%2.4f\t\t\t%2.4f\n", DistributionLogExpectation(targetDist), DistributionLogStddev(targetDist, currentMean));
		cout << "Best-Fit Lognormal Distribution" << endl;
		cout << "Mean\tStandard Deviation" << endl;
		printf("%2.4f\t\t\t%2.4f\n", currentMean, currentStddev);
		cout << "Zero Offset: " << timeIntervals.Base(myNode.FirstBinOfNode()) << endl;
*/
		
//First Fit the shape parameter
		double totalImprove = 0.1;
		double currentShape = 1.0;
		double currentRate = 0.5;
		FillGammaDist(currentShape, currentRate, targetDist.size(), candidateDist);
		double currentDist = LeastSquareDistance(targetDist, candidateDist);
		do{
			double loopStartDist = currentDist;
			double shapeToTry = currentShape * ( 1.0 / 1.001); //try a slightly smaller shape
			FillGammaDist(shapeToTry, currentRate, targetDist.size(), candidateDist);
			double distWithSmallerShape = LeastSquareDistance(targetDist, candidateDist);
			
			shapeToTry = currentShape * (1.001); //try a slightly larger shape
			FillGammaDist(shapeToTry, currentRate, targetDist.size(), candidateDist);
			double distWithLargerShape = LeastSquareDistance(targetDist, candidateDist);
			
			if(distWithSmallerShape < currentDist && distWithLargerShape > currentDist){ 
				//move to the slightly smaller shape that we already scored
				double improvement = currentDist - distWithSmallerShape;
				currentShape = currentShape * ( 1.0 / 1.001);
				FillGammaDist(currentShape, currentRate, targetDist.size(), candidateDist); 
				currentDist = distWithSmallerShape;
				do{
					shapeToTry = currentShape * ( 1.0 / 1.001); //try a slightly smaller shape
					FillGammaDist(shapeToTry, currentRate, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentShape = shapeToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			else if(distWithLargerShape < currentDist && distWithSmallerShape > currentDist){
				//move to the slightly larger shape that we already scored
				double improvement = currentDist - distWithLargerShape;
				currentShape = currentShape * (1.001);
				FillGammaDist(currentShape, currentRate, targetDist.size(), candidateDist); 
				currentDist = distWithLargerShape;
				do{
					shapeToTry = currentShape * (1.001); //try a slightly larger shape
					FillGammaDist(shapeToTry, currentRate, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentShape = shapeToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			FillGammaDist(currentShape, currentRate, targetDist.size(), candidateDist);
			currentDist = LeastSquareDistance(targetDist, candidateDist);
			
			totalImprove = loopStartDist - currentDist;
		
		
//Now fit the Rate
		//do{
			//double loopStartDist = currentDist;
			double rateToTry = currentRate * ( 1.0 / 1.001); //try a slightly smaller Rate
			FillGammaDist(currentShape, rateToTry, targetDist.size(), candidateDist);
			double distWithSmallerRate = LeastSquareDistance(targetDist, candidateDist);
			
			rateToTry = currentRate * (1.001); //try a slightly larger Rate
			FillGammaDist(currentShape, rateToTry, targetDist.size(), candidateDist);
			double distWithLargerRate = LeastSquareDistance(targetDist, candidateDist);
			
			if(distWithSmallerRate < currentDist && distWithLargerRate > currentDist){
				//move to the slightly smaller Rate that we already scored
				double improvement = currentDist - distWithSmallerRate;
				currentRate = currentRate * ( 1.0 / 1.001);
				FillGammaDist(currentShape, currentRate, targetDist.size(), candidateDist); 
				currentDist = distWithSmallerRate;
				do{
					rateToTry = currentRate * ( 1.0 / 1.001); //try a slightly smaller Rate
					FillGammaDist(currentShape, rateToTry, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentRate = rateToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			else if(distWithLargerRate < currentDist && distWithSmallerRate > currentDist){
				//move to the slightly larger Rate that we already scored
				double improvement = currentDist - distWithLargerRate;
				currentRate = currentRate * (1.001);
				FillGammaDist(currentShape, currentRate, targetDist.size(), candidateDist); 
				currentDist = distWithLargerRate;
				do{
					rateToTry = currentRate * (1.001); //try a slightly larger mean
					FillGammaDist(currentShape, rateToTry, targetDist.size(), candidateDist);
					double proposedDist =  LeastSquareDistance(targetDist, candidateDist);
					improvement = currentDist - proposedDist;
					if(improvement > 0.0){
						currentRate = rateToTry;
						currentDist = proposedDist;
					}
				}while(improvement > 0.0);
			}
			FillGammaDist(currentShape, currentRate, targetDist.size(), candidateDist);
			currentDist = LeastSquareDistance(targetDist, candidateDist);
			
			totalImprove = loopStartDist - currentDist;
		}while(totalImprove > 0.0);
		
		cout << "Target Distribution Shape\tTarget Distribution Rate" << endl;
		//printf("%2.4f\t\t\t%2.4f\n", DistributionLogExpectation(targetDist), DistributionLogRate(targetDist, currentShape));
		cout << "Best-Fit Gamma Distribution" << endl;
		cout << "Shape\t\t\tScale" << endl;
		printf("%2.4f\t\t\t%2.4f\n", currentShape, 1/currentRate);
		//Changing zero offset value to be the top of the first bin, or base of the second to last interval
		double zeroOffset = timeIntervals.Base(myNode.FirstBinOfNode()) - timeIntervals.Length(myNode.FirstBinOfNode());
		cout << "Zero Offset: " << zeroOffset << endl;
		cout << "Discretized likelihood curve and best-fit gamma distribution printed to likes.log file." << endl;
		

		FILE *out1 = fopen("likes.log", "w");
		for(int i=0;i<targetDist.size();i++)
			fprintf(out1, "missing int=\t%d\t%1.20f\t%1.10f\n", i, targetDist[i], candidateDist[i]);  
			fclose(out1);	
	

		FILE *out = fopen("SNAPEStats.log", "a");
		fprintf(out, "%2.5f\t%2.5f\t%2.5f\t%2.5f\t%2.5f\t%2.5f\t%i\n", myNode.OriginationRate(), myNode.ExtinctionRate(), R, zeroOffset, currentShape, 1/currentRate, currentDiversity);  
		fclose(out);
		
		
	}
	else{
		cout << "Sorry, problem reading occurrences!" << endl;
	}
	
    return 0;
}



