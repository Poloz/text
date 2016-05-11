// game-03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h> 
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

#define PI 3.14159265

int draw_sector(int energy, int warheads, float fuel, float oxygen, int sector[10][10], string prompt, int mode) {
	system("CLS");
	// status bar
	cout << "Energy: " << energy << 
		    "    Warheads storage: " << warheads << 
		    "    Fuel level: " << setprecision(4) << fuel << "%" <<
	    	"    Oxygen: " << setprecision(5) << oxygen << "t" << endl;
	// sector map
	cout << endl << "Near range radar screen:" << endl <<
		    "-----------------------------------" << endl;
	cout << "|    0  1  2  3  4  5  6  7  8  9 |" << endl;
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
		cout << "|";
		if (mode == 0) {
			if (i == 3) { cout << "  Avalible commands:"; }
			if (i == 4) { cout << "          a - attack"; }
			if (i == 5) { cout << "          m - move"; }
			if (i == 6) { cout << "          q - quit"; }
		}
		if (mode == 1) { 
			if (i == 1) { cout << "  Moving directions:"; } 
			if (i == 3) { cout << "              8   1   2"; }
			if (i == 5) { cout << "              7   S   3"; }
			if (i == 7) { cout << "              6   5   4"; }
		}
		if (mode == 2) {
			if (i == 1) { cout << "     Warhead course:"; }
			if (i == 3) { cout << "             315  0  45"; }
			if (i == 5) { cout << "             270  S  90"; }
			if (i == 7) { cout << "             225 180 135"; }
		}
		cout << endl;
	}
	cout << "-----------------------------------" << endl;
	cout << endl << endl << prompt;

	return 0;
}


int main()
{
	int clingons = 0;
	int warheads = 15;
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
	// q - quit
	char input = 'z';
	string prompt = "Your orders, capt'n? ";
	int ship_x = 0;
	int ship_y = 0;
	int ship_dest_x = 0;
	int ship_dest_y = 0;
	// Draw prompt_mode
	// 0 - avaliable commands
	// 1 - moving direction
	// 2 - attack routine
	int prompt_mode = 0;
	int warhead_course = 0;
	double warhead_x = 0;
	double warhead_y = 0;
	double delta = 0.001;
	double warhead_radius = 0;
	int target = 0;


	// randomize sector : 6-11 stars, 1-4 clingons, 1 ship, 0-1 base
	srand(time(NULL));
	for (int i = 1; i <= (rand() % 5 + 6); i++) { 
		do { 
			j = rand() % 10; 
			k = rand() % 10; 
		} while (sector[j][k] != 0); 
		sector[j][k] = 1; 
	}
	//                             3   1
	for (int i = 1; i <= (rand() % 3 + 1); i++) {
		do {
			j = rand() % 10;
			k = rand() % 10;
		} while (sector[j][k] != 0);
		sector[j][k] = 2;
		clingons++;
	}
	// Our ship
	do {
		ship_x = rand() % 10;
		ship_y = rand() % 10;
	} while (sector[ship_x][ship_y] != 0);
	sector[ship_x][ship_y] = 3;
	// 0-1 base
	if ((rand() % 2) == 1) {
		do {
			j = rand() % 10;
			k = rand() % 10;
		} while (sector[j][k] != 0);
		sector[j][k] = 4;
	}

	do 
	{
		switch (input) {
		case 'q': // Quit
			return 0;
			break;
		case 'a': // Attack
			prompt_mode = 2;
			prompt = "Warhead course? ";
			draw_sector(energy, warheads, fuel, oxygen, sector, prompt, prompt_mode);
			cin >> warhead_course;			
			warheads--;
			warhead_course = 90 - warhead_course;
			if (warhead_course < 0) { warhead_course += 360; }
			do {
				warhead_radius += delta;
				warhead_x = cos(warhead_course * PI / 180) * warhead_radius;
				warhead_y = sin(warhead_course * PI / 180) * warhead_radius;
				target = sector[ship_x + (int)round(warhead_x)][ship_y + (int)round(warhead_y)];
				if ((target != 0) && (target != 3)) { 
					if (target != 1) {
						sector[ship_x + (int)round(warhead_x)][ship_y + (int)round(warhead_y)] = 0;
					}
					break; }
			} while ((abs(warhead_x) <= 10.0) && (abs(warhead_y) <= 10.0));			
			warhead_radius = 0;
			switch (target) {
			case 1:
				prompt = "Warhead wanished in the star's flames! ";
				break;
			case 2:
				prompt = "Clingon killed! ";
				clingons--;
				break;
			case 4:
				prompt = "Base destroyed! ";
				break;
			default:
				prompt = "Something destroyed! ";
				break;
			}
			break;
		case 'm': // Move
			prompt = "Heading, capt'n? ";
			prompt_mode = 1;
			draw_sector(energy, warheads, fuel, oxygen, sector, prompt, prompt_mode);
			cin >> input;
			switch (input)
			{
			case '7':
				ship_dest_x = ship_x;
				ship_dest_y = ship_y - 1;
				break;
			case '6':
				ship_dest_x = ship_x+1;
				ship_dest_y = ship_y-1;
				break;
			case '5':
				ship_dest_x = ship_x+1;
				ship_dest_y = ship_y;
				break;
			case '4':
				ship_dest_x = ship_x+1;
				ship_dest_y = ship_y+1;
				break;
			case '3':
				ship_dest_x = ship_x;
				ship_dest_y = ship_y+1;
				break;
			case '2':
				ship_dest_x = ship_x-1;
				ship_dest_y = ship_y+1;
				break;
			case '1':
				ship_dest_x = ship_x-1;
				ship_dest_y = ship_y;
				break;
			case '8':
				ship_dest_x = ship_x-1;
				ship_dest_y = ship_y-1;
				break;
			default:
				prompt = "Can't carry the order, sir! ";
				break;
			}
			if ((input >= '1') && (input <= '8')) {
				if ((ship_dest_x < 0) || (ship_dest_y < 0) || (ship_dest_x > 9) || (ship_dest_y > 9)) {
					prompt = "Out of range, sir! ";					
				}
				else
					if (sector[ship_dest_x][ship_dest_y] != 0) {						
						switch (sector[ship_dest_x][ship_dest_y]) {
						case 1:
							prompt = "Star ahead! ";
							break;
						case 2:
							prompt = "Clingon ahead! ";
							break;
						case 4:
							prompt = "Base ahead! ";
							break;
						default:
							prompt = "Something ahead! ";
							break;
						}
					}
					else {
						sector[ship_x][ship_y] = 0;
						sector[ship_dest_x][ship_dest_y] = 3;
						ship_x = ship_dest_x;
						ship_y = ship_dest_y;
						prompt = "Maneuver commenced! ";
						fuel -= 0.1;
						oxygen -= 0.3;
						energy -= 100;
					}
			}
			break;
		default:
			break;
		}
		prompt_mode = 0;
		draw_sector(energy, warheads, fuel, oxygen, sector, prompt, prompt_mode);
		oxygen-= 0.9;
		fuel -= 0.01;
		energy += 250;		
	} while (cin >> input);
	return 0;
}