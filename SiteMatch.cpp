// SiteMatch.cpp : Defines the entry point for the application.
//

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>
#include "GeneSite.h"
#include "FASTAFileHandle.h"
#include "MatchItem.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

void printGene(GeneSite);
bool handleArgs(int count, char* args[]);
map<int, map<int, GeneSite>> matchGene(map<int, GeneSite> hg19Genes, map<int, GeneSite> mGenes);
map<int, GeneSite> mapFromFile(string path);
map<int, string> matchListFromFile(string path);
streampos fIndex(int ind, int headerLen);

string matchFilePath;
string chromDirPath;
string header;
string chrom;
string inPath;
string outPath;
string tmp;
bool verbose = false;
unsigned int bases;
unsigned int coord;

int main(int argc, char* argv[])
{
	if (!handleArgs(argc, *&argv))
		return 1;
	// At this point, we will have EITHER
	// filePath set OR
	// chrom, coord and header set
	ofstream outs;
	outs.open(outPath);

	if (matchFilePath == "")
	{
		GeneSite g(header, chrom, coord);
		MatchItem item(g, chromDirPath);
		string output = item.doMatch(bases);
		if (verbose) cout << "Matching sequence: " << output << endl;
		outs << item.getHeader() << endl;
		outs << output << endl << endl;
		// send new line to out stream
	}
	else
	{
		ifstream ins;
		if (fs::exists(matchFilePath) &&
			fs::is_regular_file(matchFilePath))
		{
			GeneSite g;

			ins.open(matchFilePath);
			while (!ins.eof())
			{
				if (ins.get() != '#')
				{
					ins.unget();
					ins >> g.name >> g.geneSymbol >> g.chrom >> g.txStart >> g.txEnd >> g.direction;
					MatchItem item(g, chromDirPath);
					string output = item.doMatch(bases);
					if (verbose) cout << "Matching sequence: " << output << endl;
					outs << item.getHeader() << endl;
					outs << output << endl << endl;
				}
				else
				{
					string tmp;
					getline(ins, tmp);
				}
			}
			ins.close();
		}
		else {
			cout << matchFilePath << " is not a regular file." << endl;
		}
	}
	outs.close();
	return 0;
}

void printGene(GeneSite gene)
{
	cout << "Name: " << gene.name << endl;
	cout << "Symbol: " << gene.geneSymbol << endl;
	cout << "Direction: " << gene.direction << endl;
	cout << "\n";
}

bool handleArgs(int count, char* args[])
{
	po::options_description desc("Allowed Options");
	desc.add_options()
		("help,?", "Show this message")
		("header,h", po::value<string>(), "The header name")
		("chrom,c", po::value<string>(), "The desired chromosome")
		("coord,C", po::value<unsigned int>(), "The location coordinate of the desire site")
		("bases,b", po::value<unsigned int>(), "Number of base pairs to return")
		("genome-dir,d", po::value<string>(), "Path to directory containing genome files")
		("input-file,i", po::value<string>(), "The path the the input file to be used for matching")
		("output-file,o", po::value<string>(), "Path to output FASTA file")
		("verbose,v", "Display verbose output to standard out.")
		;

	po::positional_options_description p;
	p.add("genome-dir", 1);
	p.add("output-file", 1);
	p.add("input-file", 1);

	po::variables_map vm;
	try {
		po::store(po::command_line_parser(count, args).
			options(desc).positional(p).run(), vm);
	}
	catch (exception ex) {
		cout << "Unknown arguments" << endl;
		cout << desc << endl;
		return false;
	}

	if (vm.count("help"))
	{
		cout << desc << endl;
		return false;
	}
	if (vm.count("verbose"))
	{
		verbose = true;
	}
	else
	{
		verbose = false;
	}
	if (vm.count("header") && !vm.count("file-input"))
	{
		header = vm["header"].as<string>();
		if (verbose) cout << "Header: " << vm["header"].as<string>() << endl;	// DEBUG
	}
	else if (vm.count("header") && vm.count("file-input"))
	{
		cout << "Header and Input-File are specified. Deafulting to input-file. Header will be discarded." << endl;
		matchFilePath = vm["input-file"].as<string>();
	}
	else if (!vm.count("header") && vm.count("file-input"))
	{
		matchFilePath = vm["input-file"].as<string>();
	}

	if (vm.count("chrom") && !vm.count("file-input"))
	{
		chrom = vm["chrom"].as<string>();
		if (verbose) cout << "Chromosome: " << vm["chrom"].as<string>() << endl;	// DEBUG

	}
	else if (vm.count("chrom") && vm.count("file-input"))
	{
		cout << "Chrom and Input-File are specified. Deafulting to input-file. Chrom will be discarded." << endl;
		chrom = "";
	}
	else if (vm.count("input-file"))
	{
		matchFilePath = vm["input-file"].as<string>();
	}
	else 
	{
		cout << "--input-file or --chrom is required!" << endl;
		cout << desc << endl;
		return false;
	}


	if (vm.count("coord") && !vm.count("input-file"))
	{
		coord = vm["coord"].as<unsigned int>();
		if (verbose) cout << "Coordinate: " << vm["coord"].as<unsigned int>() << endl;	// DEBUG
	}
	else if (vm.count("coord") && vm.count("input-file"))
	{
		cout << "Coord and Input-File are specified. Deafulting to input-file. Coord will be discarded." << endl;
		matchFilePath = vm["input-file"].as<string>();
		coord = -1;
	}

	if (vm.count("bases"))
	{
		bases = vm["bases"].as<unsigned int>();
		if (verbose) cout << "Bases: " << vm["bases"].as<unsigned int>() << endl;	// DEBUG
	}
	else {
		cout << "--bases is required!" << endl;
		cout << desc << endl;
		return false;
	}

	if (vm.count("genome-dir"))
	{
		chromDirPath = vm["genome-dir"].as<string>();
		if (verbose) cout << "Genome Directory: " << vm["genome-dir"].as<string>() << endl;	// DEBUG

	}
	else {
		cout << "--genome-dir is required!" << endl;
		cout << desc << endl;
		return false;
	}

	if (vm.count("output-file"))
	{
		outPath = vm["output-file"].as<string>();
		if (verbose) cout << "Out File: " << vm["output-file"].as<string>() << endl;	// DEBUG
	}
	else 
	{
		cout << "--output-file is required!" << endl;
		cout << desc << endl;
		return false;
	}

	if (vm.count("input-file") && !vm.count("header"))
	{
		inPath = vm["input-file"].as<string>();
		if (verbose) cout << "In File: " << vm["input-file"].as<string>() << endl;	// DEBUG
		matchFilePath = vm["input-file"].as<string>();
	}
	else if (vm.count("input-file") && vm.count("header"))
	{
		cout << "Header and Input-File are specified. Deafulting to input-file. Header will be discarded." << endl;
		inPath = vm["input-file"].as<string>();
		header = "";
	}

	po::notify(vm);
	return true;
}

map<int, map<int, GeneSite>> matchGene(map<int, GeneSite> hg19Genes, map<int, GeneSite> mGenes)
{
	map<int, map<int, GeneSite>> result;
	map<int, GeneSite> match;

	for (int i = 0; i < hg19Genes.size(); i++)
	{
		for (int j = 0; j < mGenes.size(); j++)
		{
			if (hg19Genes[i].txStart == mGenes[j].txStart)
			{
				unsigned int pos = j - 10;
				unsigned int ind = 0;
				while (pos <= 21)
				{
					match[ind] = mGenes[pos];
					pos++;
				}
				result[i] = match;
			}
		}
	}

	return result;
}

map<int, GeneSite> mapFromFile(string path)
{
	map<int, GeneSite> result;
	ifstream inFile;
	string line;
	string _name, _geneSymbol, _chrom, _txStart, _txEnd, _direction;
	unsigned int count = 0;

	inFile.open(path);
	while (getline(inFile, line))
	{
		istringstream reader(line);
		reader >> _name >> _geneSymbol >> _chrom >> _txStart >> _txEnd >> _direction;
		if (_name == "#hg19.knownGene.name") // ignore header
		{
			continue;
		}
		result[count] = GeneSite(_name, _geneSymbol, _chrom, stoi(_txStart), stoi(_txEnd), _direction);
		printGene(result[count]);
		count++;
	}

	inFile.close();

	return result;
}

map<int, string> matchListFromFile(string path)
{
	map<int, string> result;
	ifstream inFile;
	string line, tmp;
	unsigned int count = 0;

	inFile.open(path);
	while (getline(inFile, line))
	{
		istringstream reader(line);
		if (line == "\"x\"")
		{
			continue;
		}
		replace(line.begin(), line.end(), '\"', ' ');
		reader.str(line);
		reader >> tmp >> result[count];
		count++;
	}
	return result;

}

streampos fIndex(int ind, int headerLen)
{
	streampos result;
	result = ind - headerLen;
	result = result - (result % 50);
	return result;
}