#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

int rand_number(int low, int high){
	return low + rand() % (high - low);
}

struct p3 {
int x, y, z;
p3(int x, int y, int z) : x(x), y(y), z(z) {}
} typedef point3;

void generate_maze(int x_dim, int y_dim, int z_dim){
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

	

	for(int z = 0; z < z_dim; z++){
		cout << z << endl;
		for(int y = 0; y < y_dim; y++){
			for(int x = 0; x < x_dim; x++){
				cout << (data[x][y][z] ? "#" : " ");
			}
			cout << endl;
		}
	}

}

int main()
{
	generate_maze(20,10,10);
	return 0;
}
