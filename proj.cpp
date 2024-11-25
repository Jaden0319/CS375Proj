#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

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

// Data structure for heuristics
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

// Greedy Best-First Search Function
bool greedySearch(const vector<vector<int> >& grid, const Point& start, const Point& goal,
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
                double heuristicCost = heuristicFunc(next, goal, D, D2);
                pq.push((Node){next, 0, heuristicCost, heuristicCost}); // Greedy uses only the heuristic
            }
        }
    }

    return false; // No path found
}

// Display Grid
void displayGrid(const vector<vector<int> >& grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            cout << (grid[i][j] ? '|' : ' ') << ' ';
        }
        cout << endl;
    }
}

// Generate Random Maze-Like Grid
vector<vector<int> > generateRandomGrid(int rows, int cols, int obstacleDensity, Point start, Point goal) {
    vector<vector<int> > grid(rows, vector<int>(cols, 0));

    // Seed random number generator
    srand(time(0));

    // Randomly place obstacles
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (rand() % 100 < obstacleDensity) {
                grid[i][j] = 1; // Place an obstacle
            }
        }
    }

    // Ensure start and goal are open
    grid[start.x][start.y] = 0;
    grid[goal.x][goal.y] = 0;

    return grid;
}

// Compare Heuristics and Algorithms
void compareAlgorithms(const vector<vector<int> >& grid, const Point& start, const Point& goal) {
    HeuristicConfig heuristics[3] = {
        {"Manhattan", manhattanDistance, 1.0, 1.0},
        {"Diagonal", diagonalDistance, 1.0, sqrt(2)},
        {"Euclidean", euclideanDistance, 1.0, 1.0}
    };

    for (int i = 0; i < 3; ++i) {
        HeuristicConfig h = heuristics[i];
        int expandedNodesAStar = 0, expandedNodesGreedy = 0;

        chrono::high_resolution_clock::time_point startTimeAStar = chrono::high_resolution_clock::now();
        bool foundAStar = aStarSearch(grid, start, goal, h.heuristicFunc, h.D, h.D2, expandedNodesAStar);
        chrono::high_resolution_clock::time_point endTimeAStar = chrono::high_resolution_clock::now();

        chrono::high_resolution_clock::time_point startTimeGreedy = chrono::high_resolution_clock::now();
        bool foundGreedy = greedySearch(grid, start, goal, h.heuristicFunc, h.D, h.D2, expandedNodesGreedy);
        chrono::high_resolution_clock::time_point endTimeGreedy = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> elapsedAStar = endTimeAStar - startTimeAStar;
        chrono::duration<double, milli> elapsedGreedy = endTimeGreedy - startTimeGreedy;

        cout << h.name << " Heuristic:" << endl;
        cout << "  A* Algorithm:" << endl;
        cout << "    Path Found: " << (foundAStar ? "Yes" : "No") << endl;
        cout << "    Nodes Expanded: " << expandedNodesAStar << endl;
        cout << "    Time Taken: " << fixed << setprecision(2) << elapsedAStar.count() << " ms" << endl;
        cout << "  Greedy Algorithm:" << endl;
        cout << "    Path Found: " << (foundGreedy ? "Yes" : "No") << endl;
        cout << "    Nodes Expanded: " << expandedNodesGreedy << endl;
        cout << "    Time Taken: " << fixed << setprecision(2) << elapsedGreedy.count() << " ms" << endl;
        cout << "---------------------------------------" << endl;
    }
}

int main() {
    Point start1 = {0, 0}, goal1 = {29, 49};
    Point start2 = {0, 0}, goal2 = {39, 59};

    // Generate two random grids
    vector<vector<int> > randomGrid1 = generateRandomGrid(30, 50, 30, start1, goal1); // 30% obstacles
    vector<vector<int> > randomGrid2 = generateRandomGrid(40, 60, 40, start2, goal2); // 40% obstacles

    cout << "Random Maze-Like Grid 1:" << endl;
    displayGrid(randomGrid1);
    compareAlgorithms(randomGrid1, start1, goal1);

    cout << "\nRandom Maze-Like Grid 2:" << endl;
    displayGrid(randomGrid2);
    compareAlgorithms(randomGrid2, start2, goal2);

    return 0;
}



