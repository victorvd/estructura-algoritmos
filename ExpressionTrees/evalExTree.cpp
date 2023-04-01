
#include <iostream>

using namespace std;

class ExprNode {
	public:
	char data;
	ExprNode *left, *right;
	
	ExprNode(char data) {
		this->data = data;
		this->left = NULL;
		this->right = NULL;
	}
};

class StackNode {
	public:
	ExprNode *trNode;
	StackNode *next;
	
	StackNode(ExprNode *trNode) {
		this->trNode = trNode;
		next = NULL;
	}
};

class ExpressionTree {
	private:
	StackNode *top;

	public:
	ExpressionTree() {
		top = NULL;
	}

	/* Clear tree
	void clear() {
		top = NULL;
	}*/

	// Push a node
	void push(ExprNode *ptr) {
		if (top == NULL)
			top = new StackNode(ptr);
		else {
			StackNode *nptr = new StackNode(ptr);
			nptr->next = top;
			top = nptr;
		}
	}

	// Pop a node
	ExprNode *pop() {
		if (top == NULL) {
			cout<<"Underflow"<<endl;
		}
		else {
			ExprNode *ptr = top->trNode;
			top = top->next;
			return ptr;
		}
		
		return 0;
	}

	// Get top node
	ExprNode *peek() {
		return top->trNode;
	}

	// Insert character
	void insert(char val) {
		if (isDigit(val)) {
			ExprNode *nptr = new ExprNode(val);
			push(nptr);
		}
		else if (isOperator(val)) {
			ExprNode *nptr = new ExprNode(val);
			nptr->left = pop();
			nptr->right = pop();
			push(nptr);
		}
		else {
			cout<<"Invalid Expression"<<endl;
			return;
		}
	}

	// Check if digit
	bool isDigit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	// Check if operator
	bool isOperator(char ch) {
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	} 

	// Convert character to digit
	int toDigit(char ch) {
		return ch - '0';
	}

	// Build tree from input
	void buildTree(string eqn) {
		for (int i = eqn.length() - 1; i >= 0; i--)
		insert(eqn[i]);
	}

	// Evaluate peek element
	double evaluate() {
		return compute(peek());
	}

	// Evaluate tree
	double compute(ExprNode *ptr) {
		if (ptr->left == NULL && ptr->right == NULL)
			return toDigit(ptr->data);
		else {
			double result = 0.0;
			double left = compute(ptr->left);
			double right = compute(ptr->right);
			char op = ptr->data;
			switch (op) {
				case '+': 
					result = left + right;
					break;
				case '-': 
					result = left - right;
					break;
				case '*': 
					result = left * right;
					break;
				case '/': 
					result = left / right;
					break;
				default: 
					result = 0.0;
					break;
			}
			return result;
		}
	}

	// Get postfix expression
	void postfix() {
		postOrder(peek());
	}

	// Post order traversal
	void postOrder(ExprNode *ptr) {
		if (ptr != NULL) {
			postOrder(ptr->left);
			postOrder(ptr->right);
			cout<<ptr->data;
		}
	}

	// Get infix expression
	void infix() {
		inOrder(peek());
	}

	// In order traversal
	void inOrder(ExprNode *ptr) {
		if (ptr != NULL) {
			inOrder(ptr->left);
			cout<<ptr->data;
			inOrder(ptr->right);
		}
	}

	// Get prefix expression
	void prefix() {
		preOrder(peek());
	}

	// Pre order traversal
	void preOrder(ExprNode *ptr) {
		if (ptr != NULL) {
			cout<<ptr->data;
			preOrder(ptr->left);
			preOrder(ptr->right);
		}
	}
};


int main()
{
	//test: +-+7*/935/82*/625
	string expForm;
	ExpressionTree exTr;
	cout<<"\nEnter expression in prefix form: ";
	cin>>expForm;
	exTr.buildTree(expForm);
	cout<<"\nPrefix  : ";
	exTr.prefix();
	cout<<"\n\nInfix   : ";
	exTr.infix();
	cout<<"\n\nPostfix : ";
	exTr.postfix();
	cout<<"\n\nEvaluated Result : "<<exTr.evaluate()<<endl<<endl;
	return 0;
}
