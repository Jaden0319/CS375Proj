#include <iostream>
#include <utility>
#include <cmath>
#include <stack>
#include <set>
#include <cfloat>
#include <cstring>


using namespace std;

#define ROW 9  
#define COL 10


typedef pair<int, int> Pair;


typedef pair<double, pair<int, int> > pPair;

struct cell {
   
    int parent_i, parent_j;
    double f, g, h;
};

bool isValid(int row, int col) {
  
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}


bool isUnBlocked(int grid[][COL], int row, int col) {
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}


bool isDestination(int row, int col, Pair dest) {
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

double calculateHValue(int row, int col, Pair dest, int h) {
    
    if(h == 0) {  //Euclidean
        return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
    } 
    else if(h == 1) { //manhattan
        return abs(row - dest.first) + abs(col - dest.second);
    }
    else  { 
        int dx = abs(row - dest.first);
        int dy = abs(col - dest.second);
        double D = 1.0;         // Cost of straight movement
        double D2 = 1.414;     // Cost of diagonal movement
        return D * (dx + dy) + (D2 - 2 * D) * min(dx, dy); //diagonal
    }
    
}

void tracePath(cell cellDetails[][COL], Pair dest) {
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
    }

    return;
}

void aStarSearch(int grid[][COL], Pair src, Pair dest, int hvalue) {
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }

    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }

    if (isUnBlocked(grid, src.first, src.second) == false
        || isUnBlocked(grid, dest.first, dest.second)
               == false) {
        printf("Source or the destination is blocked\n");
        return;
    }

    if (isDestination(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return;
    }

    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    cell cellDetails[ROW][COL];

    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    set<pPair> openList;

    openList.insert(make_pair(0.0, make_pair(i, j)));

    bool foundDest = false;
    int nodesExpanded = 0;

    while (!openList.empty()) {
        pPair p = *openList.begin();
        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
        nodesExpanded++;
       
        double gNew, hNew, fNew;
        
        if (isValid(i - 1, j) == true) {
       
            if (isDestination(i - 1, j, dest) == true) {
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
           
            else if (closedList[i - 1][j] == false
                     && isUnBlocked(grid, i - 1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest, hvalue);
                fNew = gNew + hNew;

                
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        if (isValid(i + 1, j) == true) {
            
            if (isDestination(i + 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            
            else if (closedList[i + 1][j] == false
                     && isUnBlocked(grid, i + 1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest, hvalue);
                fNew = gNew + hNew;

               
                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));
                    
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        if (isValid(i, j + 1) == true) {
           
            if (isDestination(i, j + 1, dest) == true) {
            
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i][j + 1] == false
                     && isUnBlocked(grid, i, j + 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest, hvalue);
                fNew = gNew + hNew;

                
                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));

                    
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

      
        if (isValid(i, j - 1) == true) {
            
            if (isDestination(i, j - 1, dest) == true) {
                
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

           
            else if (closedList[i][j - 1] == false
                     && isUnBlocked(grid, i, j - 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest, hvalue);
                fNew = gNew + hNew;

                
                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));

                    
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

       
    if(hvalue != 1) {
        
        if (isValid(i - 1, j + 1) == true) {
            
            if (isDestination(i - 1, j + 1, dest) == true) {
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            
            else if (closedList[i - 1][j + 1] == false
                     && isUnBlocked(grid, i - 1, j + 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j + 1, dest, hvalue);
                fNew = gNew + hNew;

                
                if (cellDetails[i - 1][j + 1].f == FLT_MAX
                    || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j + 1)));

                    
                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

       
        if (isValid(i - 1, j - 1) == true) {
          
            if (isDestination(i - 1, j - 1, dest) == true) {
                
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i - 1][j - 1] == false
                     && isUnBlocked(grid, i - 1, j - 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j - 1, dest, hvalue);
                fNew = gNew + hNew;

                
                if (cellDetails[i - 1][j - 1].f == FLT_MAX
                    || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j - 1)));
                    
                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        if (isValid(i + 1, j + 1) == true) {
            
            if (isDestination(i + 1, j + 1, dest) == true) {
                
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i + 1][j + 1] == false
                     && isUnBlocked(grid, i + 1, j + 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j + 1, dest, hvalue);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j + 1].f == FLT_MAX
                    || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j + 1)));

                   
                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        if (isValid(i + 1, j - 1) == true) {
          
            if (isDestination(i + 1, j - 1, dest) == true) {
          
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                //printf("The destination cell is found\n");
                //tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            
            else if (closedList[i + 1][j - 1] == false
                     && isUnBlocked(grid, i + 1, j - 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j - 1, dest, hvalue);
                fNew = gNew + hNew;

        
                if (cellDetails[i + 1][j - 1].f == FLT_MAX
                    || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j - 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
            }
           }
         }
        }
    }

    if (foundDest == false) {
        printf("Failed to find the Destination Cell\n");
    }
    printf("Nodes expanded: %d ", nodesExpanded);
        

    return;
}

void displayGrid(int grid[][COL]) {
    cout << "Grid:\n";
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
}

int main() {
    Pair src = make_pair(8, 0);
    Pair dest = make_pair(0, 9);

    int grid1[ROW][COL] = { //no diagonal paths, Good test
        {1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    int grid2[ROW][COL] = { //diagonal paths, Show diagonal 

        {1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 1, 0, 1, 1},
        {1, 1, 0, 1, 0, 1, 0, 0, 1, 1},
        {1, 0, 0, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    int (*grids[2])[COL] = {grid1, grid2};
    const char* heuristics[] = {"Euclidean", "Manhattan", "Diagonal"};

    long int avgE_Graph_One = 0;
    long int avgM_Graph_One = 0;
    long int avgD_Graph_One = 0;
    long int avgE_Graph_Two = 0;
    long int avgM_Graph_Two = 0;
    long int avgD_Graph_Two = 0;



    for(int i = 0; i < 1000; i++) {

        for (int g = 0; g < 2; g++) {

            if(i == 1) {
                cout << "\n=====================================\n";
                cout << "Testing on Grid " << g + 1 << ":\n";
                displayGrid(grids[g]);
            }
           

                for (int h = 0; h < 3; h++) {

                    
                    if (i == 1) {
                        cout << "\n-------------------------------------\n";
                        cout << "Using " << heuristics[h] << " Heuristic:\n";
                    }
                    
                    auto start = chrono::high_resolution_clock::now();
                    aStarSearch(grids[g], src, dest, h);
                    auto end = chrono::high_resolution_clock::now();
                    auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    if (i == 1) {
                        cout << "\nRunning Time: " << elapsed.count() << " ns\n";
                    }
                    
                    
                    if(g == 0) {
                        if(heuristics[h] == "Euclidean") {
                            avgE_Graph_One += elapsed.count();

                        }
                        else if(heuristics[h] == "Manhattan") {
                            avgM_Graph_One += elapsed.count();

                        }
                        else {
                            avgD_Graph_One += elapsed.count();

                        }
                    }
                    else if(g == 1) {
                        if(heuristics[h] == "Euclidean") {
                            avgE_Graph_Two += elapsed.count();

                        }
                        else if(heuristics[h] == "Manhattan") {
                            avgM_Graph_Two += elapsed.count();

                        }
                        else {
                            avgD_Graph_Two += elapsed.count();
                        }
                    }
            }
        }
    }

    cout << "\nGrid One Average Running Times in ns for 1000 runs: \n";
    cout << "Euclidean: " << avgE_Graph_One / 1000.0 << " ns \n";
    cout << "Manhattan: " << avgM_Graph_One / 1000.0 << " ns \n";
    cout << "Diagonal: " << avgD_Graph_One / 1000.0 << " ns \n";
    cout << "--------------------------------\n";
     cout << "Grid Two Average Running Times in ns for 1000 runs: \n";
    cout << "Euclidean: " << avgE_Graph_Two / 1000.0 << " ns \n";
    cout << "Manhattan: " << avgM_Graph_Two / 1000.0 << " ns \n";
    cout << "Diagonal: " << avgD_Graph_Two / 1000.0 << " ns \n";
    cout << "--------------------------------\n";


    return 0;
}

