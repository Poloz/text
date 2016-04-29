// converter-02.cpp : Defines the entry point for the console application.
//

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
		

		pos_b = in_str.find(pattern);
		pos_e = in_str.find("\"", pos_b + 3);
		cout << pos_b << " " << pos_e << " " << in_str.length() - pos_e - pos_b - 3 << endl;
		//cout << instring.substr(pos_b+3, pos_e - pos_b - 3) << endl;
		conv = stod(in_str.substr(pos_b + 3, pos_e - pos_b - 3));
		conv += round(conv * 0.28);
//		cout << in_str.substr(0, pos_b + 3) <<
//			conv <<
//			in_str.substr(pos_e, in_str.length() - pos_e - pos_b - 3) <<
//			endl;
		out_str = in_str.substr(0, pos_b + 3) +
				to_string (int(conv)) +
				in_str.substr(pos_e, in_str.length() - pos_e); // - pos_b - 3

	}
	return out_str;
}


int main()
{
	char ch;

	ifstream fin("input.txt");
	if (fin.is_open())
	{
		cout << "Opened file\n";
		string instring;
		string outstring;
		while (getline(fin, instring))
		{
			cout << instring << endl;
			instring = replace_me(instring, "x=\"");
			cout << instring << endl;
			instring = replace_me(instring, "y=\"");
			cout << instring << endl;

		}
	}
	else
	{
		cout << "Error opening input file\n";
	}
	cin >> ch;
    return 0;
}

