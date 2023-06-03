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
	//https://cdn.programiz.com/sites/tutorial2program/files/search-tree.png
	
	node.dispRoutine(node.getRoot());
	
	/*Mat image(500, 500, CV_8UC3, Scalar(200, 235, 225));
	
	int font_size = 1;
	Scalar font_Color(22, 22, 165);
	int font_weight = 2;
	
	int txtPosX = 80;
	int txtPosY = 80;
	int txtEsp = 60;
	
	node.display(node.getRoot(), image, font_size, font_Color, font_weight, txtPosX, txtPosY, txtEsp);
	
	imshow("Output", image);
	waitKey(0);

	node.search(15);
	
	node.remove(15);

	node.display(node.getRoot());
	
	node.search(15);*/
}
