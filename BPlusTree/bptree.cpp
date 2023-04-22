#include <iostream>
#include "BPlusTree.h"

int main(int argc, char* argv[]){

	if (argc != 2) {
		printf("Formato: ./bptree <Degree>\n");
		return -1;
	}

	MAX = stoi(argv[1]);

	BPTree node;
	node.insert(5);
	node.insert(15);
	node.insert(25);
	node.insert(35);
	node.insert(45);
	node.insert(55);
	node.insert(40);
	node.insert(30);
	node.insert(20);
	//https://cdn.programiz.com/sites/tutorial2program/files/search-tree.png
	node.display(node.getRoot());

	node.search(15);
	
	node.remove(15);

	node.display(node.getRoot());
	
	node.search(15);
}
