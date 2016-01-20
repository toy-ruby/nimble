#include "MatchItem.h"
#include "FASTAFileHandle.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

MatchItem::MatchItem()
{
}
MatchItem::MatchItem(GeneSite g, string dir)
{
	string coord;
	header = ">";
	header.append(g.name);
	header.append("|");
	header.append(g.chrom);
	header.append("|");
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
    cout << "File: " << p << " is not a directory or does not exist." << endl;
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
            //newPos = ((int)newPos + ((int)(site / data_width) * 2)) - 1;	// Windows line breaks
            newPos = ((int)newPos + ((int)(site / data_width))) - 1;	// Linux line breaks
			chromIn.seekg(newPos);
			auto tell_pos = chromIn.tellg();
			
			// determine if current position is within bases positions of beg/end of line
			if (tell_pos > data_width) {
				if ((site % data_width) < (1 + bases))
				{
                    newPos -= bases + 1;    // Linux
                    //newPos -= bases + 2;  // Windows
				}
				else {
					newPos -= bases;
				}
			}
			else 
			{
				// TODO determine if this is necessary; may not need conditional here
				if ((tell_pos % data_width) > (data_width - bases))
				{
                    newPos -= bases + 2;    // Windows
                    newPos -= bases + 1;    // Linux
				}
				else {
					newPos -= bases;
				}
			}

            char tmp;
			chromIn.seekg(newPos);
            for (int i = (bases * 2); i >= 0; i--)
			{
                tmp = chromIn.get();
                if (tmp == '\n' ||
                        tmp == '\r')
				{
					i++;
					continue;
				}
                //chromIn.unget();
                targetSeq.push_back(tmp);
				//cout << "tmp = " << tmp << endl;
			}
			chromIn.close();
			return targetSeq;
		}
		
	}
}
