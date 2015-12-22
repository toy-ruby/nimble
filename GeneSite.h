//#pragma once
#ifndef GENESITE_H
#define GENESITE_H

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
	GeneSite(string _name, string _chrom, int _txStart);
	string stripSymbol(string);
	void clear();

	string name;
	string geneSymbol;
	string chrom;
	int txStart;
	int txEnd;
	string direction;
};

#endif