#ifndef TIME_INTERVAL
#define TIME_INTERVAL

#include <cassert>

class TimeInterval{
	string name;
	double base;
	double len;
	public:
	TimeInterval(){};
	TimeInterval(const char *n, double b, double l){
		name = n;
		base = b;
		len = l;
	}
	const string &Name(){return name;}
	double Base() {return base;}
	double Length() {return len;}
};

class TimeIntervalSet{
	TimeInterval intervals[85];
	int numIntervals;
	public:
	TimeIntervalSet(){

/* This file can be edited to choose a different time scale. 
	Currently we have available the PBDB 10 MY bins, and the Gradstein Stages
	Simply comment out the currently implemented time scale and uncomment the one you want to use.
	Alternatively, you can write new time scales and put them in this file.
	Changes to this file require the software to be compiled once again.
*/
	//Time scale: PBDB 10 MY bins
/*
		numIntervals = 49;
		intervals[0] = TimeInterval("\"Cenozoic 6\"", 11.6, 11.6);
		intervals[1] = TimeInterval("\"Cenozoic 5\"", 23, 11.4);
		intervals[2] = TimeInterval("\"Cenozoic 4\"", 33.9, 10.9);
		intervals[3] = TimeInterval("\"Cenozoic 3\"", 40.4, 6.5);
		intervals[4] = TimeInterval("\"Cenozoic 2\"", 55.8, 15.4);
		intervals[5] = TimeInterval("\"Cenozoic 1\"", 65.5, 9.7);
		intervals[6] = TimeInterval("\"Cretaceous 8\"", 70.6, 5.1);
		intervals[7] = TimeInterval("\"Cretaceous 7\"", 83.5, 12.9);
		intervals[8] = TimeInterval("\"Cretaceous 6\"", 93.5, 10);
		intervals[9] = TimeInterval("\"Cretaceous 5\"", 99.6, 6.1);
		intervals[10] = TimeInterval("\"Cretaceous 4\"", 112, 12.4);
		intervals[11] = TimeInterval("\"Cretaceous 3\"", 125, 13);
		intervals[12] = TimeInterval("\"Cretaceous 2\"", 136.4, 11.4);
		intervals[13] = TimeInterval("\"Cretaceous 1\"", 145.5, 9.1);
		intervals[14] = TimeInterval("\"Jurassic 6\"", 150.8, 5.3);
		intervals[15] = TimeInterval("\"Jurassic 5\"", 164.7, 13.9);
		intervals[16] = TimeInterval("\"Jurassic 4\"", 171.6, 6.9);
		intervals[17] = TimeInterval("\"Jurassic 3\"", 183, 11.4);
		intervals[18] = TimeInterval("\"Jurassic 2\"", 189.6, 6.6);
		intervals[19] = TimeInterval("\"Jurassic 1\"", 199.6, 10);
		intervals[20] = TimeInterval("\"Triassic 4\"", 216.5, 16.9);
		intervals[21] = TimeInterval("\"Triassic 3\"", 228, 11.5);
		intervals[22] = TimeInterval("\"Triassic 2\"", 245, 17);
		intervals[23] = TimeInterval("\"Triassic 1\"", 251, 6);
		intervals[24] = TimeInterval("\"Permian 4\"", 260.4, 9.4);
		intervals[25] = TimeInterval("\"Permian 3\"", 270.6, 10.2);
		intervals[26] = TimeInterval("\"Permian 2\"", 284.4, 13.8);
		intervals[27] = TimeInterval("\"Permian 1\"", 299, 14.6);
		intervals[28] = TimeInterval("\"Carboniferous 5\"", 306.5, 7.5);
		intervals[29] = TimeInterval("\"Carboniferous 4\"", 318.1, 11.6);
		intervals[30] = TimeInterval("\"Carboniferous 3\"", 336, 17.9);
		intervals[31] = TimeInterval("\"Carboniferous 2\"", 349.5, 13.5);
		intervals[32] = TimeInterval("\"Carboniferous 1\"", 360.7, 11.2);
		intervals[33] = TimeInterval("\"Devonian 5\"", 376.1, 15.4);
		intervals[34] = TimeInterval("\"Devonian 4\"", 383.7, 7.6);
		intervals[35] = TimeInterval("\"Devonian 3\"", 391.9, 8.2);
		intervals[36] = TimeInterval("\"Devonian 2\"", 409.1, 17.2);
		intervals[37] = TimeInterval("\"Devonian 1\"", 418.1, 9);
		intervals[38] = TimeInterval("\"Silurian 2\"", 428.2, 10.1);
		intervals[39] = TimeInterval("\"Silurian 1\"", 443.7, 15.5);
		intervals[40] = TimeInterval("\"Ordovician 5\"", 449.5, 5.8);
		intervals[41] = TimeInterval("\"Ordovician 4\"", 460.5, 11);
		intervals[42] = TimeInterval("\"Ordovician 3\"", 466, 5.5);
		intervals[43] = TimeInterval("\"Ordovician 2\"", 479, 13);
		intervals[44] = TimeInterval("\"Ordovician 1\"", 490, 11);
		intervals[45] = TimeInterval("\"Cambrian 4\"", 501, 11);
		intervals[46] = TimeInterval("\"Cambrian 3\"", 513, 12);
		intervals[47] = TimeInterval("\"Cambrian 2\"", 530, 17);
		intervals[48] = TimeInterval("\"Cambrian 1\"", 542, 12);
*/	
		
		
		//Time scale: Gradstein Stages
		numIntervals = 85;
		intervals[0] = TimeInterval("Gelasian", 2.59, 2.59);
		intervals[1] = TimeInterval("Piacenzian", 3.6, 1.01);
		intervals[2] = TimeInterval("Zanclean", 5.33, 1.73);
		intervals[3] = TimeInterval("Messinian", 7.25, 1.92);
		intervals[4] = TimeInterval("Tortonian", 11.61, 4.36);
		intervals[5] = TimeInterval("Serravallian", 13.65, 2.04);
		intervals[6] = TimeInterval("Langhian", 15.97, 2.32);
		intervals[7] = TimeInterval("Burdigalian", 20.43, 4.46);
		intervals[8] = TimeInterval("Aquitanian", 23.03, 2.6);
		intervals[9] = TimeInterval("Chattian", 28.4, 5.37);
		intervals[10] = TimeInterval("Rupelian", 33.9, 5.5);
		intervals[11] = TimeInterval("Priabonian", 37.2, 3.3);
		intervals[12] = TimeInterval("Bartonian", 40.4, 3.2);
		intervals[13] = TimeInterval("Lutetian", 48.6, 8.2);
		intervals[14] = TimeInterval("Ypresian", 55.8, 7.2);
		intervals[15] = TimeInterval("Thanetian", 58.7, 2.9);
		intervals[16] = TimeInterval("Selandian", 61.7, 3);
		intervals[17] = TimeInterval("Danian", 65.5, 3.8);
		intervals[18] = TimeInterval("Maastrichtian", 70.6, 5.1);
		intervals[19] = TimeInterval("Campanian", 83.5, 12.9);
		intervals[20] = TimeInterval("Santonian", 85.8, 2.3);
		intervals[21] = TimeInterval("Coniacian", 89.3, 3.5);
		intervals[22] = TimeInterval("Turonian", 93.5, 4.2);
		intervals[23] = TimeInterval("Cenomanian", 99.6, 6.1);
		intervals[24] = TimeInterval("Albian", 112, 12.4);
		intervals[25] = TimeInterval("Aptian", 125, 13);
		intervals[26] = TimeInterval("Barremian", 130, 5);
		intervals[27] = TimeInterval("Hauterivian", 136.4, 6.4);
		intervals[28] = TimeInterval("Valanginian", 140.2, 3.8);
		intervals[29] = TimeInterval("Berriasian", 145.5, 5.3);
		intervals[30] = TimeInterval("Tithonian", 150.8, 5.3);
		intervals[31] = TimeInterval("Kimmeridgian", 155.7, 4.9);
		intervals[32] = TimeInterval("Oxfordian", 161.2, 5.5);
		intervals[33] = TimeInterval("Callovian", 164.7, 3.5);
		intervals[34] = TimeInterval("Bathonian", 167.7, 3);
		intervals[35] = TimeInterval("Bajocian", 171.6, 3.9);
		intervals[36] = TimeInterval("Aalenian", 175.6, 4);
		intervals[37] = TimeInterval("Toarcian", 183, 7.4);
		intervals[38] = TimeInterval("Pliensbachian", 189.6, 6.6);
		intervals[39] = TimeInterval("Sinemurian", 196.5, 6.9);
		intervals[40] = TimeInterval("Hettangian", 199.6, 3.1);
		intervals[41] = TimeInterval("Rhaetian", 203.6, 4);
		intervals[42] = TimeInterval("Norian", 216.5, 12.9);
		intervals[43] = TimeInterval("Carnian", 228, 11.5);
		intervals[44] = TimeInterval("Ladinian", 237, 9);
		intervals[45] = TimeInterval("Anisian", 245, 8);
		intervals[46] = TimeInterval("Olenekian", 249.7, 4.7);
		intervals[47] = TimeInterval("Induan", 251, 1.3);
		intervals[48] = TimeInterval("Changhsingian", 253.8, 2.8);
		intervals[49] = TimeInterval("Wuchiapingian", 260.4, 6.6);
		intervals[50] = TimeInterval("Capitanian", 265.8, 5.4);
		intervals[51] = TimeInterval("Wordian", 268, 2.2);
		intervals[52] = TimeInterval("Roadian", 270.6, 2.6);
		intervals[53] = TimeInterval("Kungurian", 275.6, 5);
		intervals[54] = TimeInterval("Artinskian", 284.4, 8.8);
		intervals[55] = TimeInterval("Sakmarian", 294.6, 10.2);
		intervals[56] = TimeInterval("Asselian", 299, 4.4);
		intervals[57] = TimeInterval("Gzhelian", 303.9, 4.9);
		intervals[58] = TimeInterval("Kasimovian", 306.5, 2.6);
		intervals[59] = TimeInterval("Moscovian", 311.7, 5.2);
		intervals[60] = TimeInterval("Bashkirian", 318.1, 6.4);
		intervals[61] = TimeInterval("Serpukhovian", 326.4, 8.3);
		intervals[62] = TimeInterval("Visean", 345.3, 18.9);
		intervals[63] = TimeInterval("Tournaisian", 359.2, 13.9);
		intervals[64] = TimeInterval("Famennian", 374.5, 15.3);
		intervals[65] = TimeInterval("Frasnian", 385.3, 10.8);
		intervals[66] = TimeInterval("Givetian", 391.8, 6.5);
		intervals[67] = TimeInterval("Eifelian", 397.5, 5.7);
		intervals[68] = TimeInterval("Emsian", 407, 9.5);
		intervals[69] = TimeInterval("Pragian", 411.2, 4.2);
		intervals[70] = TimeInterval("Lochkovian", 416, 4.8);
		intervals[71] = TimeInterval("unnamed Pridoli stage", 418.7, 2.7);
		intervals[72] = TimeInterval("Ludfordian", 421.3, 2.6);
		intervals[73] = TimeInterval("Gorstian", 422.9, 1.6);
		intervals[74] = TimeInterval("Homerian", 426.2, 3.3);
		intervals[75] = TimeInterval("Sheinwoodian", 428.2, 2);
		intervals[76] = TimeInterval("Telychian", 436, 7.8);
		intervals[77] = TimeInterval("Aeronian", 439, 3);
		intervals[78] = TimeInterval("Rhuddanian", 443.7, 4.7);
		intervals[79] = TimeInterval("Hirnantian", 445.6, 1.9);
		intervals[80] = TimeInterval("middle Late Ordovician", 455.8, 10.2);
		intervals[81] = TimeInterval("early Late Ordovician", 460.9, 5.1);
		intervals[82] = TimeInterval("Darriwilian", 468.1, 7.2);
		intervals[83] = TimeInterval("early Middle Ordovician", 471.8, 3.7);
		intervals[84] = TimeInterval("late Early Ordovician", 478.6, 6.8);
 
	}
	int GetIntervalNum(string &n){
		int num=0;
		while(num < numIntervals && (n == intervals[num].Name()) == false) num++;
		if(num < numIntervals) return num;
		else return -1;
	}
	int GetIntervalNum(const char *n){
		int num=0;
		while(num < numIntervals && (n == intervals[num].Name()) == false) num++;
		if(num < numIntervals) return num;
		else return -1;
	}
	const string &Name(int n){
		assert(n > -1 && n < numIntervals);
		return intervals[n].Name();
	}
	double Base(int n){
		assert(n > -1 && n < numIntervals);
		return intervals[n].Base();
	}
	double Length(int n){
		assert(n > -1 && n < numIntervals);
		return intervals[n].Length();
	}
};	

#endif