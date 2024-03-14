#include <stdio.h>
#include <stack>

#include <iostream> 
#include <stdlib.h>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;

	pos_t(int i, int j) : i(i), j(j) {};
	pos_t() {};
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
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
	FILE* file = fopen(file_name, "r");
	if(!file) {
		printf("arquivo nao encontrado\n");
		exit(1);
	}

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(file, "%d %d", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
	maze = new char*[num_rows];
	for (int i = 0; i < num_rows; ++i)
		// Aloca cada linha da matriz
		maze[i] = new char[num_cols];

	
	for (int i = 0; i < num_rows; ++i) {
		fscanf(file, "%s", maze[i]);
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			if(maze[i][j] == 'e') {
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	int i = pos.i, j = pos.j;
	// Marcar a posição atual com o símbolo '.'
	maze[i][j] = 'o';
	
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

		if(!(pos_to_verify[index].i >= 0 and pos_to_verify[index].i < num_rows and
			pos_to_verify[index].j >= 0 and pos_to_verify[index].j < num_cols))
			continue;

		char c_to_verify = maze[pos_to_verify[index].i][pos_to_verify[index].j];

		if(c_to_verify == 's')
			return true;
		else if(c_to_verify == 'x')
			valid_positions.push(pos_to_verify[index]);
	}

	maze[i][j] = '.';

	if(valid_positions.empty())
		return false;
	
	pos = valid_positions.top();
	valid_positions.pop();
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
	printf("saida %sencontrada\n", exit_found ? "" : "nao ");
	
	return 0;
}
