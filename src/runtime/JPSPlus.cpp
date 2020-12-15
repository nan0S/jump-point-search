#include "JPSPlus.hpp"
#include "Common.hpp"

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
