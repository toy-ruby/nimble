#include "FASTAFileHandle.h"

FASTAFileHandle::FASTAFileHandle()
{
}

FASTAFileHandle::FASTAFileHandle(string path)
{
	filePath = path;
	dataWidth = detectDataWidth();
}

FASTAFileHandle::~FASTAFileHandle()
{
}

bool FASTAFileHandle::trimFASTA(string inPath, string outPath, double n = 1)
/*
Take inPath of FASTA File, write n number of lines to outPath file
Purpose is to cut a sample of an otherwise too-large-to-open file
in most text editors. This is largely for troubleshooting and debugging
purposes. Likely, no practical use cases.
*/
{
	bool result = true;
	string line = "";

	ofstream out;
	out.open(outPath);
	if (out.fail())
	{
		cout << "Out file open failed!";
		result = false;
	}
	ifstream in;
	in.open(inPath);
	if (!in.is_open())
	{
		cout << "File is not open." << endl;
	}

	for (int i = 0; i < n; i++)
	{
		getline(in, line);
		out << line << "\n";		
	}
	out.close();
	in.close();

	return result;
}

ifstream FASTAFileHandle::getFileInStream()
{
	ifstream fin;
	if (!isValid())
	{
		cout << "Invalid FASTA format: " << filePath << endl;
	}
	else {
		fin.open(filePath);
		return fin;
	}
}

bool FASTAFileHandle::isValid()
{
	ifstream in;
	bool result;

	in.open(filePath);
	result = (char)in.get() == '>';
	in.close();
	return result;
}

unsigned int FASTAFileHandle::detectDataWidth()
{
	char cur;
	unsigned int width = 0;
	ifstream in;
	in.open(filePath);

	if ((char)in.get() == '>')
	{
		string tmp;
		std::getline(in, tmp);
	}

    cur = in.get();
    while (cur != '\n' && cur != '\r')
	{        
		width++;
        cur = in.get();
	}
	in.close();
	return width;
}

unsigned int FASTAFileHandle::getDataWidth()
{
	return dataWidth;
}
