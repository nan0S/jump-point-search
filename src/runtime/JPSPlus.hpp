#ifndef JPSPLUS_HPP
#define JPSPLUS_HPP

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <cmath>

#define MAPSIZE 20

class JPSPlus {
public:
	void read();
	void run();

private:
	enum direction {
		NORTH = 0, SOUTH, WEST, EAST,
		NORTHWEST, NORTHEAST, SOUTHWEST, SOUTHEAST,
		NONE
	};

	struct Node {
		int row, col;
		int prow, pcol;
		direction dir;
		double sortCost;

		bool operator<(const Node& o) const;
	};

	inline bool inBounds(const int& r, const int& c);
	inline bool isCardinal(const direction& dir);
	inline int sign(const int& x);
	inline double heuristic(const int& row, const int& col);

	std::string dirToStr(const direction& dir);

private:
	int mapWidth;
	int mapHeight;

	int startRow;
	int startCol;
	int goalRow;
	int goalCol;

	static constexpr int DIRCOUNT = 8;
	static constexpr direction ALLDIRS[DIRCOUNT] = {
		NORTH, NORTHEAST, EAST, SOUTHEAST,
		SOUTH, SOUTHWEST, WEST, NORTHWEST
	};
	static constexpr int drow[DIRCOUNT] = {
		[NORTH]=-1, [SOUTH]=1, [WEST]=0, [EAST]=0,
		[NORTHWEST]=-1, [NORTHEAST]=-1, [SOUTHWEST]=1, [SOUTHEAST]=1,
	};
	static constexpr int dcol[DIRCOUNT] = {
		[NORTH]=0, [SOUTH]=0, [WEST]=-1, [EAST]=1,
		[NORTHWEST]=-1, [NORTHEAST]=1, [SOUTHWEST]=-1, [SOUTHEAST]=1,
	};
	static std::vector<direction> validDirections[DIRCOUNT+1];

	int distances[MAPSIZE][MAPSIZE][DIRCOUNT];
	bool visited[MAPSIZE][MAPSIZE];
	double distanceToGoal[MAPSIZE][MAPSIZE];

	static const double SQRT2;
};

bool JPSPlus::inBounds(const int& r, const int& c) {
	return 0 <= r && r < mapHeight && 0 <= c && c < mapWidth;
}

bool JPSPlus::isCardinal(const direction& dir) {
	switch (dir) {
		case NORTH:
		case WEST:
		case EAST:
		case SOUTH:
			return true;
		default:
			return false;
	}
}

int JPSPlus::sign(const int& x) {
	return x ? (x >> 31 | 1) : 0;
}

double JPSPlus::heuristic(const int& row, const int& col) {
	int dr = abs(row - goalRow);
	int dc = abs(col - goalCol);
	return std::max(dr, dc) + std::min(dr, dc) * (SQRT2 - 1);
}

#endif /* JPSPLUS_HPP */
