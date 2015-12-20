#pragma once
#include <fstream>
#include "GeneSite.h"

class MatchItem
{
public:
	//MatchItem();
	~MatchItem();
	MatchItem(GeneSite gene, string chromDir);
	MatchItem(ifstream& cmpFile, string chromDir);

	string getHeader();
	string getSequence();
	string doMatch(unsigned int bases);

private:
	string header;
	string sequence;
	string chromDir;
	string chrom;
	ifstream *matchIn;
	bool fileValid(string path);
	bool dirValid(string path);
	unsigned long site;
};