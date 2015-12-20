#include "MatchItem.h"
#include "FASTAFileHandle.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

//MatchItem::MatchItem()
//{
//}
MatchItem::MatchItem(GeneSite g, string dir)
{
	string coord;
	header = "";
	header.append(g.name + " ");
	header.append(g.chrom + " ");
	header.append(to_string(g.txStart));
	site = g.txStart;
	chrom = g.chrom;
	if (dirValid(dir))
	{
		chromDir = dir;
	}
	else { return; }
}
MatchItem::MatchItem(ifstream &cmpFile, string dir)
{
	chromDir = dir;
	matchIn = &cmpFile;
}
MatchItem::~MatchItem()
{
}
string MatchItem::getHeader()
{
	return header;
}
string MatchItem::getSequence()
{
	return sequence;
}
bool MatchItem::fileValid(string path)
{
	string p = path;
	fs::path fp(p);
	if (fs::exists(fp) && fs::is_regular_file(fp))
	{
		return true;
	}
	return false;
}
bool MatchItem::dirValid(string path)
{
	string p = path;
	fs::path dp(p);
	if (fs::exists(dp) && fs::is_directory(dp))
	{
		return true;
	}
	return false;
}

string MatchItem::doMatch(unsigned int bases)
{
	string targetSeq;

	for (fs::directory_entry& x : fs::directory_iterator(chromDir)) // find the correct chrom file
	{
		string fileNameClean = x.path().filename().string();
		fileNameClean = fileNameClean.substr(0, fileNameClean.find("."));
		if (fileNameClean == chrom)
		{
			FASTAFileHandle fh(x.path().string());
			ifstream chromIn = fh.getFileInStream();
			streampos file_start;
			string temp;
			if ((char)chromIn.get() == '>')
			{
				getline(chromIn, temp);	// ignore header line
				file_start = chromIn.tellg();
				//cout << "file_start = " << (int)file_start << endl;
			} 

			// cout << "Trying to find " << site << "..." << endl;
			streampos newPos = site + file_start;
			int data_width = fh.getDataWidth();
			newPos = ((int)newPos + ((int)(site / data_width) * 2)) - 1;	// Windows line breaks
			// newPos = (int)newPos + (int)(site / data_width);				// Unix line breaks
			chromIn.seekg(newPos);
			auto tell_pos = chromIn.tellg();
			if ((tell_pos % data_width) > 11 || (tell_pos % data_width) < 40)	// TODO: perform calculation with data_width
			{																	// instead of magic numbers	
				newPos -= bases + 2;
			}
			else {
				newPos -= bases;
			}

			chromIn.seekg(newPos);
			for (int i = (bases * 2); i >= 0; i--)
			{
				if (chromIn.get() == '\n')
				{
					i++;
					continue;
				}
				chromIn.unget();
				targetSeq.push_back((char)chromIn.get());
			}
			chromIn.close();
		}
		return targetSeq;
	}
}