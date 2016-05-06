// game-03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h> 
#include <iostream>
#include <iomanip>

using namespace std;

int draw_screen(int energy, int torpedoes, float fuel, float oxygen, int sector[10][10]) {
	system("CLS");
	// status bar
	cout << "Energy: " << energy << 
		    "      Warheads storage: " << torpedoes << 
		    "      Fuel level: " << setprecision(4) << fuel << "%" <<
	    	"      Oxygen: " << setprecision(5) << oxygen << "t" << endl;
	// galactic map
	cout << endl << "Near range radar screen:" << endl <<
		    "---------------------------------" << endl;
	cout << "|    0  1  2  3  4  5  6  7  8  9" << endl;
	for (int i = 0; i <= 9; i++) { 
		cout << "| 0" << i; 
		for (int j = 0; j <= 9; j++) { 
			switch (sector[i][j]) {
			case 1: 
				cout << " * ";
				break;
			case 2:
				cout << " k ";
				break;
			case 3:
				cout << " S ";
				break;
			case 4:
				cout << " B ";
				break;
			default:
				cout << "   ";
				break;
			}
		}
		cout << endl;
	}
	
	return 0;
}


int main()
{
	int clingons = 0;
	int torpedoes = 0; //15
	float fuel = 100;
	float oxygen = 5000;
	int energy = 8300;
	// Local sector objects. 
	// 0 - space
	// 1 - star
	// 2 - clingon
	// 3 - our ship
	// 4 - base
	int sector[10][10] = { 0 };
	int j = 0;
	int k = 0;
	// command menu
	// a - attack
	// m - move 
	char input = 'z';

	// randomize sector : 6-11 stars, 1-4 clingons, 1 ship, 0-1 base
	srand(time(NULL));
	for (int i = 1; i <= (rand() % 5 + 6); i++) { 
		do { 
			j = rand() % 10; 
			k = rand() % 10; 
		} while (sector[j][k] != 0); 
		sector[j][k] = 1; 
	}
	for (int i = 1; i <= (rand() % 3 + 1); i++) {
		do {
			j = rand() % 10;
			k = rand() % 10;
		} while (sector[j][k] != 0);
		sector[j][k] = 2;
		clingons++;
	}

	do {
		j = rand() % 10;
		k = rand() % 10;
	} while (sector[j][k] != 0);
	sector[j][k] = 3;

	if ((rand() % 2) == 1) {
		do {
			j = rand() % 10;
			k = rand() % 10;
		} while (sector[j][k] != 0);
		sector[j][k] = 4;
	}

	do 
	{

		draw_screen(energy, torpedoes, fuel, oxygen, sector);
		oxygen-= 0.9;
		fuel -= 0.01;
		energy += 250;
		cout << endl << endl << "Your orders, capt'n? ";
	} while (cin >> input);

	return 0;
}

