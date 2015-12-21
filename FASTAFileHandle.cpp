#include "FASTAFileHandle.h"

FASTAFileHandle()
{
}

FASTAFileHandle(string path)
{
	filePath = path;
	dataWidth = detectDataWidth();
}

~FASTAFileHandle()
{
}

bool trimFASTA(string inPath, string outPath, double n = 1)
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

string getSequence(long begin, long end)
{
	string result;
	string tmp;
	char cur = '\0';
	long pos = 0;
	ifstream in = getFileInStream();
	if ((char)in.peek() == '>')
	{
		getline(in, tmp);
	}
	while(pos < begin)
	{
		in.get();
		pos++;
	}

	for (; pos < end; pos++)
	{
		result =+ cur;
		cur = (char)in.get();
	}
	return result;
}

ifstream getFileInStream()
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

bool isValid()
{
	ifstream in;
	bool result;

	in.open(filePath);
	result = (char)in.get() == '>';
	in.close();
	return result;
}

unsigned int detectDataWidth()
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

	while (cur = in.get() != '\n')
	{
		width++;
	}
	in.close();
	return width;
}

unsigned int getDataWidth()
{
	return dataWidth;
}