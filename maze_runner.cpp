#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <iostream> 
#include <thread>
#include <vector>
#include <fstream>
#include <mutex>
#include <atomic>

using namespace std;

class pos_t {
private:
	int i;
	int j;

public:
	pos_t(int i, int j) : i(i), j(j) {};
	pos_t() {};

	int get_i() {return i;}
	int get_j() {return j;}

	void set_i(int i) {this->i = i;}
	void set_j(int j) {this->j = j;}
};

int num_rows;
int num_cols;

atomic<bool> resolvido;
atomic<int> thread_counter;

vector<vector<char>> maze;

pos_t load_maze(const char* file_name) {
	pos_t initial_pos;

	ifstream file(file_name);
	if(!file.is_open()) {
		cout << "arquivo nao encontrado" << endl;
		exit(1);
	}

	file >> num_rows >> num_cols;
	maze.resize(num_rows, std::vector<char>(num_cols));
	
	for(int i = 0; i < num_rows; ++i) {
		for(int j = 0; j < num_cols; ++j) {


			file >> maze[i][j];
			if(maze[i][j] == 'e') {
				initial_pos.set_i(i);
				initial_pos.set_j(j);
			}
		}
	}

	file.close();

	return initial_pos;
}

void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			cout << maze[i][j];
		}
		cout << endl;
	}
}

void walk(pos_t pos) {
	stack<pos_t> my_valid_pos;

	thread_counter ++;

	while(!resolvido) {
		int i = pos.get_i(), j = pos.get_j();

		maze[i][j] = 'o';

		pos_t pos_to_verify[] = {pos_t(i, j+1), pos_t(i,j-1), pos_t(i+1,j), pos_t(i-1,j)};
		int path_ctr = 0;
		for (int index = 0; index < sizeof(pos_to_verify)/sizeof(*pos_to_verify); index ++) {
			if(!(pos_to_verify[index].get_i() >= 0 and pos_to_verify[index].get_i() < num_rows and
				pos_to_verify[index].get_j() >= 0 and pos_to_verify[index].get_j() < num_cols))
				continue;

			char c_to_verify = maze[pos_to_verify[index].get_i()][pos_to_verify[index].get_j()];

			if(c_to_verify == 's') {
				resolvido = true;
				thread_counter --;
				return;
			}
			else if(c_to_verify == 'x') {
				path_ctr ++;
				if(path_ctr == 1) my_valid_pos.push(pos_to_verify[index]);
				else {
					thread t(walk, pos_to_verify[index]);
					t.detach();
				}
			}
		}

		maze[i][j] = '.';

		do {

			if(my_valid_pos.empty()) {
				thread_counter --;
				return;
			}

			pos = my_valid_pos.top();
			my_valid_pos.pop();
			
		} while(maze[pos.get_i()][pos.get_j()] != 'x');
	}
	thread_counter --;
}

int main(int argc, char* argv[]) {

	resolvido = false;
	pos_t initial_pos;

	if(argc != 2)
		initial_pos = load_maze("../data/maze3.txt");
	else
		initial_pos = load_maze(argv[1]);

	thread t(walk, initial_pos);
	t.join();

	while(!resolvido and thread_counter) {
		system("clear");
		print_maze();
	}
	
	cout << "saida " << (resolvido ? "" : "nao ") << "encontrada" << endl;

	return 0;
}
