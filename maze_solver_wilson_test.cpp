#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

using namespace std;

const int row = 10;
const int col = 10;
const int dep = 6;

char W = '#';
char P = ' ';
char Mazer = '@';
char S = 'S';
char E = 'E';
char y = 'y';
char n = 'n';
char forks = '%';
char up = 'U';
char down = 'D';
char solve = '-';

int solveMaze(vector<vector<vector<char> > > *maze);
void printStep(int r, int c, int d, vector<vector<vector<char> > > *maze);
void directionManager(int r, int c, int d, vector<vector<vector<char> > > *maze);
int makeMove(int r, int c, int d, vector<vector<vector<char> > > *maze);
bool checkEnd(int r, int c, int d, vector<vector<vector<char> > > *maze);
void limitShow(int r, int c, int d, int lim, vector<vector<vector<char> > > *maze, bool solution);
int findDuder(int r, int c, int d, vector<vector<vector<char> > > *maze);
int findDudec(int r, int c, int d, vector<vector<vector<char> > > *maze);
int findDuded(int r, int c, int d, vector<vector<vector<char> > > *maze);
int findSpace(int r, int c, int d, vector<vector<vector<char> > > *maze, char rep);
void showMaze(int r, int c, int d, vector<vector<vector<char> > > *maze, bool solution);

int main(void){
    ifstream iFile ("maze.txt");
    string mazeLine = "";
    int x, y, z;
    iFile >> x;
    iFile >> y;
    iFile >> z;
    vector<vector<vector<char> > > maze;
    maze.resize(x);
    for (int a = 0; a < x; a++){
        maze[a].resize(y);
        for (int b = 0; b < y; b++){
            maze[a][b].resize(z);
        }
    }

    getline(iFile, mazeLine);
    for (int x = 0; x < dep; x ++){
        for (int y = 0; y < row; y ++){
            getline(iFile, mazeLine);
            cout << mazeLine << endl;
            for (int z = 0; z < col; z ++){
                maze[y][z][x] = mazeLine[z];
            }
        }
        getline(iFile, mazeLine);
    }
    findSpace(row, col, dep, &maze, Mazer);
    findSpace(row, col, dep, &maze, E);
    cout << "Maze to solve" << endl;
    showMaze(row, col, dep, &maze, false);
    solveMaze(&maze);
}

struct direction{
    bool North;
    bool East;
    bool South;
    bool West;
    bool Up;
    bool Down;
};

direction direction;

int solveMaze(vector<vector<vector<char> > > *maze){
    findSpace(row, col, dep, maze, Mazer);
    findSpace(row, col, dep, maze, E);
    int r = findDuder(row, col, dep, maze);
    int c = findDudec(row, col, dep, maze);
    int d = findDuded(row, col, dep, maze);
    directionManager(r, c, d, maze);
    if (checkEnd(r, c, d, maze)){
        showMaze(row, col, dep, maze, true);
        return 0;
    }
    else{
        makeMove(r, c, d, maze);
        sleep(1);
        printStep(row, col, d, maze);
    }
    solveMaze(maze);
}

void printStep(int r, int c, int d, vector<vector<vector<char> > > *maze){
    string line = "";
    for (int x = 0; x < r; x ++){
        for (int y = 0; y < c; y ++){
            cout << (*maze)[x][y][d];
        }
        cout << endl;
    }
}



void directionManager(int r, int c, int d, vector<vector<vector<char> > > *maze){

    direction.North = false;
    direction.East = false;
    direction.South = false;
    direction.West = false;
    direction.Up = false;
    direction.Down = false;

    if (r - 1 != -1){
        direction.North = true;
    }
    if (r + 1 != row){
        direction.South = true;
    }
    if (c - 1 != -1){
        direction.West = true;
    }
    if (c + 1 != col){
        direction.East = true;
    }
    if (d - 1 != -1){
        direction.Down = true;
    }
    if (d + 1 != dep){
        direction.Up = true;
    }

    if (direction.North){
        if ((*maze)[r - 1][c][d] == W || (*maze)[r - 1][c][d] == n){
            direction.North = false;
        }
    }
    if (direction.South){
        if ((*maze)[r + 1][c][d] == W || (*maze)[r + 1][c][d] == n){
            direction.South = false;
        }
    }
    if (direction.East){
        if ((*maze)[r][c + 1][d] == W || (*maze)[r][c + 1][d] == n){
            direction.East = false;
        }
    }
    if (direction.West){
        if ((*maze)[r][c - 1][d] == W || (*maze)[r][c - 1][d] == n){
            direction.West = false;
        }
    }
    if (direction.Up){
        if ((*maze)[r][c][d + 1] == W || (*maze)[r][c][d + 1] == n){
            direction.Up = false;
        }
    }
    if (direction.Down){
        if ((*maze)[r][c][d - 1] == W || (*maze)[r][c][d - 1] == n){
            direction.Down = false;
        }
    }

}

int makeMove(int r, int c, int d, vector<vector<vector<char> > > *maze){
    int count = 0;

    if (direction.North)
        count++;
    if (direction.East)
        count++;
    if (direction.South)
        count++;
    if (direction.West)
        count++;
    if (direction.Up)
        count++;
    if (direction.Down)
        count++;

    if (direction.North && (*maze)[r - 1][c][d] == P){
            if (count > 2)
                (*maze)[r][c][d] = forks;
            else
                (*maze)[r][c][d] = y;
            (*maze)[r - 1][c][d] = Mazer;
            return 0;
    }
    if (direction.East && (*maze)[r][c + 1][d] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = y;
        (*maze)[r][c + 1][d] = Mazer;
        return 0;
    }
    if (direction.South && (*maze)[r + 1][c][d] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = y;
        (*maze)[r + 1][c][d] = Mazer;
        return 0;
    }
    if (direction.West && (*maze)[r][c - 1][d] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = y;
        (*maze)[r][c - 1][d] = Mazer;
        return 0;
    }
    if (direction.Up && (*maze)[r][c][d + 1] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = y;
        (*maze)[r][c][d + 1] = Mazer;
        return 0;
    }
    if (direction.Down && (*maze)[r][c][d - 1] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = y;
        (*maze)[r][c][d - 1] = Mazer;
        return 0;
    }

    if (direction.North && ((*maze)[r - 1][c][d] == y || (*maze)[r - 1][c][d] == forks)){
        (*maze)[r][c][d] = n;
        (*maze)[r - 1][c][d] = Mazer;
        return 0;
    }
    if (direction.East && ((*maze)[r][c + 1][d] == y || (*maze)[r][c + 1][d] == forks)){
        (*maze)[r][c][d] = n;
        (*maze)[r][c + 1][d] = Mazer;
        return 0;
    }
    if (direction.South && ((*maze)[r + 1][c][d] == y || (*maze)[r + 1][c][d] == forks)){
        (*maze)[r][c][d] = n;
        (*maze)[r + 1][c][d] = Mazer;
        return 0;
    }
    if (direction.West && ((*maze)[r][c - 1][d] == y || (*maze)[r][c - 1][d] == forks)){
        (*maze)[r][c][d] = n;
        (*maze)[r][c - 1][d] = Mazer;
        return 0;
    }
    if (direction.Up && ((*maze)[r][c][d + 1] == y || (*maze)[r][c][d + 1] == forks)){
        (*maze)[r][c][d] = n;
        (*maze)[r][c][d + 1] = Mazer;
        return 0;
    }
    if (direction.Down && ((*maze)[r][c][d - 1] == y || (*maze)[r][c][d - 1] == forks)){
        (*maze)[r][c][d] = n;
        (*maze)[r][c][d - 1] = Mazer;
        return 0;
    }
}

bool checkEnd(int r, int c, int d, vector<vector<vector<char> > > *maze){
    if (direction.North){
        if ((*maze)[r - 1][c][d] == E){
            return true;
        }
    }
    if (direction.East){
        if ((*maze)[r][c + 1][d] == E){
            return true;
        }
    }
    if (direction.South){
        if ((*maze)[r + 1][c][d] == E){
            return true;
        }
    }
    if (direction.West){
        if ((*maze)[r][c - 1][d] == E){
            return true;
        }
    }
    if (direction.Up){
        if ((*maze)[r][c][d + 1] == E){
            return true;
        }
    }
    if (direction.Down){
        if ((*maze)[r-1][c][d - 1] == E){
            return true;
        }
    }
    return false;
}

int findDuder(int r, int c, int d, vector<vector<vector<char> > > *maze){
    for (int x = 0; x < d; x++){
        for (int y = 0; y < r; y++){
            for (int z = 0; z < c; z++){
                if ((*maze)[y][z][x] == Mazer){
                    return y;
                }
            }
        }
    }
}

int findDudec(int r, int c, int d, vector<vector<vector<char> > > *maze){
    for (int x = 0; x < d; x++){
        for (int y = 0; y < r; y++){
            for (int z = 0; z < c; z++){
                if ((*maze)[y][z][x] == Mazer){
                    return z;
                }
            }
        }
    }
}

int findDuded(int r, int c, int d, vector<vector<vector<char> > > *maze){
    for (int x = 0; x < d; x++){
        for (int y = 0; y < r; y++){
            for (int z = 0; z < c; z++){
                if ((*maze)[y][z][x] == Mazer){
                    return x;
                }
            }
        }
    }
}

int findSpace(int r, int c, int d, vector<vector<vector<char> > > *maze, char rep){
    for (int x = 0; x < d; x++){
        for (int y = 0; y < c; y++){
            for (int z = 0; z < r; z++){
                if ((*maze)[z][y][x] == P && (y - 1 == -1 || y + 1 == c || z - 1 == -1 || z + 1 == r)){
                    (*maze)[z][y][x] = rep;
                    return 0;
                }
            }
        }
    }
}

void showMaze(int r, int c, int d, vector<vector<vector<char> > > *maze, bool solution){
    if (c >= 10 && c <= 20){
        limitShow(r, c, d, 4, maze, solution);
    }
    if (c > 20 && c <= 33){
        limitShow(r, c, d, 3, maze, solution);
    }
    if (c > 33 && c <= 50){
        limitShow(r, d, d, 2, maze, solution);
    }
}

void limitShow(int r, int c, int d, int lim, vector<vector<vector<char> > > *maze, bool solution){
    int w = d;
    int layer = 0;
    int moveup = 0;
        while(w / lim != 0){
            for (int g = 0; g < lim; g++){
                cout << left  << "Layer: " << setw(c - 6) << layer;
                layer ++;
            }
            cout << endl;
            for (int x = 0; x < r; x ++){
                for (int y = 0; y < lim; y ++){
                    for (int z = 0; z < c; z ++){
                        if (solution){
                            if ((*maze)[x][z][y + moveup] == 'y' || (*maze)[x][z][y + moveup] == forks){
                                (*maze)[x][z][y + moveup] = solve;
                            }
                            else if ((*maze)[x][z][y + moveup] == n){
                                (*maze)[x][z][y + moveup] = P;
                            }
                            else{
                                (*maze)[x][z][y + moveup] = (*maze)[x][z][y + moveup];
                            }
                        }
                        cout << (*maze)[x][z][y + moveup];
                    }
                    cout << " ";
                }
                cout << endl;
            }
            w-=lim;
            moveup+=4;
        }

        for (int g = 0; g < d % lim; g++){
            cout << left <<  "Layer: " << setw(c - 6) << layer;
            layer ++;
        }
        cout << endl;
        for (int x = 0; x < r; x ++){
            for (int y = 0; y < d % lim; y ++){
                for (int z = 0; z < c; z ++){
                    if (solution){
                        if ((*maze)[x][z][y + moveup] == 'y' || (*maze)[x][z][y + moveup] == forks){
                            (*maze)[x][z][y + moveup] = solve;
                        }
                        else if ((*maze)[x][z][y + moveup] == n){
                            (*maze)[x][z][y + moveup] = P;
                        }
                        else{
                            (*maze)[x][z][y + moveup] = (*maze)[x][z][y + moveup];
                        }
                    }
                    cout << (*maze)[x][z][y + moveup];
                }
                cout << " ";
            }
            cout << endl;
        }
    }
