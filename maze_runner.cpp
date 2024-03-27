#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <iostream> 
#include <thread>
#include <vector>
#include <fstream>

using namespace std;

// Matriz de char representnado o labirinto
vector<vector<char>> maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
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

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//    valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	ifstream file(file_name);
	if(!file.is_open()) {
		cout << "arquivo nao encontrado" << endl;
		exit(1);
	}

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	file >> num_rows >> num_cols;
	maze.resize(num_rows, std::vector<char>(num_cols));
	
	for(int i = 0; i < num_rows; ++i) {
		for(int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
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

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			cout << maze[i][j];
		}
		cout << endl;
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	int i = pos.get_i(), j = pos.get_j();
	// Marcar a posição atual com o símbolo '.'
	maze[i][j] = 'o';
	this_thread::sleep_for(chrono::milliseconds(100));
	// Limpa a tela
	system("clear");

	// Imprime o labirinto
	print_maze();
	
	/* Dado a posição atual, verifica quais sao as próximas posições válidas
		Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		cada uma delas no vetor valid_positions
			- pos.i, pos.j+1
			- pos.i, pos.j-1
			- pos.i+1, pos.j
			- pos.i-1, pos.j
		Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	*/

	pos_t pos_to_verify[] = {pos_t(i, j+1), pos_t(i,j-1), pos_t(i+1,j), pos_t(i-1,j)};
	for (int index = 0; index < sizeof(pos_to_verify)/sizeof(*pos_to_verify); index ++) {

		if(!(pos_to_verify[index].get_i() >= 0 and pos_to_verify[index].get_i() < num_rows and
			pos_to_verify[index].get_j() >= 0 and pos_to_verify[index].get_j() < num_cols))
			continue;

		char c_to_verify = maze[pos_to_verify[index].get_i()][pos_to_verify[index].get_j()];

		if(c_to_verify == 's')
			return true;
		else if(c_to_verify == 'x')
			valid_positions.push(pos_to_verify[index]);
	}

	maze[i][j] = '.';

	if(valid_positions.empty())
		return false;
	
	do {
		pos = valid_positions.top();
		valid_positions.pop();
	} while(maze[pos.get_i()][pos.get_j()] != 'x');

	return walk(pos);
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos;
	if(argc != 2)
		initial_pos = load_maze("../data/maze3.txt");
	else
		initial_pos = load_maze(argv[1]);
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	cout << "saida " << (exit_found ? "" : "nao ") << "encontrada" << endl;
	
	return 0;
}
