
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


#include <iostream>
#include <cstring>

std::vector<JPSPlus::direction> JPSPlus::validDirections[DIRCOUNT] = {
	{ WEST, NORTHWEST, NORTH, NORTHEAST, EAST }, /* NORTH */
	{ WEST, SOUTHWEST, SOUTH, SOUTHEAST, EAST }, /* SOUTH */
	{ NORTH, NORTHWEST, WEST, SOUTHWEST, SOUTH }, /* WEST */
	{ WEST, SOUTHWEST, SOUTH, SOUTHEAST, EAST}, /* SOUTH */
	{ NORTH, WEST, NORTHWEST }, /* NORTHWEST */
	{ NORTH, EAST, NORTHEAST }, /* NORTHEAST */
	{ SOUTH, WEST, SOUTHWEST }, /* SOUTHWEST */
	{ SOUTH, EAST, SOUTHEAST } /* SOUTHEAST */
};

JPSPlus::JPSPlus() {
	std::memset(jumpPoint, 0, sizeof(jumpPoint));
}

void JPSPlus::read() {
	std::cin >> mapWidth >> mapHeight;
	for (int i = 0; i < mapHeight; ++i) {
		std::string row;
		std::cin >> row;
		assert(static_cast<int>(row.size()) == mapWidth);

		for (int j = 0; j < mapWidth; ++j)
			wall[i][j] = row[j] == '#';
	}
}

void JPSPlus::preprocessing() {
	// printMap();
	calculatePrimaryJumpPoints();
	// printAllPrimaryJumpPoints();
	calculateStraightJumpPoints();
	calculateDiagonalJumpPoints();
	printDistances();
}

void JPSPlus::calculatePrimaryJumpPoints() {
	for (direction dir : {NORTH, SOUTH, WEST, EAST}) {
		int dr = drow[dir];
		int dc = dcol[dir];
		for (int r = 0; r < mapHeight; ++r)
			for (int c = 0; c < mapWidth; ++c)
				if (!isWall(r, c)) {
					int pr = r - dr, pc = c - dc;
					if (!inBounds(pr, pc) || isWall(pr, pc))
						continue;

					int lpr = pr + dc, lpc = pc + dr;
					int lr = r + dc, lc = c + dr;
					if (inBounds(lpr, lpc) && inBounds(lr, lc) && isWall(lpr, lpc) && !isWall(lr, lc)) {
						jumpPoint[r][c][dir] = true;
						continue;
					}

					int rpr = pr - dc, rpc = pc - dr;
					int rr = r - dc, rc = c - dr;
					if (inBounds(rpr, rpc) && inBounds(rr, rc) && isWall(rpr, rpc) && !isWall(rr, rc))
						jumpPoint[r][c][dir] = true;
				}
	}
}

void JPSPlus::calculateStraightJumpPoints() {
	/* NORTH and WEST cardinal directions */
	for (direction dir : {NORTH, WEST}) {
		int dr = drow[dir];
		int dc = dcol[dir];

		for (int r = 0; r < mapHeight; ++r)
			for (int c = 0; c < mapWidth; ++c)
				if (!isWall(r, c)) {
					int pr = r + dr;
					int pc = c + dc;
					if (!inBounds(pr, pc) || isWall(pr, pc))
						distance[r][c][dir] = 0;
					else if (jumpPoint[pr][pc][dir])
						distance[r][c][dir] = 1;
					else {
						int d = distance[pr][pc][dir];
						distance[r][c][dir] = d + (d <= 0 ? -1 : 1);
					}
				}
	}

	/* SOUTH and EAST cardinal directions */
	for (direction dir : {SOUTH, EAST}) {
		int dr = drow[dir];
		int dc = dcol[dir];

		for (int r = mapHeight - 1; r >= 0; --r)
			for (int c = mapWidth - 1; c >= 0; --c)
				if (!isWall(r, c)) {
					int pr = r + dr;
					int pc = c + dc;
					if (!inBounds(pr, pc) || isWall(pr, pc))
						distance[r][c][dir] = 0;
					else if (jumpPoint[pr][pc][dir])
						distance[r][c][dir] = 1;
					else {
						int d = distance[pr][pc][dir];
						distance[r][c][dir] = d + (d <= 0 ? -1 : 1);
					}
				}
	}
}

void JPSPlus::calculateDiagonalJumpPoints() {
	/* NORTHWEST and NORTHEAST diagonal directions */
	for (direction dir : {NORTHWEST, NORTHEAST}) {
		int dr = drow[dir];
		int dc = dcol[dir];

		for (int r = 0; r < mapHeight; ++r)
			for (int c = 0; c < mapWidth; ++c)
				if (!isWall(r, c)) {
					int pr = r + dr;
					int pc = c + dc;

					if (!inBounds(r + dr, c) || !inBounds(r, c + dc) || !inBounds(pr, pc) ||
						isWall(r + dr, c) || isWall(r, c + dc) || isWall(pr, pc))
						distance[r][c][dir] = 0;
					else {
						assert(validDirections[dir].size() >= 2);
						int reldir1 = validDirections[dir][0];
						int reldir2 = validDirections[dir][1];

						if (distance[pr][pc][reldir1] > 0 ||
							distance[pr][pc][reldir2] > 0)
							distance[r][c][dir] = 1;
						else {
							int d = distance[pr][pc][dir];
							distance[r][c][dir] = d + (d <= 0 ? -1 : 1);
						}
					}
				}
	}

	/* SOUTHWEST and SOUTHEAST diagonal directions */
	for (direction dir : {SOUTHWEST, SOUTHEAST}) {
		int dr = drow[dir];
		int dc = dcol[dir];

		for (int r = mapHeight - 1; r >= 0; --r)
			for (int c = 0; c < mapWidth; ++c)
				if (!isWall(r, c)) {
					int pr = r + dr;
					int pc = c + dc;

					if (!inBounds(r + dr, c) || !inBounds(r, c + dc) || !inBounds(pr, pc) ||
						isWall(r + dr, c) || isWall(r, c + dc) || isWall(pr, pc))
						distance[r][c][dir] = 0;
					else {
						assert(validDirections[dir].size() >= 2);
						int reldir1 = validDirections[dir][0];
						int reldir2 = validDirections[dir][1];

						if (distance[pr][pc][reldir1] > 0 ||
							distance[pr][pc][reldir2] > 0)
							distance[r][c][dir] = 1;
						else {
							int d = distance[pr][pc][dir];
							distance[r][c][dir] = d + (d <= 0 ? -1 : 1);
						}
					}
				}
	}
}

void JPSPlus::printMap() {
	printf("  ");
	for (int c = 0; c < mapWidth; ++c)
		printf("%2d", c);
	puts("");
	for (int r = 0; r < mapHeight; ++r) {
		printf("%2d", r);
		for (int c = 0; c < mapWidth; ++c)
			printf("%2s", isWall(r, c) ? "#" : ".");
		puts("");
	}
	puts("");
}

void JPSPlus::printAllPrimaryJumpPoints() {
	for (int r = 0; r < mapHeight; ++r)
		for (int c = 0; c < mapWidth; ++c)
			for (direction dir : {NORTH, SOUTH, WEST, EAST})
				if (jumpPoint[r][c][dir])
					printMapWithPrimaryJumpPoint(r, c, dir);
}

void JPSPlus::printMapWithPrimaryJumpPoint(int jr, int jc, direction dir) {
	int pr = jr - drow[dir], pc = jc - dcol[dir];
	printf("  ");
	for (int c = 0; c < mapWidth; ++c)
		printf("%2d", c);
	puts("");
	for (int r = 0; r < mapHeight; ++r) {
		printf("%2d", r);
		for (int c = 0; c < mapWidth; ++c)
			if (r == jr && c == jc)
				printf("%2s", "N");
			else if (r == pr && c == pc)
				printf("%2s", "P");
			else
				printf("%2s", isWall(r, c) ? "#" : ".");
		puts("");
	}
	printf("(%d, %d, %s)\n\n", jr, jc, dirToStr(dir).c_str());
}

void JPSPlus::printDistances() {
	for (int r = 0; r < mapHeight; ++r)
		for (int c = 0; c < mapWidth; ++c)
			if (!isWall(r, c)) {
				printf("%d %d", c, r);
				for (direction dir : ALLDIRS)
					printf(" %d", distance[r][c][dir]);
				puts("");
			}

}

std::string JPSPlus::dirToStr(const direction& dir) {
	switch (dir) {
		case NORTH:
			return "NORTH";
		case SOUTH:
			return "SOUTH";
		case WEST:
			return "WEST";
		case EAST:
			return "EAST";
		case NORTHWEST:
			return "NORTHWEST";
		case NORTHEAST:
			return "NORTHEAST";
		case SOUTHWEST:
			return "SOUTHWEST";
		case SOUTHEAST:
			return "SOUTHEAST";
	}
	return "UNKNOWN";
}

int main() {
	JPSPlus jpsPlus;
	jpsPlus.read();
	jpsPlus.preprocessing();

	return 0;
}
