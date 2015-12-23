#ifndef FASTAFILEHANDLE_H
#define FASTAFILEHANDLE_H

#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class FASTAFileHandle
{
public:
	FASTAFileHandle();
	FASTAFileHandle(string path);
	~FASTAFileHandle();

	bool trimFASTA(string inPath, string outPath, double lines);
	ifstream getFileInStream();
	bool isValid();
	unsigned int getDataWidth();

private:
	string filePath;
	unsigned int fileSize;
	unsigned int dataWidth;

	unsigned int detectDataWidth();
};

#endif