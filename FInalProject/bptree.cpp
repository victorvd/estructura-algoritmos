#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
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
	
	node.dispRoutine(node.getRoot());
}
