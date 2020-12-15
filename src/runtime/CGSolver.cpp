

#include <iostream>

#if defined(LOCAL) && !defined(NDEBUG)
#define DEBUG
#endif

#ifdef DEBUG

inline void __debug(const char* s, const char*) {
    std::cerr << s << ": ";
}

template<typename T>
void __debug(const char* s, const T& x) {
    std::cerr << s << ": " << x << " ";
}

template<typename T, typename... Args>
void __debug(const char* s, const T& x, const Args&... rest) {
    int bracket = 0;
    char c;
    while ((c = *s) != ',' || bracket)
    {
        std::cerr << *s++;
        switch (c)
        {
            case '(':
            case '{':
            case '[':
                ++bracket;
                break;
            case ')':
            case '}':
            case ']':
                --bracket;
        }
    }
    std::cerr << ": ";
    std::cerr << x << ",";
    __debug(s + 1, rest...);

}

template<typename... Args>
void _debug(const char* s, const Args&... rest) {
    __debug(s, rest...);
    std::cerr << std::endl;
}

#define debug(...) _debug(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) 13
#endif


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


#include <iostream>
#include <iomanip>
#include <cstring>
#include <queue>

std::vector<JPSPlus::direction> JPSPlus::validDirections[DIRCOUNT+1] = {
	{ WEST, NORTHWEST, NORTH, NORTHEAST, EAST }, /* NORTH */
	{ WEST, SOUTHWEST, SOUTH, SOUTHEAST, EAST }, /* SOUTH */
	{ NORTH, NORTHWEST, WEST, SOUTHWEST, SOUTH }, /* WEST */
	{ NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH }, /* EAST */
	{ NORTH, WEST, NORTHWEST }, /* NORTHWEST */
	{ NORTH, EAST, NORTHEAST }, /* NORTHEAST */
	{ SOUTH, WEST, SOUTHWEST }, /* SOUTHWEST */
	{ SOUTH, EAST, SOUTHEAST }, /* SOUTHEAST */
	{ NORTH, SOUTH, WEST, EAST, NORTHWEST, NORTHEAST, SOUTHWEST, SOUTHEAST } /* NONE */
};

const double JPSPlus::SQRT2 = std::sqrt(2.0);

bool JPSPlus::Node::operator<(const Node &o) const {
	std::cerr << std::setprecision(12);
	debug(col, row, o.col, o.row, sortCost, o.sortCost, sortCost == o.sortCost);
	std::cerr << std::setprecision(2);
	return o.sortCost < sortCost;
}

void JPSPlus::read() {
	std::cin >> mapWidth >> mapHeight;
	std::cin >> startCol >> startRow;
	std::cin >> goalCol >> goalRow;

	int open;
	std::cin >> open;

	for (int i = 0; i < open; ++i) {
		int col, row;
		std::cin >> col >> row;
		for (const auto& dir : ALLDIRS)
			std::cin >> distances[row][col][dir];
	}

}

void JPSPlus::run() {
	std::cout << std::fixed << std::setprecision(2);

	std::memset(visited, 0, sizeof(visited));
	for (int r = 0; r < mapHeight; ++r)
		std::fill(distanceToGoal[r], distanceToGoal[r] + MAPSIZE, INFINITY);

	Node start{startRow, startCol, -1, -1, NONE, heuristic(startRow, startCol) };
	distanceToGoal[startRow][startCol] = 0;
	std::priority_queue<Node> openList;
	openList.push(start);

	while (!openList.empty()) {
		Node curNode = openList.top();
		openList.pop();

		if (visited[curNode.row][curNode.col])
			continue;
		visited[curNode.row][curNode.col] = true;

		double curDist = distanceToGoal[curNode.row][curNode.col];
		std::cout << curNode.col << " " << curNode.row << " "
				  << curNode.pcol << " " << curNode.prow << " "
				  << curDist << std::endl;
		debug(curNode.sortCost);

		if (curNode.row == goalRow && curNode.col == goalCol)
			return;

		int toGoalDiffRow = goalRow - curNode.row;
		int toGoalDiffCol = goalCol - curNode.col;

		for (const auto& dir : validDirections[curNode.dir]) {
			int succRow = -1, succCol = -1;
			double givenCost = -1;

			bool isDirCardinal = isCardinal(dir);
			int dr = drow[dir];
			int dc = dcol[dir];
			int dist = abs(distances[curNode.row][curNode.col][dir]);
			bool inDirectionRow = sign(toGoalDiffRow) == dr;
			bool inDirectionCol = sign(toGoalDiffCol) == dc;

			debug(isDirCardinal, dr, dc, dist, inDirectionRow, inDirectionCol, dirToStr(dir));

			if (isDirCardinal && inDirectionRow && inDirectionCol &&
				abs(toGoalDiffRow) + abs(toGoalDiffCol) <= dist) {
				succRow = goalRow;
				succCol = goalCol;
				givenCost = curDist + abs(toGoalDiffRow) + abs(toGoalDiffCol);
			}
			else if (!isDirCardinal && inDirectionRow && inDirectionCol &&
				(abs(toGoalDiffRow) <= dist || abs(toGoalDiffCol) <= dist)) {
				int minToGoalDiff = std::min(abs(toGoalDiffRow), abs(toGoalDiffCol));
				succRow = curNode.row + dr * minToGoalDiff;
				succCol = curNode.col + dc * minToGoalDiff;
				givenCost = curDist + minToGoalDiff * SQRT2;
			}
			else if (distances[curNode.row][curNode.col][dir] > 0) {
				succRow = curNode.row + dr * dist;
				succCol = curNode.col + dc * dist;
				givenCost = curDist + (isDirCardinal ? dist : SQRT2 * dist);
			}

			assert((succRow == -1 && succCol == -1) || (succRow >= 0 && succCol >= 0
				&& succRow < mapHeight && succCol < mapWidth));
			if (succRow != -1 && succCol != -1) {
				assert(givenCost != -1.f);
				if (givenCost < distanceToGoal[succRow][succCol]) {
					distanceToGoal[succRow][succCol] = givenCost;
					openList.push({succRow, succCol, curNode.row, curNode.col, dir, givenCost + heuristic(succRow, succCol)});
				}
			}
		}
	}

	std::cout << "NO PATH" << std::endl;
}

std::string JPSPlus::dirToStr(const direction& dir) {
	switch (dir) {
		case NORTH: return "N";
		case SOUTH: return "S";
		case WEST: return "W";
		case EAST: return "E";
		case NORTHWEST: return "NW";
		case NORTHEAST: return "NE";
		case SOUTHWEST: return "SW";
		case SOUTHEAST: return "SE";
		default: return "UKNOWN";
	}
}

int main() {
	JPSPlus jpsPlus;
	jpsPlus.read();
	jpsPlus.run();

	return 0;
}
