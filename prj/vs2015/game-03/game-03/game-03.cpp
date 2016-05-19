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

int clingons = 0;
int ship_x = 0;
int ship_y = 0;
int sector[10][10] = { 0 };
int galaxy[12][12] = { 0 };

int init_sector(int stars_min, int stars_max, int clingons_min, int clingon_max, bool base) {

	int j;
	int k;

	srand(time(NULL));
	// stars
	for (int i = 1; i <= (rand() % (stars_max - stars_min) + stars_min); i++) {
		do { 
			j = rand() % 10; 
			k = rand() % 10; 
		} while (sector[j][k] != 0); 
		sector[j][k] = 1; 
	}
	// clingons
	for (int i = 1; i <= (rand() % (clingon_max - clingons_min) + clingons_min); i++) {
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
	if (base == true) {
		do {
			j = rand() % 10;
			k = rand() % 10;
		} while (sector[j][k] != 0);
		sector[j][k] = 4;
	}
		
	return 0;
}

int draw_statusbar(int energy, int warheads, float fuel, float oxygen) {
	system("CLS");
	cout << "Energy: " << energy <<
		"    Warheads storage: " << warheads <<
		"    Fuel level: " << setprecision(4) << fuel << "%" <<
		"    Oxygen: " << setprecision(5) << oxygen << "t" << endl;
	return 0;
}

int draw_galaxy(int energy, int warheads, float fuel, float oxygen, int galaxy[12][12], string prompt) {
	draw_statusbar(energy, warheads, fuel, oxygen);
	cout << endl << "Known galaxy map:" << endl <<
			"------------------------------------------" << endl;
	cout << "|     0  1  2  3  4  5  6  7  8  9 10 11 |" << endl;
	for (int i = 0; i <= 11; i++) {
		cout << "|";
		if (i < 10) { cout << " "; }
		cout << " 0" << i;
		for (int j = 0; j <= 11; j++) {
			switch (galaxy[i][j]) {
			case 1:
				cout << " * ";
				break;
			default:
				cout << " ? ";
				break;
			}
		}
		cout << "|" << endl;
	}

	cout << "------------------------------------------" << endl;
	cout << prompt;
	system("pause");
	return 0;
}

int draw_sector(int energy, int warheads, float fuel, float oxygen, int sector[10][10], string prompt, int mode) {
	draw_statusbar(energy, warheads, fuel, oxygen);
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
			case 5:
				cout << " w ";
				break;
			default:
				cout << " . ";
				break;
			}
		}
		cout << "|";
		if (mode == 0) {
			if (i == 3) { cout << "  Available commands:"; }
			if (i == 4) { cout << "          a - attack with warhead"; }
			if (i == 5) { cout << "          m - move"; }
			if (i == 6) { cout << "          j - jump to another sector"; }
			if (i == 7) { cout << "          q - quit"; }
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
		if (mode == 3) {
			if (i == 3) { cout << "  Available commands:"; }
			if (i == 4) { cout << "          a - attack with warhead"; }
			if (i == 5) { cout << "          m - move"; }
			if (i == 6) { cout << "          d - dock"; }
			if (i == 7) { cout << "          j - jump to another sector"; }
			if (i == 8) { cout << "          q - quit"; }
		}
		cout << endl;
	}
	cout << "-----------------------------------" << endl;
	cout << prompt;

	return 0;
}

int intro() {
	char ch;

	system("CLS");
	cout << "-----------------------------------------------------\n";
	cout << "              Old Console Trekkie Game\n\n";
	cout << "            (Not like Xbox or PS console)\n";
	cout << "\n";
	cout << "                     Remake\n";
	cout << "                      v.01\n";
	cout << "\nFYI some radar readings:\n\n";
	cout << " * - star. Hot ball of hot gases.\n";
	cout << " k - slimy klingon. Seven-assed enemy.\n";
	cout << " S - our ship. She's real beauty, ain't she, capt'n?\n";
	cout << " B - sector base. Supplies!\n";
	cout << " w - dangerous warhead.\n";
	cout << " . - vast black space. No air, just starsine.\n";
	cout << "\nYou can wander around, launch warhead occasionaly\n";
	cout << "and quit whenever you like. Shiny!\n";
	cout << "-----------------------------------------------------\n";
	cout << "\n";
	cout << "\n";
	system("pause");
	return 0;
}

int endgame(int clingons, bool base_destroyer) {
	system("CLS");
	cout << "----------------------------------------\n";
	cout << "-Clingons rest: " << clingons << endl;
	if (base_destroyer == true) {
	cout << "-By the way, you are wanted fugitive now\n for destroying friendly base.\n";
	}
	cout << "-See ya, capt'n!\n";
	cout << "----------------------------------------\n\n\n";
	system("pause");
	return 0;
}


int main()
{
	
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
	// 5 - warhead

	int j = 0;
	int k = 0;
	// command menu
	// a - attack
	// m - move
	// d - dock
	// j - jump
	// q - quit
	char input = 'z';
	string prompt = "\n\n-Your orders, capt'n?\n-";

	int ship_dest_x = 0;
	int ship_dest_y = 0;
	// Draw prompt_mode
	// 0 - avaliable commands
	// 1 - moving direction
	// 2 - attack routine
	// 3 - docking
	int prompt_mode = 0;
	int warhead_course = 0;
	double warhead_x = 0;
	double warhead_y = 0;
	double delta = 0.001;
	double warhead_radius = 1;
	int target = 0;
	bool docking = false;
	int warhead_old_x = 0;
	int warhead_old_y = 0;
	bool base_destroyer = false;
	bool quit = false;

	// randomize sector : 6-11 stars, 1-4 clingons, 1 ship, 0-1 base
	init_sector(6,11,1,4,true);
		
	intro();

	do 
	{
		switch (input) {
		case 'q': // Quit
			endgame(clingons, base_destroyer);
			quit = true;
			break;
		case 'a': // Attack
			prompt_mode = 2;
			if (warheads == 0) {
				prompt = "\n\n-We are short on warheads, need resupply!\n-What should we do?\n-";
				break;
			}
			prompt = "\n\n-Warhead course?\n-";
			draw_sector(energy, warheads, fuel, oxygen, sector, prompt, prompt_mode);

			if (cin >> warhead_course )
			{
				// It worked (input is now in a good state)
							
			warheads--;
			warhead_course = 180 - warhead_course;
			if (warhead_course < 0) { warhead_course += 360; }
			warhead_old_x = ship_x;
			warhead_old_y = ship_y;
			do {
				warhead_radius += delta;
				warhead_x = cos(warhead_course * PI / 180) * warhead_radius;
				warhead_y = sin(warhead_course * PI / 180) * warhead_radius;
				target = sector[ship_x + (int)round(warhead_x)][ship_y + (int)round(warhead_y)];
				if ( target == 0 && 
					( 
					(ship_x + (int)round(warhead_x) != warhead_old_x ) || (ship_y + (int)round(warhead_y) != warhead_old_y )
						)
					) {
					sector[ship_x + (int)round(warhead_x)][ship_y + (int)round(warhead_y)] = 5;
					draw_sector(energy, warheads, fuel, oxygen, sector, prompt, prompt_mode);
					_sleep(500);
					sector[ship_x + (int)round(warhead_x)][ship_y + (int)round(warhead_y)] = 0;
					warhead_old_x = ship_x + (int)round(warhead_x);
					warhead_old_y = ship_y + (int)round(warhead_y);
				}				
				if ((target != 0) && (target != 3)) { 
					if (target != 1) {
						sector[ship_x + (int)round(warhead_x)][ship_y + (int)round(warhead_y)] = 0;
					}
					break; }
			} while ((abs(warhead_x) <= 10.0) && (abs(warhead_y) <= 10.0));			
			warhead_radius = 1;
			//warhead_old_x = ship_x;
			//warhead_old_y = ship_y;
			switch (target) {
			case 1:
				prompt = "\n\n-Warhead wanished in the star's flames!\n-Orders?\n-";
				break;
			case 2:
				prompt = "\n\n-Clingon killed!\n-What should we do now?\n-";
				clingons--;
				break;
			case 4:
				prompt = "\n\n-Base destroyed!\n-What's next?\n-";
				base_destroyer = true;
				break;
			default:
				prompt = "\n\n-Lost warhead telemetry!\n-We all doomed!\n-";
				break;
			}

			}
			else
			{
				// input is in a bad state.
				// So first clear the state.
				cin.clear();

				// Now you must get rid of the bad input.
				// Personally I would just ignore the rest of the line
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				// now that you have reset the stream you can go back and try and read again.
				prompt = "\n\n-Wrong bearing, sir!\n-Your command, sir?\n-";
			}

			break;
		case 'm': // Move
			prompt = "\n\n-Heading, capt'n?\n-";
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
				prompt = "\n\n-Can't carry the order, sir!\n-Your command, sir?\n-";
				break;
			}
			if ((input >= '1') && (input <= '8')) {
				if ((ship_dest_x < 0) || (ship_dest_y < 0) || (ship_dest_x > 9) || (ship_dest_y > 9)) {
					prompt = "\n\n-Out of range, sir!\n-Orders?\n-";					
				}
				else
					if (sector[ship_dest_x][ship_dest_y] != 0) {						
						switch (sector[ship_dest_x][ship_dest_y]) {
						case 1:
							prompt = "\n\n-Star ahead!\n-Orders, sir?\n-";
							break;
						case 2:
							prompt = "\n\n-Clingon ahead!\n-Attacking, sir?\n-";
							break;
						case 4:
							prompt = "\n\n-Base ahead!\n-We can dock, sir?\n-";
							docking = true;
							break;
						default:
							prompt = "\n\n-Something ahead!\n-What will be the orders, sir?\n-";
							break;
						}
					}
					else {
						sector[ship_x][ship_y] = 0;
						sector[ship_dest_x][ship_dest_y] = 3;
						ship_x = ship_dest_x;
						ship_y = ship_dest_y;
						prompt = "\n\n-Maneuver commenced!\n-Orders, sir?\n-";
						if (fuel >= 0.1) { fuel -= 0.1; } else { fuel = 0; }
						if (oxygen >= 0.3) { oxygen -= 0.3; } else { oxygen = 0; }
						if (energy >= 100) { energy -= 100; } else { energy = 0; }
					}
			}
			break;
		case 'd':
			if (docking == false) { 
				prompt = "\n\n-Your orders, capt'n?\n-";
				break; }
			warheads = 15;
			fuel = 100;
			if (oxygen < 5000) { oxygen = 5000; }
			if (energy < 8300) { energy = 8300; }
			docking = false;
			prompt = "\n\n-Got fresh ones!!\n-New orders, sir?\n-";
			break;
		case 'j':
			draw_galaxy(energy, warheads, fuel, oxygen, galaxy, prompt);
			break;
		default:
			prompt = "\n\n-Your orders, capt'n?\n-";
			break;
		}
		prompt_mode = 0;
		if (docking == true) { prompt_mode = 3; }
		if (quit == true) { break; }
		draw_sector(energy, warheads, fuel, oxygen, sector, prompt, prompt_mode);
		if (oxygen >= 0.9) { oxygen -= 0.9; } else { oxygen = 0; }
		if (fuel >= 0.01) { fuel -= 0.01; }	else { fuel = 0; }
		if (energy <= 19750) { energy += 250; } else { energy = 20000; }
	} while (cin >> input);

	return 0;
}