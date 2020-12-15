#ifndef JPSPLUS_HPP
#define JPSPLUS_HPP

#include <cassert>
#include <string>
#include <vector>

#define MAPSIZE 20

class JPSPlus {
public:
	JPSPlus();
	void read();
	void preprocessing();

private:
	enum direction {
		NORTH = 0, SOUTH, WEST, EAST,
		NORTHWEST, NORTHEAST, SOUTHWEST, SOUTHEAST
	};

	void calculatePrimaryJumpPoints();
	void calculateStraightJumpPoints();
	void calculateDiagonalJumpPoints();

	inline bool inBounds(const int& r, const int& c);
	inline bool isWall(const int& r, const int& c);

	void printMap();
	void printAllPrimaryJumpPoints();
	void printMapWithPrimaryJumpPoint(int jr, int jc, direction dir);
	void printDistances();

	std::string dirToStr(const direction& dir);

private:
	int mapWidth;
	int mapHeight;
	bool wall[MAPSIZE][MAPSIZE];

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

	static std::vector<direction> validDirections[DIRCOUNT];
	bool jumpPoint[MAPSIZE][MAPSIZE][4];
	int distance[MAPSIZE][MAPSIZE][DIRCOUNT];
};

bool JPSPlus::inBounds(const int& r, const int& c) {
	return 0 <= r && r < mapHeight && 0 <= c && c < mapWidth;
}

bool JPSPlus::isWall(const int& r, const int& c) {
	assert(inBounds(r, c));
	return wall[r][c];
}

#endif /* JPSPLUS_HPP */
