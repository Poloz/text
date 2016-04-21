// Stroustrup-03.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "std_lib_facilities.h"

int main()
{
	double input = 0; 
	double max = 0;
	double min = 0;
	bool first = true;
	string unit = "";
	double total;
	double converted = 0;
	bool interrupt = false;

	while (cin >> input >> unit)
	{
		cout << input << unit << "\n";
		//calibrate on first iteration
		if (first == true)
		{
			max = input;
			min = input;
			first = false;
			cout << "First! Max and min!\n";
			continue;
		}
		//convert input to meters
		if (unit == "cm") {converted = input * 0.01;} else
			if (unit == "m") { converted = input; } else
				if (unit == "in") {converted = input / 0.0254;} else
					if (unit == "ft") { converted = input * 12 / 0.0254; } else
					{ cout << "Achtung!! Unruled unit!\n"; continue; }
		if (input >= max) { cout << "Maxxed!\n"; max = input; }
		if (input <= min) { cout << "Minned!\n"; min = input; }
	}

	/*
	while (cin >> input >> j)
	{
		cout << input << " ; " << j << "\n";
		if (input == j) { cout << "Equal!\n"; continue; }
		if (abs(input - j) <= 0.01) { cout << "Almost equal!\n"; }
		if (input > j) { double t = input; input = j; j = t; }
		cout << "Lesser: " << input << ", greater: " << j << "\n";
	}
	*/
	cout << "Gotcha!\n";
	keep_window_open();
    return 0;
}

