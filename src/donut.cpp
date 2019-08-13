//Project Identifier : 8729BF7B2234189B8DF8A6DD31770D2B18569C27

#include <iostream>
#include <string>
#include <getopt.h>
#include <iomanip>
#include <limits>
#include <vector>
#include <queue>
#include <utility>
#include <cmath>
#include "donut.h"

using namespace std;

void options(int argc, char * argv[], string &programMode) {
	opterr = true;
	int choice;
	int option_index = 0;
	option long_options[] = {
		{ "help",  no_argument, nullptr, 'h'},
		{ "mode", required_argument, nullptr, 'm'},
		{ nullptr, 0, nullptr, '\0' }
	};

	while ((choice = getopt_long(argc, argv, "hm:", long_options, &option_index)) != -1) {
		switch (choice) {
		case 'h':
			cout << "pls work\n";
			exit(0);
		case 'm':
			programMode = optarg;
			if (programMode != "MST" && programMode != "FASTTSP" && programMode != "OPTTSP") {
				cerr << "Error: invalid output option\n";
				exit(1);
			}
			break;
		default:
			cerr << "Error: invalid option" << '\n';
			exit(1);
		} //switch
	} // while
}

void init_coordinates(vector< pair<int,int> > &coordinates, int num_shops) {
	int counter = 0;
	pair <int, int> coord;
	int x;
	int y;
	while (counter != num_shops) {
		cin >> x >> y;
		coord.first = x;
		coord.second = y;
		coordinates.push_back(coord);
		++counter;
	}
}

//O(n).. worth doing this before anything else?
bool check_legal(const vector<pair<int,int> > &coordinates) {
	bool USexists = false;
	bool CANexists = false;
	bool BORDERexists = false;
	int coorSize = (int)coordinates.size();
	int x = 0;
	int y = 0;
	for (int i = 0; i < coorSize; ++i) {
		x = coordinates[i].first;
		y = coordinates[i].second;
		if ((x == 0 && y > 0) || (y == 0 && x > 0) || (x == 0 && y == 0)) {//on border
			BORDERexists = true;
		}
		else if (x < 0 || y < 0) {//IN US
			USexists = true;
		}
		else if (x > 0 && y > 0) {//IN CANADA
			CANexists = true;
		}
		if (USexists && CANexists && BORDERexists) {
			return true;
		}
	}
	if ((USexists && !CANexists) || (!USexists && CANexists)) {
		return true;
	}
	else if (!USexists && !CANexists && BORDERexists) {//if only border exists
		return true;
	}
	return false;
}


int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	
	cout << std::setprecision(2);
	cout << std::fixed;
	
	string programMode = "none";
	
	//read in command line and file
	options(argc, argv, programMode);
	
	int num_shops;
	cin >> num_shops;

	vector< pair<int, int> > coordinates;
	coordinates.reserve(num_shops);
	init_coordinates(coordinates, num_shops);
	
	if (programMode == "MST") {
		if (!check_legal(coordinates)) {
			cerr << "Cannot construct MST\n";
			exit(1);
		}
		calcDistanceWithBorder distanceBorder;
		Graph graph(distanceBorder);
		graph.print_MST(coordinates, num_shops);
		return 0;
	}
	
	else if (programMode == "FASTTSP") {
		calcDistanceNoBorder calcDistance;
		Graph graph(calcDistance);
		graph.print_FASTTSP(coordinates, num_shops);
	}
	else if (programMode == "OPTTSP" ){
		calcDistanceNoBorder calcDistance;
		Graph graph(calcDistance);
		graph.print_OPTTSP(coordinates, num_shops);
		return 0;
	}
}