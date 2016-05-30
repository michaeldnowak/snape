#include "parser.h"
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <stdio.h>
#include <string.h>
using namespace std;

Parser::Parser(char delim){
	delimiter = delim;
}

Parser::~Parser(){
	for(vector<string*>::iterator it = chunks.begin();it != chunks.end(); it++){
		delete *it;
	} 
	chunks.clear();
}
	
int Parser::Parse(string &str, int expected){
	int pos=0;
	char c[2];
	c[1] = '\0';

	while(pos <= str.size()){
		char temp[50];
		temp[0] = '\0';
		while(str[pos] != delimiter && pos < str.size()){
			c[0] = str[pos];
			strcat(temp, c);
			pos++;
		}
		pos++;
		string *tempStr = new string(temp);
		chunks.push_back(tempStr);
	}
	if(chunks.size() != expected) return 1;
	else return 0;
}

void Parser::Empty(){
	for(vector<string*>::iterator it = chunks.begin();it != chunks.end(); it++){
		delete *it;
	}
	chunks.clear();		
}
		
const char *Parser::GetChunk(int num){
	assert(num < chunks.size());
	return chunks[num]->c_str();	
}

int Parser::NumChunks() const { return chunks.size();}





