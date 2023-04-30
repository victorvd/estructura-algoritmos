#include <iostream>
#include <vector>
#include "BStarTree.h"

int main(int argc, char* argv[]){

	if (argc != 2) {
		printf("Formato: ./bptree <Degree>\n");
		return -1;
	}

	//MAX = stoi(argv[1]);
	
	vector<int> bsv{5, 15, 25, 35, 45, 55, 40, 30, 20};

	BSTree<int> *node;
	
	for (int i = 0; i < bsv.size(); i++){
	        node->Insertar(bsv[i]);
        }
	
	node->Imprimir();

	node->Buscar(15);
	
	node->Eliminar(15);

	node->Imprimir();
	
	node->Buscar(15);
}
