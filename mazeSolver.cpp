#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>
#ifdef __unix__
#include <sys/time.h>
#endif

using namespace std;

int x, y, z;

char W = '#';
char P = ' ';
char Mazer = '@';
char S = 'S';
char E = 'E';
char g = 'y';
char n = 'n';
char forks = '%';
char up = 'U';
char down = 'D';
char solve = '-';

int solveMaze(vector<vector<vector<char> > > *maze, bool showStep);
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
void generate_maze(int x_dim, int y_dim, int z_dim, const char *filename);

int main( int argc, char *argv[] ){
#ifdef __unix__
timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);
srand(ts.tv_nsec);
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST
#endif
    string generate = "";
    bool gen = true;
    if(argc < 2)
    while(true){
    cout << "Generate New Map?[y/N] ";
    getline(cin, generate);
    if (generate == "y" || generate == "Y"){
        gen = true;
	break;
    }else
    if (generate == "N" || generate == "n"){
        gen = false;
	break;
    }
    cout << "could not parse neither 'y' nor 'n'" << endl;
    }
    if(gen) generate_maze(rand() % 5 + 10, rand() % 5 + 10, rand() % 10 + 5, "maze.txt");
    ifstream iFile ("maze.txt");
    string mazeLine = "";
    string input = "";
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
    for (int a = 0; a < z; a ++){
        for (int b = 0; b < x; b ++){
            getline(iFile, mazeLine);
            cout << mazeLine << endl;
            for (int c = 0; c < y; c ++){
                maze[b][c][a] = mazeLine[c];
            }
        }
        getline(iFile, mazeLine);
    }
    findSpace(x, y, z, &maze, Mazer);
    findSpace(x, y, z, &maze, E);
    cout << "Maze to solve" << endl;
    showMaze(x, y, z, &maze, false);
    if(argc < 3){
    cout << "Show steps?[y/n] ";
    getline(cin, input);
    } else input = argv[2];
    bool input2 = true;
    if (input == "y" || input == "Y"){
        input2 = true;
    }
    if (input == "N" || input == "n"){
        input2 = false;
    }
    solveMaze(&maze, input2);
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

void draw(vector<vector<vector<char> > > *maze, int x, int z, int y, int moveup){
#ifdef __unix__
			if((*maze)[x][z][y + moveup] == solve || (*maze)[x][z][y + moveup] == Mazer) 
				cout << BOLD(FBLU("#"));
			else
			if((*maze)[x][z][y + moveup] == up)
				cout << BOLD(FGRN("#"));
			else
			if((*maze)[x][z][y + moveup] == down)
				cout << BOLD(FRED("#"));
			else
			if((*maze)[x][z][y + moveup] == E)
				cout << BOLD(FYEL("#"));
			else  cout <<(*maze)[x][z][y + moveup];
			#else
			cout << (*maze)[x][z][y + moveup];
			#endif
}

int solveMaze(vector<vector<vector<char> > > *maze, bool showStep){
    findSpace(x, y, z, maze, Mazer);
    findSpace(x, y, z, maze, E);
    int r = findDuder(x, y, z, maze);
    int c = findDudec(x, y, z, maze);
    int d = findDuded(x, y, z, maze);
    directionManager(r, c, d, maze);
    if (checkEnd(r, c, d, maze)){
        showMaze(x, y, z, maze, true);
        return 0;
    }
    else{
        makeMove(r, c, d, maze);
        if (showStep) {
            usleep(1000);
            printStep(x, y, d, maze);
        }
    }
    solveMaze(maze, showStep);
}

int step = 0;

void printStep(int r, int c, int d, vector<vector<vector<char> > > *maze){
    string line = "";
    cout << " On Layer: " << d << ", Step: " << (++step) << endl;
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
    if (r + 1 != x){
        direction.South = true;
    }
    if (c - 1 != -1){
        direction.West = true;
    }
    if (c + 1 != y){
        direction.East = true;
    }
    if (d - 1 != -1){
        direction.Down = true;
    }
    if (d + 1 != z){
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
            (*maze)[r][c][d] = g;
        (*maze)[r - 1][c][d] = Mazer;
        return 0;
    }
    if (direction.East && (*maze)[r][c + 1][d] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = g;
        (*maze)[r][c + 1][d] = Mazer;
        return 0;
    }
    if (direction.South && (*maze)[r + 1][c][d] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = g;
        (*maze)[r + 1][c][d] = Mazer;
        return 0;
    }
    if (direction.West && (*maze)[r][c - 1][d] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = g;
        (*maze)[r][c - 1][d] = Mazer;
        return 0;
    }
    if (direction.Up && (*maze)[r][c][d + 1] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = up;
        (*maze)[r][c][d + 1] = Mazer;
        return 0;
    }
    if (direction.Down && (*maze)[r][c][d - 1] == P){
        if (count > 2)
            (*maze)[r][c][d] = forks;
        else
            (*maze)[r][c][d] = down;
        (*maze)[r][c][d - 1] = Mazer;
        return 0;
    }

    if (direction.North && ((*maze)[r - 1][c][d] == g || (*maze)[r - 1][c][d] == forks)
        || (*maze)[r - 1][c][d] == up || (*maze)[r - 1][c][d] == down){
        (*maze)[r][c][d] = n;
        (*maze)[r - 1][c][d] = Mazer;
        return 0;
    }
    if (direction.East && ((*maze)[r][c + 1][d] == g || (*maze)[r][c + 1][d] == forks)
            || (*maze)[r][c + 1][d] == up || (*maze)[r][c + 1][d] == down){
        (*maze)[r][c][d] = n;
        (*maze)[r][c + 1][d] = Mazer;
        return 0;
    }
    if (direction.South && ((*maze)[r + 1][c][d] == g || (*maze)[r + 1][c][d] == forks)
            || (*maze)[r + 1][c][d] == up || (*maze)[r + 1][c][d] == down){
        (*maze)[r][c][d] = n;
        (*maze)[r + 1][c][d] = Mazer;
        return 0;
    }
    if (direction.West && ((*maze)[r][c - 1][d] == g || (*maze)[r][c - 1][d] == forks)
            || (*maze)[r][c - 1][d] == up || (*maze)[r][c - 1][d] == down){
        (*maze)[r][c][d] = n;
        (*maze)[r][c - 1][d] = Mazer;
        return 0;
    }
    if (direction.Up && ((*maze)[r][c][d + 1] == g || (*maze)[r][c][d + 1] == forks)
            || (*maze)[r][c][d + 1] == up || (*maze)[r][c][d + 1] == down){
        (*maze)[r][c][d] = n;
        (*maze)[r][c][d + 1] = Mazer;
        return 0;
    }
    if (direction.Down && ((*maze)[r][c][d - 1] == g || (*maze)[r][c][d - 1] == forks)
            || (*maze)[r][c][d - 1] == up || (*maze)[r][c][d - 1] == down){
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
                        if ((*maze)[x][z][y + moveup] == g || (*maze)[x][z][y + moveup] == forks){
                            (*maze)[x][z][y + moveup] = solve;
                        }
                        else if ((*maze)[x][z][y + moveup] == n){
                            (*maze)[x][z][y + moveup] = P;
                        }
                        else{
                            (*maze)[x][z][y + moveup] = (*maze)[x][z][y + moveup];
                        }
                    }
			
                draw(maze,x,z,y,moveup);
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
                    if ((*maze)[x][z][y + moveup] == g || (*maze)[x][z][y + moveup] == forks){
                        (*maze)[x][z][y + moveup] = solve;
                    }
                    else if ((*maze)[x][z][y + moveup] == n){
                        (*maze)[x][z][y + moveup] = P;
                    }
                    else{
                        (*maze)[x][z][y + moveup] = (*maze)[x][z][y + moveup];
                    }
                }
                draw(maze,x,z,y,moveup);
            }
            cout << " ";
        }
        cout << endl;
    }
}

int rand_number(int low, int high){
	return low + rand() % (high - low);
}

struct p3 {
int x, y, z;
p3(int x, int y, int z) : x(x), y(y), z(z) {}
} typedef point3;

void generate_maze(int x_dim, int y_dim, int z_dim, const char *filename){
	vector<vector<vector<bool> > > data;
	data.resize(x_dim);
	for(int x = 0; x < x_dim; x++){
		data[x].resize(y_dim);
		for(int y = 0; y < y_dim; y++){
			data[x][y].resize(z_dim);
			for(int z = 0; z < z_dim; z++){
				data[x][y][z] = true;
			}
		}
	}

	int randx = rand_number(1, x_dim - 1);
	int randy = rand_number(1, y_dim - 1);
	int randz = rand_number(0, z_dim);

	vector<point3> edges;
	cout << randx << "," << randy << "," << randz << endl;
	data[randx][randy][randz] = false;
	if(randx != 1) edges.push_back(point3(randx - 1, randy, randz));
	if(randx != x_dim - 2) edges.push_back(point3(randx + 1, randy, randz));
	if(randy != 1) edges.push_back(point3(randx, randy - 1, randz));
	if(randy != y_dim - 2) edges.push_back(point3(randx, randy + 1, randz));
	if(randz != 0) edges.push_back(point3(randx, randy, randz - 1));
	if(randz != z_dim - 1) edges.push_back(point3(randx, randy, randz + 1));

	/* Create Inner Maze */

	while(!edges.empty()){
		point3 curr = edges.back();
		edges.pop_back();
		int count = 0;
		if(!data[curr.x + 1][curr.y][curr.z]) count++;
		if(!data[curr.x - 1][curr.y][curr.z]) count++;
		if(!data[curr.x][curr.y + 1][curr.z]) count++;
		if(!data[curr.x][curr.y - 1][curr.z]) count++;
		if(curr.z != z_dim - 1 && !data[curr.x][curr.y][curr.z + 1]) count++;
		if(curr.z != 0 && !data[curr.x][curr.y][curr.z - 1]) count++;
		if(count < 2){
			data[curr.x][curr.y][curr.z] = false;
			if(curr.x != 1 && data[curr.x - 1][curr.y][curr.z]) edges.push_back(point3(curr.x - 1, curr.y, curr.z));
			if(curr.x != x_dim - 2 && data[curr.x + 1][curr.y][curr.z]) edges.push_back(point3(curr.x + 1, curr.y, curr.z));
			if(curr.y != 1 && data[curr.x][curr.y - 1][curr.z]) edges.push_back(point3(curr.x, curr.y - 1, curr.z));
			if(curr.y != y_dim - 2 && data[curr.x][curr.y + 1][curr.z]) edges.push_back(point3(curr.x, curr.y + 1, curr.z));
			if(curr.z != 0 && data[curr.x][curr.y][curr.z - 1]) edges.push_back(point3(curr.x, curr.y, curr.z - 1));
			if(curr.z != z_dim - 1 && data[curr.x][curr.y][curr.z + 1]) edges.push_back(point3(curr.x, curr.y, curr.z + 1));
		}
		random_shuffle(edges.begin(), edges.end());
	}

	/* Create Beginning and End */

	bool beginning = false, end = false;
	for(int i = 0; i < 2; i++)
	while(true){
		int rand_val = rand_number(0, 2*(x_dim + y_dim));
		bool opposite = (rand_val - (x_dim+y_dim) >= 0);
		bool x = (rand_val % (x_dim+y_dim))-x_dim < 0;
		int val = (rand_val % (x_dim+y_dim)) - (x ? 0 : x_dim);
		int layer = rand_number(0, z_dim);
		int dx, dy;
		if(x){
			dx = val;
			dy = (opposite ? y_dim - 1 : 0);
			if(!data[dx][dy][layer]) continue;
			if(opposite && !data[dx][dy - 1][layer]){
				data[dx][dy][layer] = false;
				break;
			} else if(!opposite && !data[dx][dy + 1][layer]){
				data[dx][dy][layer] = false;
				break;
			}
		} else {
			dx = (opposite ? x_dim - 1 : 0);
			dy = val;
			if(!data[dx][dy][layer]) continue;
			if(opposite && !data[dx - 1][dy][layer]){
				data[dx][dy][layer] = false;
				break;
			} else if(!opposite && !data[dx + 1][dy][layer]){
				data[dx][dy][layer] = false;
			}
		}
		
	}


	ofstream file;
	file.open(filename);
	file << y_dim << " " << x_dim << " " << z_dim << endl;
	for(int z = 0; z < z_dim; z++){
		for(int y = 0; y < y_dim; y++){
			for(int x = 0; x < x_dim; x++){
				file << (data[x][y][z] ? "#" : " ");
			}
			file << endl;
		}
		file << endl;
	}
	file.close();
}
