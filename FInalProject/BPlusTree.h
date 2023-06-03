#ifndef BPlusTree_H
#define BPlusTree_H

using namespace cv;
using namespace std;

int MAX;

class Nodo {
	bool IS_LEAF;
	int *key, size;
	Nodo **ptr;
	friend class BPTree;

	public:
	Nodo();
};
class BPTree {
	Nodo *root;
	void insertInternal(int, Nodo *, Nodo *);
	void removeInternal(int, Nodo *, Nodo *);
	Nodo *findParent(Nodo *, Nodo *);

	public:
	BPTree();
	void search(int);
	void insert(int);
	void remove(int);
	void display(Nodo *cursor, Mat &image, Scalar frame_Color, int font_size, Scalar font_Color,
			int font_weight, int &txtPosX, int &txtPosY, int txtEsp);
	void dispRoutine(Nodo *cursor);
	Nodo *getRoot();
};
Nodo::Nodo() {
	key = new int[MAX];
	ptr = new Nodo *[MAX + 1];
}
BPTree::BPTree() {
	root = NULL;
}

// Search operation
void BPTree::search(int x) {
	if (root == NULL) {
		cout << "Tree is empty\n";
	} else {
		Nodo *cursor = root;
		while (cursor->IS_LEAF == false) {
			for (int i = 0; i < cursor->size; ++i) {
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					break;
				}
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}
		for (int i = 0; i < cursor->size; ++i) {
			if (cursor->key[i] == x) {
				cout << "Found\n";
				return;
			}
		}
		cout << "Not found\n";
	}
}

// Insert Operation
void BPTree::insert(int x) {
	if (root == NULL) {
		root = new Nodo;
		root->key[0] = x;
		root->IS_LEAF = true;
		root->size = 1;
	} else {
		Nodo *cursor = root;
		Nodo *parent;
		while (cursor->IS_LEAF == false) {
			parent = cursor;
			for (int i = 0; i < cursor->size; ++i) {
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					break;
				}
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}
		if (cursor->size < MAX) {
			int i = 0;
			while (x > cursor->key[i] && i < cursor->size)
				++i;
			for (int j = cursor->size; j > i; j--) {
				cursor->key[j] = cursor->key[j - 1];
			}
			cursor->key[i] = x;
			cursor->size++;
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
			cursor->ptr[cursor->size - 1] = NULL;
		} else {
			Nodo *newLeaf = new Nodo;
			int virtualNodo[MAX + 1];
			for (int i = 0; i < MAX; ++i) {
				virtualNodo[i] = cursor->key[i];
			}
			int i = 0, j;
			while (x > virtualNodo[i] && i < MAX)
				++i;
			for (int j = MAX + 1; j > i; j--) {
				virtualNodo[j] = virtualNodo[j - 1];
			}
			virtualNodo[i] = x;
			newLeaf->IS_LEAF = true;
			cursor->size = (MAX + 1) / 2;
			newLeaf->size = MAX + 1 - (MAX + 1) / 2;
			cursor->ptr[cursor->size] = newLeaf;
			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
			cursor->ptr[MAX] = NULL;
			for (i = 0; i < cursor->size; ++i) {
				cursor->key[i] = virtualNodo[i];
			}
			for (i = 0, j = cursor->size; i < newLeaf->size; ++i, ++j) {
				newLeaf->key[i] = virtualNodo[j];
			}
			if (cursor == root) {
				Nodo *newRoot = new Nodo;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->IS_LEAF = false;
				newRoot->size = 1;
				root = newRoot;
			} else {
				insertInternal(newLeaf->key[0], parent, newLeaf);
			}
		}
	}
}
void BPTree::insertInternal(int x, Nodo *cursor, Nodo *child) {
	if (cursor->size < MAX) {
		int i = 0;
		while (x > cursor->key[i] && i < cursor->size)
			++i;
		for (int j = cursor->size; j > i; j--) {
			cursor->key[j] = cursor->key[j - 1];
		}
		for (int j = cursor->size + 1; j > i + 1; j--) {
			cursor->ptr[j] = cursor->ptr[j - 1];
		}
		cursor->key[i] = x;
		cursor->size++;
		cursor->ptr[i + 1] = child;
	} else {
		Nodo *newInternal = new Nodo;
		int virtualKey[MAX + 1];
		Nodo *virtualPtr[MAX + 2];
		for (int i = 0; i < MAX; ++i) {
			virtualKey[i] = cursor->key[i];
		}
		for (int i = 0; i < MAX + 1; ++i) {
			virtualPtr[i] = cursor->ptr[i];
		}
		int i = 0, j;
		while (x > virtualKey[i] && i < MAX)
			++i;
		for (int j = MAX + 1; j > i; j--) {
			virtualKey[j] = virtualKey[j - 1];
		}
		virtualKey[i] = x;
		for (int j = MAX + 2; j > i + 1; j--) {
			virtualPtr[j] = virtualPtr[j - 1];
		}
		virtualPtr[i + 1] = child;
		newInternal->IS_LEAF = false;
		cursor->size = (MAX + 1) / 2;
		newInternal->size = MAX - (MAX + 1) / 2;
		for (i = 0, j = cursor->size + 1; i < newInternal->size; ++i, ++j) {
			newInternal->key[i] = virtualKey[j];
		}
		for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; ++i, ++j) {
			newInternal->ptr[i] = virtualPtr[j];
		}
		if (cursor == root) {
			Nodo *newRoot = new Nodo;
			newRoot->key[0] = cursor->key[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->IS_LEAF = false;
			newRoot->size = 1;
			root = newRoot;
		} else {
			insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
		}
	}
}
Nodo *BPTree::findParent(Nodo *cursor, Nodo *child) {
	Nodo *parent;
	if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
		return NULL;
	}
	for (int i = 0; i < cursor->size + 1; ++i) {
		if (cursor->ptr[i] == child) {
			parent = cursor;
			return parent;
		} else {
			parent = findParent(cursor->ptr[i], child);
			if (parent != NULL)
				return parent;
		}
	}
	return parent;
}

// Delete Operation
void BPTree::remove(int x) {
	if (root == NULL) {
		cout << "Tree empty\n";
	} else {
		Nodo *cursor = root;
		Nodo *parent;
		int leftSibling, rightSibling;
		while (cursor->IS_LEAF == false) {
			for (int i = 0; i < cursor->size; ++i) {
				parent = cursor;
				leftSibling = i - 1;
				rightSibling = i + 1;
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					break;
				}
				if (i == cursor->size - 1) {
					leftSibling = i;
					rightSibling = i + 2;
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}
		bool found = false;
		int pos;
		for (pos = 0; pos < cursor->size; pos++) {
			if (cursor->key[pos] == x) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Not found\n";
			return;
		}
		for (int i = pos; i < cursor->size; ++i) {
			cursor->key[i] = cursor->key[i + 1];
		}
		cursor->size--;
		if (cursor == root) {
			for (int i = 0; i < MAX + 1; ++i) {
				cursor->ptr[i] = NULL;
			}
			if (cursor->size == 0) {
				cout << "Tree died\n";
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				root = NULL;
			}
			return;
		}
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
		cursor->ptr[cursor->size + 1] = NULL;
		if (cursor->size >= (MAX + 1) / 2) {
			return;
		}
		if (leftSibling >= 0) {
			Nodo *leftNodo = parent->ptr[leftSibling];
			if (leftNodo->size >= (MAX + 1) / 2 + 1) {
				for (int i = cursor->size; i > 0; i--) {
					cursor->key[i] = cursor->key[i - 1];
				}
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
				cursor->ptr[cursor->size - 1] = NULL;
				cursor->key[0] = leftNodo->key[leftNodo->size - 1];
				leftNodo->size--;
				leftNodo->ptr[leftNodo->size] = cursor;
				leftNodo->ptr[leftNodo->size + 1] = NULL;
				parent->key[leftSibling] = cursor->key[0];
				return;
			}
		}
		if (rightSibling <= parent->size) {
			Nodo *rightNodo = parent->ptr[rightSibling];
			if (rightNodo->size >= (MAX + 1) / 2 + 1) {
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
				cursor->ptr[cursor->size - 1] = NULL;
				cursor->key[cursor->size - 1] = rightNodo->key[0];
				rightNodo->size--;
				rightNodo->ptr[rightNodo->size] = rightNodo->ptr[rightNodo->size + 1];
				rightNodo->ptr[rightNodo->size + 1] = NULL;
				for (int i = 0; i < rightNodo->size; ++i) {
					rightNodo->key[i] = rightNodo->key[i + 1];
				}
				parent->key[rightSibling - 1] = rightNodo->key[0];
				return;
			}
		}
		if (leftSibling >= 0) {
			Nodo *leftNodo = parent->ptr[leftSibling];
			for (int i = leftNodo->size, j = 0; j < cursor->size; ++i, ++j) {
				leftNodo->key[i] = cursor->key[j];
			}
			leftNodo->ptr[leftNodo->size] = NULL;
			leftNodo->size += cursor->size;
			leftNodo->ptr[leftNodo->size] = cursor->ptr[cursor->size];
			removeInternal(parent->key[leftSibling], parent, cursor);
			delete[] cursor->key;
			delete[] cursor->ptr;
			delete cursor;
		} else if (rightSibling <= parent->size) {
			Nodo *rightNodo = parent->ptr[rightSibling];
			for (int i = cursor->size, j = 0; j < rightNodo->size; ++i, ++j) {
				cursor->key[i] = rightNodo->key[j];
			}
			cursor->ptr[cursor->size] = NULL;
			cursor->size += rightNodo->size;
			cursor->ptr[cursor->size] = rightNodo->ptr[rightNodo->size];
			cout << "Merging two leaf Nodos\n";
			removeInternal(parent->key[rightSibling - 1], parent, rightNodo);
			delete[] rightNodo->key;
			delete[] rightNodo->ptr;
			delete rightNodo;
		}
	}
}
void BPTree::removeInternal(int x, Nodo *cursor, Nodo *child) {
	if (cursor == root) {
		if (cursor->size == 1) {
			if (cursor->ptr[1] == child) {
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				root = cursor->ptr[0];
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				cout << "Changed root Nodo\n";
				return;
			} else if (cursor->ptr[0] == child) {
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				root = cursor->ptr[1];
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				cout << "Changed root Nodo\n";
				return;
			}
		}
	}
	int pos;
	for (pos = 0; pos < cursor->size; pos++) {
		if (cursor->key[pos] == x) {
			break;
		}
	}
	for (int i = pos; i < cursor->size; ++i) {
		cursor->key[i] = cursor->key[i + 1];
	}
	for (pos = 0; pos < cursor->size + 1; pos++) {
		if (cursor->ptr[pos] == child) {
			break;
		}
	}
	for (int i = pos; i < cursor->size + 1; ++i) {
		cursor->ptr[i] = cursor->ptr[i + 1];
	}
	cursor->size--;
	if (cursor->size >= (MAX + 1) / 2 - 1) {
		return;
	}
	if (cursor == root)
		return;
	Nodo *parent = findParent(root, cursor);
	int leftSibling, rightSibling;
	for (pos = 0; pos < parent->size + 1; pos++) {
		if (parent->ptr[pos] == cursor) {
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}
	if (leftSibling >= 0) {
		Nodo *leftNodo = parent->ptr[leftSibling];
		if (leftNodo->size >= (MAX + 1) / 2) {
			for (int i = cursor->size; i > 0; i--) {
				cursor->key[i] = cursor->key[i - 1];
			}
			cursor->key[0] = parent->key[leftSibling];
			parent->key[leftSibling] = leftNodo->key[leftNodo->size - 1];
			for (int i = cursor->size + 1; i > 0; i--) {
				cursor->ptr[i] = cursor->ptr[i - 1];
			}
			cursor->ptr[0] = leftNodo->ptr[leftNodo->size];
			cursor->size++;
			leftNodo->size--;
			return;
		}
	}
	if (rightSibling <= parent->size) {
		Nodo *rightNodo = parent->ptr[rightSibling];
		if (rightNodo->size >= (MAX + 1) / 2) {
			cursor->key[cursor->size] = parent->key[pos];
			parent->key[pos] = rightNodo->key[0];
			for (int i = 0; i < rightNodo->size - 1; ++i) {
				rightNodo->key[i] = rightNodo->key[i + 1];
			}
			cursor->ptr[cursor->size + 1] = rightNodo->ptr[0];
			for (int i = 0; i < rightNodo->size; ++i) {
				rightNodo->ptr[i] = rightNodo->ptr[i + 1];
			}
			cursor->size++;
			rightNodo->size--;
			return;
		}
	}
	if (leftSibling >= 0) {
		Nodo *leftNodo = parent->ptr[leftSibling];
		leftNodo->key[leftNodo->size] = parent->key[leftSibling];
		for (int i = leftNodo->size + 1, j = 0; j < cursor->size; ++j) {
			leftNodo->key[i] = cursor->key[j];
		}
		for (int i = leftNodo->size + 1, j = 0; j < cursor->size + 1; ++j) {
			leftNodo->ptr[i] = cursor->ptr[j];
			cursor->ptr[j] = NULL;
		}
		leftNodo->size += cursor->size + 1;
		cursor->size = 0;
		removeInternal(parent->key[leftSibling], parent, cursor);
	} else if (rightSibling <= parent->size) {
		Nodo *rightNodo = parent->ptr[rightSibling];
		cursor->key[cursor->size] = parent->key[rightSibling - 1];
		for (int i = cursor->size + 1, j = 0; j < rightNodo->size; ++j) {
			cursor->key[i] = rightNodo->key[j];
		}
		for (int i = cursor->size + 1, j = 0; j < rightNodo->size + 1; ++j) {
			cursor->ptr[i] = rightNodo->ptr[j];
			rightNodo->ptr[j] = NULL;
		}
		cursor->size += rightNodo->size + 1;
		rightNodo->size = 0;
		removeInternal(parent->key[rightSibling - 1], parent, rightNodo);
	}
}
void BPTree::display(Nodo *cursor, Mat &image, Scalar frame_Color, int font_size, Scalar font_Color,
			int font_weight, int &txtPosX, int &txtPosY, int txtEsp) {

	if (cursor != NULL) {
		for (int i = 0; i < cursor->size; ++i) {
			
			//cout << cursor->key[i] << " ";
			
			Point text_position(txtPosX, txtPosY);
			
			putText(image, to_string(cursor->key[i]), text_position,
				FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);
				
			//Size size = getTextSize(to_string(cursor->key[i]), FONT_HERSHEY_COMPLEX, font_weight, 2, nullptr);
			//cout << size.width << ' ' << size.height << endl;
			
			Point p1(txtPosX - 5, txtPosY - 33);
			Point p2(txtPosX + 55, txtPosY + 13);
			rectangle(image, p1, p2, frame_Color, 2, LINE_8);
			
			txtPosX = txtPosX + txtEsp;
		}
		//cout << "\n";
		txtPosY = txtPosY + txtEsp;
		txtPosX = 80;
		if (cursor->IS_LEAF != true) {
			for (int i = 0; i <= cursor->size; ++i) {
				//display(cursor->ptr[i]);
				display(cursor->ptr[i], image, frame_Color, font_size, font_Color,
					font_weight, txtPosX, txtPosY, txtEsp);
			}
		}
	}
}

void BPTree::dispRoutine(Nodo *cursor) {

	Mat image(500, 500, CV_8UC3, Scalar(200, 235, 225));
	
	int font_size = 1;
	int font_weight = 2;
	Scalar font_Color(22, 22, 165);
	Scalar frame_Color(87, 34, 37);
	
	int txtPosX = 80;
	int txtPosY = 80;
	int txtEsp = 60;
	
	display(cursor, image, frame_Color, font_size, font_Color, font_weight, txtPosX, txtPosY, txtEsp);
	
	imshow("Binary Plus Tree", image);
	waitKey(0);
}

Nodo *BPTree::getRoot() {
	return root;
}
#endif

