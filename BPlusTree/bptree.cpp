#include <iostream>
#include <vector>
#include "BPlusTree.h"

int main(int argc, char* argv[]){

	if (argc != 2) {
		printf("Formato: ./bptree <Degree>\n");
		return -1;
	}

	MAX = stoi(argv[1]);
	
	vector<int> bpv{5, 15, 25, 35, 45, 55, 40, 30, 20};

	BPTree node;
	
	for (int i = 0; i < bpv.size(); i++){
	        node.insert(bpv[i]);
        }

	node.display(node.getRoot());

	node.search(15);
	
	node.remove(15);

	node.display(node.getRoot());
	
	node.search(15);
}
