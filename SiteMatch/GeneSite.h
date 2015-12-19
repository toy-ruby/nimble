#pragma once

#include <string>
#include <stdio.h>

using namespace std;

class GeneSite
{
public:
	GeneSite();
	~GeneSite();
	GeneSite(string _name, string _geneSymbol, string _chrom, int _txStart, int _txEnd, string _direction);
	GeneSite(string _geneSymbol, string _chrom, int _txStart, int _txEnd, string _direction);
	GeneSite::GeneSite(string _name, string _chrom, int _txStart);
	string GeneSite::stripSymbol(string);
	void GeneSite::clear();

	string name;
	string geneSymbol;
	string chrom;
	int txStart;
	int txEnd;
	string direction;
};

