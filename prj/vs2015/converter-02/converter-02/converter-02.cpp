// Converter utility for enlarging values in xml by 1.28 coefficient.
// Checks for " x=", " y=", "_x=" and "_y=" strings.

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string replace_me(string in_str, string pattern)
{
	string out_str;
	if (in_str.find(pattern) != string::npos)
	{
		int pos_b = 0,
			pos_e = 0;
		double conv = 0;
		int magic = pattern.length();
		double scale = 0.28;		

		pos_b = in_str.find(pattern);
		pos_e = in_str.find("\"", pos_b + magic);
		conv = stod(in_str.substr(pos_b + magic, pos_e - pos_b - magic));
		conv += round(conv * scale);
		out_str = in_str.substr(0, pos_b + magic) +
				to_string (int(conv)) +
				in_str.substr(pos_e, in_str.length() - pos_e);
	}
	else
	{
		out_str = in_str;
	}
	return out_str;
}


int main(int argc, char *argv[] )
{
	if (argc != 2) { cout << "Bigfoot xml converter, v0.1. (c)JoyBits 2016.\n" <<
		"Use: converter <filename>\n" << 
		"Input file will be overwritten."; return 0; }

	string outname = argv[1];
	const char * inname = argv[1];
	ifstream fin(outname);	
	outname += ".new";	
	ofstream fout(outname);

	if (fout.is_open())
	{ 
		if (fin.is_open())
		{
			string instring;
			bool success = true;
			const char * outname_ch;
			const char * inname_ch;

			while (getline(fin, instring))
			{
				instring = replace_me(instring, " x=\"");
				instring = replace_me(instring, " y=\"");
				instring = replace_me(instring, "_x=\"");
				instring = replace_me(instring, "_y=\"");
				fout << instring << endl;
			}
			fin.close();
			if (remove(inname) != 0)
			{ 
				cout << "Remove " << inname << " failed." << endl;
				success = false; 
			}
			outname_ch=outname.c_str();
			fout.close();
			if (rename(outname_ch, inname) != 0)
			{ 
				cout << "Rename " << outname_ch << " to " << inname << " failed." << endl;
				success = false;
			}
			if (success) { cout << "Converted successfully: " << inname << endl; }
		}
		else
		{
			cout << "Error opening input file: " << inname << endl;
		}
	}
	else
	{
		cout << "Error creating temp file: " << outname << endl;
	}
	return 0;
}

