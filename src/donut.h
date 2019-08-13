//Project Identifier : 8729BF7B2234189B8DF8A6DD31770D2B18569C27
#include <limits>
#include <algorithm>

using namespace std;

//PART A ONLY
class calcDistanceWithBorder{
public:
	double operator() (double x, double y, double x2, double y2) {
		double inf = numeric_limits<double>::infinity();
		double delta_x = x2 - x;
		double delta_y = y2 - y;
		if ((x == 0 && y > 0) || (y == 0 && x > 0) || (x == 0 && y == 0)) {//ON BORDER
			return sqrt((delta_x * delta_x) + (delta_y * delta_y));
		}
		else if (x > 0 && y > 0) {//IN CANADA
			if ((x2 > 0 && y2 > 0) || ((x2 == 0 && y2 > 0) || (y2 == 0 && x2 > 0) || (x2 == 0 && y2 == 0))) {//in canada or on border
				return sqrt((delta_x * delta_x) + (delta_y * delta_y));
			}
			//else its in america
			return inf;
		}
		else {//IN US
			if (!(x2 > 0 && y2 > 0)) {//in US or ON BORDER
				return sqrt((delta_x * delta_x) + (delta_y * delta_y));
			}
			//else its in canada
			return inf;
		}
	}
};
//Part B and C 
class calcDistanceNoBorder {
public:
	double operator() (double x, double y, double x2, double y2) {
		double delta_x = x2 - x;
		double delta_y = y2 - y;
		return sqrt((delta_x * delta_x) + (delta_y * delta_y));
	}
};

class Graph {
private:
	calcDistanceWithBorder distanceBorder;
	calcDistanceNoBorder calcDistance;

	//PART A
	struct Shop {
		double distance = numeric_limits<double>::infinity();
		int parentShop;
		bool visited = false;
	};
	//PART B
	struct ShopB {
		double x;
		double y;
		int shopID;
	};
	//PART C
	struct ShopC {
		double distance = numeric_limits<double>::infinity();
		int shopID;
		bool visited = false;
	};

	vector<vector<double> > adjMax;

	vector<int> path;

	vector<int> bestPath;

	double currBest = 0;
	double current_distance = 0;
	double distance = 0;

public:
	//PART A CONSTRUCTOR
	Graph(calcDistanceWithBorder distanceBorder_in)
		:distanceBorder(distanceBorder_in){}
	
	//PART B AND C CONSTRUCTOR
	Graph(calcDistanceNoBorder calcDistance_in)
		:calcDistance(calcDistance_in) {}

	int closest_shop(const vector<Shop> &shops) {
		double smallestDistance = -1;
		int smallestShop = -1;
		int shopsSize = (int)shops.size();
		bool found = false;

		for (int i = 0; i < shopsSize; ++i) {
			if ((shops[i].visited == false) && !found) {
				smallestDistance = shops[i].distance;
				smallestShop = i;
				found = true;
			}
			else if ((shops[i].visited == false) && found) {
				if (shops[i].distance < smallestDistance) {
					smallestDistance = shops[i].distance;
					smallestShop = i;
				}
			}

		}
		if (smallestDistance != -1) {
			return smallestShop;
		}
		
		return -1;

	}

	//PART A MST
	void print_MST(const vector<pair<int, int> >&coordinates, int num_shops) {
		double totalDistance = 0;

		vector<Shop> shops;
		shops.resize(num_shops);
		int shopsSize = (int)shops.size();

		double x = 0;
		double y = 0;
		double x2 = 0;
		double y2 = 0;
		double distance = 0;
		int shop_number = 0;

		int coordinates_size = (int)coordinates.size();
		shops[0].distance = 0;
		shops[0].parentShop = 0;
		Shop current_shop;
		int truth_counter = 0;
		while (truth_counter != num_shops) {
			//get closest unreached shop
			shop_number = closest_shop(shops);
			shops[shop_number].visited = true;
			++truth_counter;
			current_shop = shops[shop_number];
			totalDistance += current_shop.distance;

			x = coordinates[shop_number].first;
			y = coordinates[shop_number].second;

			for (int k = 0; k < coordinates_size; ++k) {
				if (!shops[k].visited) {
					x2 = coordinates[k].first;
					y2 = coordinates[k].second;
					distance = distanceBorder(x, y, x2, y2);
					
					if (shops[k].distance > distance) {
						shops[k].distance = distance;
						shops[k].parentShop = shop_number;
					}
				}
			}
		}//while
		cout << totalDistance << '\n';

		for (int i = 1; i < shopsSize; ++i) {
			if (shops[i].parentShop > i) {
				cout << i << ' ' << shops[i].parentShop << '\n';
			}
			else {
				cout << shops[i].parentShop << ' ' << i << '\n';
			}
		}
	}
	
	//PART B
	//Arbritrary Insertion Heuristic O(n^2)
	void print_FASTTSP(const vector<pair<int, int> > & coordinates, int num_shops) {
		vector<ShopB> current_shops;
		current_shops.reserve(num_shops);
		int coorSize = (int)coordinates.size();

		//INITIALIZATION
		double distance = 0;
		//push back first shop
		ShopB shop;
		shop.x = coordinates[0].first;
		shop.y = coordinates[0].second;
		shop.shopID = 0;
		current_shops.push_back(shop);

		//add second shop
		double lowest_distance = numeric_limits<double>::infinity();
		ShopB closest_shop;
		closest_shop.x = coordinates[1].first;
		closest_shop.y = coordinates[1].second;
		closest_shop.shopID = 1;
		for (int i = 1; i < coorSize; ++i) {
			distance = calcDistance(shop.x, shop.y, coordinates[i].first, coordinates[i].second);
			if (distance < lowest_distance) {
				lowest_distance = distance;
				closest_shop.x = coordinates[i].first;
				closest_shop.y = coordinates[i].second;
				closest_shop.shopID = i;
			}
		}
		
		current_shops.push_back(closest_shop);
		
		int skip_this_shop = closest_shop.shopID;

		ShopB new_shop;
		double distanceik;
		double distancekj;
		double distanceij;
		double smallestEdgeLength;
		double currentEdgeLength;
		int smallestj;
		auto it = current_shops.begin();

		
		for (int i = 1; i < coorSize; ++i) {
			//SELECTION
			if (i == skip_this_shop) {
				continue;
			}
			//k
			new_shop.x = coordinates[i].first;
			new_shop.y = coordinates[i].second;
			new_shop.shopID = i;
			smallestEdgeLength = numeric_limits<double>::infinity();
			smallestj = 0;
			int current_shops_size = (int)current_shops.size();
			for (int j = 0; j < current_shops_size; ++j) {
				//INSERTION PREPARATION 
				if (j != current_shops_size - 1) {
					distanceik = calcDistance(current_shops[j].x, current_shops[j].y, new_shop.x, new_shop.y);
					distancekj = calcDistance(new_shop.x, new_shop.y, current_shops[j + 1].x, current_shops[j + 1].y);
					distanceij = calcDistance(current_shops[j].x, current_shops[j].y, current_shops[j + 1].x, current_shops[j + 1].y);
				}
				else if (j == current_shops_size - 1){
					distanceik = calcDistance(current_shops[j].x, current_shops[j].y, new_shop.x, new_shop.y);
					distancekj = calcDistance(new_shop.x, new_shop.y, current_shops[0].x, current_shops[0].y);
					distanceij = calcDistance(current_shops[j].x, current_shops[j].y, current_shops[0].x, current_shops[0].y);
				}
				currentEdgeLength = distanceik + distancekj - distanceij;
				if (currentEdgeLength < smallestEdgeLength) {
					smallestEdgeLength = currentEdgeLength;
					smallestj = j + 1;
				}
			}
			//INSERTION
			it = current_shops.begin();
			current_shops.insert(it + smallestj, new_shop);
		}
		//PRINT PREPARATION
		double total_distance = 0;
		double current_distance = 0;
		int current_shops_size = (int)current_shops.size();
		for (int i = 0; i < current_shops_size - 1; ++i) {
			current_distance = calcDistance(current_shops[i].x, current_shops[i].y, current_shops[i + 1].x, current_shops[i + 1].y);
			total_distance += current_distance;
		}
		//the distance from the last node back to the beginning
		total_distance += calcDistance(current_shops[current_shops_size - 1].x, current_shops[current_shops_size - 1].y, current_shops[0].x, current_shops[0].y);

		//PRINT
		cout << total_distance << '\n';
		for (int i = 0; i < current_shops_size; ++i) {
			cout << current_shops[i].shopID << ' ';
		}
		cout << '\n';
		
		
	}//FASTTSP

	//PART C
	//Adapted Arbritrary Insertion Heuristic
	double print_FASTTSP_C(const vector<pair<int, int> > & coordinates, int num_shops) {
		vector<ShopB> current_shops;
		current_shops.reserve(num_shops);
		int coorSize = (int)coordinates.size();

		//INITIALIZATION
		//push back first shop
		ShopB shop;
		shop.x = coordinates[0].first;
		shop.y = coordinates[0].second;
		shop.shopID = 0;
		current_shops.push_back(shop);

		//push back second shop
		shop.x = coordinates[1].first;
		shop.y = coordinates[1].second;
		shop.shopID = 1;
		current_shops.push_back(shop);

		//push back third shop
		shop.x = coordinates[2].first;
		shop.y = coordinates[2].second;
		shop.shopID = 2;
		current_shops.push_back(shop);

		ShopB new_shop;
		double distanceik;
		double distancekj;
		double distanceij;
		double smallestEdgeLength;
		double currentEdgeLength;
		int smallestj;
		auto it = current_shops.begin();

		for (int i = 3; i < coorSize; ++i) {
			//SELECTION
			new_shop.x = coordinates[i].first;
			new_shop.y = coordinates[i].second;
			new_shop.shopID = i;
			smallestEdgeLength = numeric_limits<double>::infinity();
			smallestj = 0;

			int current_shops_size = (int)current_shops.size();
			for (int j = 0; j < current_shops_size; ++j) {
				//INSERTION PREPARATION 
				if (j != current_shops_size - 1) {
					distanceik = calcDistance(current_shops[j].x, current_shops[j].y, new_shop.x, new_shop.y);
					distancekj = calcDistance(new_shop.x, new_shop.y, current_shops[j + 1].x, current_shops[j + 1].y);
					distanceij = calcDistance(current_shops[j].x, current_shops[j].y, current_shops[j + 1].x, current_shops[j + 1].y);
				}
				else if (j == current_shops_size - 1) {
					distanceik = calcDistance(current_shops[j].x, current_shops[j].y, new_shop.x, new_shop.y);
					distancekj = calcDistance(new_shop.x, new_shop.y, current_shops[0].x, current_shops[0].y);
					distanceij = calcDistance(current_shops[j].x, current_shops[j].y, current_shops[0].x, current_shops[0].y);
				}
				currentEdgeLength = distanceik + distancekj - distanceij;
				if (currentEdgeLength < smallestEdgeLength) {
					smallestEdgeLength = currentEdgeLength;
					smallestj = j + 1;
				}
			}

			it = current_shops.begin();
			current_shops.insert(it + smallestj, new_shop);
		}
		//PRINT PREPARATION
		double total_distance = 0;
		double current_distance = 0;
		int current_shops_size = (int)current_shops.size();
		for (int i = 0; i < current_shops_size - 1; ++i) {
			current_distance = calcDistance(current_shops[i].x, current_shops[i].y, current_shops[i + 1].x, current_shops[i + 1].y);
			total_distance += current_distance;
		}
		//the distance from the last node back to the beginning
		total_distance += calcDistance(current_shops[current_shops_size - 1].x, current_shops[current_shops_size - 1].y, current_shops[0].x, current_shops[0].y);

		//return
		path.reserve(current_shops_size);
		bestPath.reserve(current_shops_size);
		for (int i = 0; i < current_shops_size; ++i) {
			path.push_back(current_shops[i].shopID);
			bestPath.push_back(current_shops[i].shopID);
		}
		
		return total_distance;
	}//FASTTSP_C

	void init_adjMax(const vector<pair<int, int> > &coordinates, int num_shops) {
		vector<vector<double> > adjMax_in(num_shops, vector<double>(num_shops, -1)); 
		double x;
		double y;
		double x2;
		double y2;
		double distance = 0;
		int coordinates_size = (int)coordinates.size();
		for (int i = 0; i < coordinates_size; ++i) {
			x = coordinates[i].first;
			y = coordinates[i].second;
			for (int j = 0; j < coordinates_size; ++j) {
				x2 = coordinates[j].first;
				y2 = coordinates[j].second;
				if (i != j) {
					distance = calcDistance(x, y, x2, y2);
					adjMax_in[i][j] = distance;
				}
				else if (i == j) {
					adjMax_in[i][j] = 0;
				}
			}
		}
		adjMax = adjMax_in;
	}

	//DONE O(n)
	double closest_shop_distance(int row, size_t start_here) {
		double smallest_distance = numeric_limits<double>::infinity();
		size_t unconnectedSize = (path.size() - start_here);
		size_t i = start_here;
		size_t counter = 0;
		while (counter != unconnectedSize) {
			if (adjMax[row][path[i]] < smallest_distance) {
				if (row != path[i]) {
					smallest_distance = adjMax[row][path[i]];
				}
			}
			++counter;
			++i;
		}
		return smallest_distance;
	}
	//part c
	
	int closest_shop(const vector<ShopC> &shops) {
		double smallestDistance = numeric_limits<double>::infinity();
		int smallestShop = -1;
		int shopsSize = (int)shops.size();
		bool found = false;

		for (int i = 0; i < shopsSize; ++i) {
			if (!shops[i].visited && !found) {
				smallestDistance = shops[i].distance;
				smallestShop = i;
				found = true;
			}
			else if (!shops[i].visited && found) {
				if (shops[i].distance < smallestDistance) {
					smallestDistance = shops[i].distance;
					smallestShop = i;
				}
			}
			

		}
		if (smallestDistance != -1) {
			return smallestShop;
		}
		return -1;
	}

	double return_sub_MST(size_t start_here) {
		vector<ShopC> unconnectedShops;
		size_t size = path.size() - start_here;
		unconnectedShops.resize(size);

		size_t counter = 0;
		size_t i = start_here;
		while (counter != size) {
			unconnectedShops[counter].shopID = path[i];
			++counter;
			++i;
		}

		double total_distance = 0;
		double distance = 0;
		int shop_number = 0;
		int num_shops = (int)unconnectedShops.size();
		unconnectedShops[0].distance = 0;
		ShopC current_shop;
		int current_shop_ID = 0;
		auto it = unconnectedShops.begin();
		while (!unconnectedShops.empty()) {
			//get closest unreached shop
			shop_number = closest_shop(unconnectedShops);
			it = unconnectedShops.begin() + shop_number;
			current_shop = unconnectedShops[shop_number];
			current_shop_ID = unconnectedShops[shop_number].shopID;
			total_distance += current_shop.distance;
			unconnectedShops.erase(it);
			num_shops = (int)unconnectedShops.size();
			//update adjacent neighbors
			for (int i = 0; i < num_shops; ++i) {
				distance = adjMax[current_shop_ID][unconnectedShops[i].shopID];
				if (unconnectedShops[i].distance > distance) {
					unconnectedShops[i].distance = distance;
				}
			}
		}//while

		return total_distance;
	}
	
	//DONE O(n^2 + 2n) because of closest_shop_distance and MST
	double lower_bound(size_t start_here) {
		//calculate two paths to make full tour 
		int start_shop_id = path[0];
		int current_last_shop_id = path[start_here - 1];
		double connecting_distances = closest_shop_distance(current_last_shop_id, start_here) + closest_shop_distance(start_shop_id, start_here); 
		return current_distance + connecting_distances + return_sub_MST(start_here);
	}

	//DONE O(n^2 + 2n) because of lower_bound
	bool promising(size_t start_here) {
		if ((path.size() - start_here) <= 3) {
			return true;
		}
		distance = lower_bound(start_here);
		if (distance >= currBest) {
			return false;
		}
		return true;
	}

	//DONE O(n!)
	void genPerms(size_t permLength) {
		if (path.size() == permLength) {
			current_distance += adjMax[path[path.size() - 1]][path[0]];
			if (current_distance < currBest) {
				currBest = current_distance; 
				bestPath = path;
			}
			current_distance -= adjMax[path[path.size() - 1]][path[0]];
			return;
		} 
		if (!promising(permLength))
			return;
		for (size_t i = permLength; i < path.size(); ++i) {
			swap(path[permLength], path[i]);
			current_distance += adjMax[path[permLength - 1]][path[permLength]];
			genPerms(permLength + 1);
			current_distance -= adjMax[path[permLength - 1]][path[permLength]];
			swap(path[permLength], path[i]);
		} // for
	} // genPerms()

	//DONE O(n! + n^2) because of genPerms and FASTTSP
	void print_OPTTSP(const vector<pair<int,int> > &coordinates, int num_shops) {
		init_adjMax(coordinates, num_shops);

		currBest = print_FASTTSP_C(coordinates, num_shops);
		size_t permLength = 1;
		genPerms(permLength);
		cout << currBest << '\n';
		for (int i = 0; i < (int)bestPath.size(); ++i) {
			cout << bestPath[i] << ' ';
		}
		cout << '\n';
	}
};