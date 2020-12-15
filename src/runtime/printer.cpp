#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Compute the nodes visited by the JPS+ algorithm when performing runtime phase search.
 **/

int main()
{
    int width; // Width of the map
    int height; // Height of the map
    cin >> width >> height; cin.ignore();
    cerr << width << " " << height << endl;
    int startColumn; // coordinate of the starting tile
    int startRow; // coordinate of the starting tile
    int goalColumn; // coordinate of the goal tile
    int goalRow; // coordinate of the goal tile
    cin >> startColumn >> startRow >> goalColumn >> goalRow; cin.ignore();
    cerr << startColumn << " " << startRow << " " << goalColumn << " " << goalRow << endl;
    int open; // number of open tiles on the map
    cin >> open; cin.ignore();
    cerr << open << endl;
    for (int i = 0; i < open; i++) {
        int column; // coordinate of the empty tile described
        int row; // coordinate of the empty tile described
        int N; // distance to the closest jump point (positive number) or wall (otherwise) going north
        int NE; // distance to the closest jump point (positive number) or wall (otherwise) going northeast
        int E; // distance to the closest jump point (positive number) or wall (otherwise) going east
        int SE; // distance to the closest jump point (positive number) or wall (otherwise) going southeast
        int S; // distance to the closest jump point (positive number) or wall (otherwise) going south
        int SW; // distance to the closest jump point (positive number) or wall (otherwise) going southwest
        int W; // distance to the closest jump point (positive number) or wall (otherwise) going west
        int NW; // distance to the closest jump point (positive number) or wall (otherwise) going northwest
        cin >> column >> row >> N >> NE >> E >> SE >> S >> SW >> W >> NW; cin.ignore();
        cerr << column << " " << row << " " << N << " " << NE << " " << E << " " << SE << " " << S << " " << SW << " " << W << " " << NW << endl;
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;


    // In order of nodes visited by the JPS+ algorithm, a line containing "nodeColumn nodeRow parentColumn parentRow givenCost".
    cout << "startColumn startRow -1 -1 0.00" << endl;

    // game loop
    while (1) {

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // In order of nodes visited by the JPS+ algorithm, a line containing "nodeColumn nodeRow parentColumn parentRow givenCost".
        cout << "3 4 0 2 3.14" << endl;
    }
}
