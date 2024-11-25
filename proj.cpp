#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

struct Point {
    int x, y;
};

// Priority queue node
struct Node {
    Point point;
    double cost, heuristic, totalCost;
    bool operator<(const Node& other) const {
        return totalCost > other.totalCost; // Priority for lower total cost
    }
};

// Heuristics
double manhattanDistance(const Point& a, const Point& b, double D, double /*unused*/) {
    return D * (abs(a.x - b.x) + abs(a.y - b.y));
}

double diagonalDistance(const Point& a, const Point& b, double D, double D2) {
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    return D * (dx + dy) + (D2 - 2 * D) * min(dx, dy);
}

double euclideanDistance(const Point& a, const Point& b, double D, double /*unused*/) {
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    return D * sqrt(dx * dx + dy * dy);
}

// Data structure to replace std::tuple
struct HeuristicConfig {
    string name;
    double (*heuristicFunc)(const Point&, const Point&, double, double);
    double D, D2;
};

// A* Search Function
bool aStarSearch(const vector<vector<int> >& grid, const Point& start, const Point& goal,
                 double (*heuristicFunc)(const Point&, const Point&, double, double),
                 double D, double D2, int& expandedNodes) {
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<bool> > visited(rows, vector<bool>(cols, false));
    priority_queue<Node> pq;

    pq.push((Node){start, 0, heuristicFunc(start, goal, D, D2), heuristicFunc(start, goal, D, D2)});
    expandedNodes = 0;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.point.x == goal.x && current.point.y == goal.y) {
            return true; // Goal found
        }

        if (visited[current.point.x][current.point.y]) continue;
        visited[current.point.x][current.point.y] = true;
        expandedNodes++;

        // Movement directions
        Point directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
        for (int i = 0; i < 8; ++i) {
            Point dir = directions[i];
            int newX = current.point.x + dir.x;
            int newY = current.point.y + dir.y;

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && grid[newX][newY] == 0) {
                Point next = {newX, newY};
                double stepCost = (dir.x == 0 || dir.y == 0) ? D : D2; // Straight or diagonal
                double newCost = current.cost + stepCost;
                double heuristicCost = heuristicFunc(next, goal, D, D2);
                pq.push((Node){next, newCost, heuristicCost, newCost + heuristicCost});
            }
        }
    }

    return false; // No path found
}

// Compare Heuristics
void compareHeuristics(const vector<vector<int> >& grid, const Point& start, const Point& goal) {
    HeuristicConfig heuristics[3] = {
        {"Manhattan", manhattanDistance, 1.0, 1.0},
        {"Diagonal", diagonalDistance, 1.0, sqrt(2)},
        {"Euclidean", euclideanDistance, 1.0, 1.0}
    };

    for (int i = 0; i < 3; ++i) {
        HeuristicConfig h = heuristics[i];
        int expandedNodes = 0;

        chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
        bool found = aStarSearch(grid, start, goal, h.heuristicFunc, h.D, h.D2, expandedNodes);
        chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> elapsed = endTime - startTime;

        cout << h.name << " Heuristic:" << endl;
        cout << "  Path Found: " << (found ? "Yes" : "No") << endl;
        cout << "  Nodes Expanded: " << expandedNodes << endl;
        cout << "  Time Taken: " << fixed << setprecision(2) << elapsed.count() << " ms" << endl;
        cout << "---------------------------------------" << endl;
    }
}

int main() {
    // Larger Grid Structures
    vector<vector<int> > grid1(10, vector<int>(10, 0));
    for (int i = 2; i <= 7; ++i) grid1[i][5] = 1;

    vector<vector<int> > grid2(15, vector<int>(15, 0));
    for (int i = 0; i < 15; ++i) grid2[7][i] = 1;
    for (int j = 5; j < 10; ++j) grid2[j][7] = 1;

    vector<vector<int> > grid3(20, vector<int>(20, 0));
    for (int i = 5; i < 15; ++i) grid3[i][10] = 1;
    for (int j = 8; j < 12; ++j) grid3[10][j] = 1;

    Point start1 = {0, 0}, goal1 = {9, 9};
    Point start2 = {0, 0}, goal2 = {14, 14};
    Point start3 = {0, 0}, goal3 = {19, 19};

    cout << "Grid 1 Results:" << endl;
    compareHeuristics(grid1, start1, goal1);

    cout << "\nGrid 2 Results:" << endl;
    compareHeuristics(grid2, start2, goal2);

    cout << "\nGrid 3 Results:" << endl;
    compareHeuristics(grid3, start3, goal3);

    return 0;
}

