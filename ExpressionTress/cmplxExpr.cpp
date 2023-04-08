
#include <bits/stdc++.h>

using namespace std;

int precedence(char op)
{
	if(op == '+'||op == '-')
		return 1;
	if(op == '*'||op == '/')
		return 2;

	return 0;
}

double applyOp(double a, double b, char op)
{
	switch(op) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return a / b;
	}
	
	return 0;
}

double applyUop(double a, string u_op)
{
	if (u_op == "sen")
		return sin(a);
	else if (u_op == "cos")
		return cos(a);
	else if (u_op == "sqrt")
		return sqrt(a);
	else
		return 0;
}

double evaluate(string tokens){

	int i;
	
	stack <double> values;
	
	stack <char> ops;
	
	stack <string> u_ops;
	
	for(i = 0; i < tokens.length(); i++){
		
		if(tokens[i] == ' ')
			continue;
		
		else if(tokens[i] == '('){
			ops.push(tokens[i]);
		}
		
		else if(isdigit(tokens[i]) && u_ops.empty()){
			double val = 0.0;

			while(i < tokens.length() && isdigit(tokens[i]))
			{
				val = (val * 10) + (tokens[i] - '0');
				i++;
			}
			
			values.push(val);
			
			i--;
		}
		
		else if(isdigit(tokens[i]) && !u_ops.empty()){
		
			double val = 0.0;
			
			while(i < tokens.length() && isdigit(tokens[i]))
			{
				val = (val * 10) + (tokens[i] - '0');
				i++;
			}
			
			val = applyUop(val, u_ops.top());
			
			values.push(val);
			
			u_ops.pop();
			
			i--;
		}
		
		else if(isalpha(tokens[i])){
		
			string unary;
			
			if (tokens[i] == 'x'){
				double x;
				cout << "\nX value: ";
				cin >> x;
				if (!values.empty()){
					x = values.top() * x;
					values.pop();
				}
				values.push(x);
			}
			
			else if (tokens[i] == 'y'){
				double y;
				cout << "\nY value: ";
				cin >> y;
				if (!values.empty()){
					y = values.top() * y;
					values.pop();
				}
				values.push(y);					
			}
			
			else {			
				while(i < tokens.length() && isalpha(tokens[i]))
				{
					unary.push_back(tokens[i]);
					i++;
				}
				
				u_ops.push(unary);
				
				i--;
			}
		}
		
		else if(tokens[i] == ')')
		{
			
			while(!ops.empty() && ops.top() != '(')
			{
				double val2 = values.top();
				values.pop();
				
				double val1 = values.top();
				values.pop();
				
				char op = ops.top();
				ops.pop();
				
				values.push(applyOp(val1, val2, op));
			}
			
			if(!ops.empty())
				ops.pop();
		}
		
		else
		{

			while(!ops.empty() && precedence(ops.top()) >= precedence(tokens[i])){
				double val2 = values.top();
				values.pop();
				
				double val1 = values.top();
				values.pop();
				
				char op = ops.top();
				ops.pop();
				
				values.push(applyOp(val1, val2, op));
			}
			
			ops.push(tokens[i]);
		}
	}
	
	while(!ops.empty()){
		double val2 = values.top();
		values.pop();
				
		double val1 = values.top();
		values.pop();
				
		char op = ops.top();
		ops.pop();
				
		values.push(applyOp(val1, val2, op));
	}
	
	return values.top();
}

int main() {

	cout << "10 + 3 * sen6 = " << evaluate("10 + 3 * sen6") << "\n";
	cout << "sqrt100 * 2 + 12 = " << evaluate("sqrt100 * 2 + 12") << "\n";
	cout << "100 * ( 2 + 12 + cos2) = " << evaluate("100 * ( 2 + 12 + cos2)") << "\n";
	cout << "100 * ( 2x + 12y ) / 14 = " << evaluate("100 * ( 2x + 12y ) / 14");
	cout << endl;
	return 0;
}
