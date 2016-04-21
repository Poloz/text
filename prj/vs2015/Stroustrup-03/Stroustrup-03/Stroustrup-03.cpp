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
	double total = 0;
	double converted = 0;
	bool interrupt = false;
	int counter = 0;
	vector<double> ruler;

	while (cin >> input >> unit)
	{
		cout << input << unit;

		//convert input to meters
		if (unit == "cm") {converted = input * 0.01;} else
			if (unit == "m") { converted = input; } else
				if (unit == "in") {converted = input * 2.54 / 100;} else
					if (unit == "ft") { converted = input * 12 * 2.54 / 100; } else
					{ cout << "\nAchtung!! Unruled unit!\n"; continue; }
		cout << " equal to " << converted << " m.\n";
		ruler.push_back(converted);

		total = total + converted;
		counter++;

		//calibrate on first iteration
		if (first == true)
		{
			max = input;
			min = input;
			first = false;
			cout << "First! Max and min!\n";
			continue;
		}
		if (input >= max) { cout << "Maxxed!\n"; max = input; }
		if (input <= min) { cout << "Minned!\n"; min = input; }
	}

	cout << "Total length: " << total << " m, " << counter << " items received.\n";
	sort(ruler);
	cout << "Ruler: ";
	for (double output : ruler) {
		cout << output << " ";
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
	cout << "That's all!\n";
	keep_window_open();
    return 0;
}

