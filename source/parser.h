#ifndef PARSER
#define PARSER

#include <string>
#include <vector>
using namespace std;

class Parser{
	vector<string *> chunks; 
	char delimiter;
	public:  
	Parser(char);
	~Parser();
	int Parse(string &, int); 
	void Empty();
	int NumChunks() const;
	const char *GetChunk(int num);
};

#endif 
