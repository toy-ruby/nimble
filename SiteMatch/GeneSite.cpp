#include "stdafx.h"
#include "GeneSite.h"
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

GeneSite::GeneSite()
{
	name = "";
	geneSymbol = "";
	chrom = "";
	txStart = -1;
	txEnd = -1;
	direction = "";
}

GeneSite::~GeneSite()
{
}

GeneSite::GeneSite(string _name, string _geneSymbol, string _chrom, int _txStart,
	int _txEnd, string _direction)
{
	name = _name;
	geneSymbol = stripSymbol(_geneSymbol);
	chrom = _chrom;
	txStart = _txStart;
	txEnd = _txEnd;
	direction = _direction;
}

GeneSite::GeneSite(string _geneSymbol, string _chrom, int _txStart, int _txEnd, string _direction)
{
	geneSymbol = stripSymbol(_geneSymbol);
	chrom = _chrom;
	txStart = _txStart;
	txEnd = _txEnd;
	direction = _direction;
}

GeneSite::GeneSite(string _name, string _chrom, int _txStart)
{
	chrom = _chrom;
	txStart = _txStart;
}

string GeneSite::stripSymbol(string rawSymbol)
{
	string result;
	// Remove all chars after ":"
	result = rawSymbol.substr(0, rawSymbol.find(':'));
	return result;
}

void GeneSite::clear()
{
	name = "";
	geneSymbol = "";
	chrom = "";
	txStart = -1;
	txEnd = -1;
	direction = "";
}