#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <unordered_map>
#include <cassert>
using namespace std;

static unordered_map<char, int> precedence({{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}});

/*
处理和计算数学表达式，主要是shunting yard算法，算法比较简单，掌握该算法可以处理多种优先级混合，含有括号等的表达式
更复杂就涉及到编译原理相关的知识了，略
*/

/*
热身：括号匹配
*/
bool is_brackets_match(string &exp) {
	stack<char> s;
	for (int i = 0; i < exp.size(); i++) {
		char &c = exp[i];
		if (c == '\''){ // '引号内注释跳过检测'
			for (i++; i < exp.size() && exp[i] != '\''; i++);
			i++;
		} else if (c == '(' || c == '[') {
			s.push(c);
		} else if (c == ')') {
			if (s.empty() || s.top() != '(')
				return false;
			s.pop();
		} else if (c == ']') {
			if (s.empty() || s.top() != '[')
				return false;
			s.pop();
		}
	}
	if (!s.empty())	return false;
	return true;
}


double op(char c, double i, double j) {
	if (c == '+')
		return i+j;
	else if (c == '-')
		return i-j;
	else if (c == '*')
		return i*j;
	else if (c == '/')
		return i / j;
	cout << "Unsupport operator " << c << "." << endl;
	return 0;
}

/*
计算后缀表达式，如"312*+"
*/
double calculate_postfix(string &exp) {
	stack<double> s;
	for (int i = 0; i < exp.size(); i++) {
		char c = exp[i];
		if (isdigit(c))
			s.push(c-'0');
		else {
			double r = s.top(); s.pop();
			double l = s.top(); s.pop();
			s.push(op(c, l, r));
		}
	}
	return s.top();
}


/*
调度场算法（Shunting Yard Algorithm）是一个用于将中缀表达式转换为后缀表达式的经典算法，因其操作类似于火车编组场而得名。
主要参考资料：https://en.wikipedia.org/wiki/Shunting-yard_algorithm
总结较好的资料：https://boycgit.github.io/algorithm-shunting-yard/
*/


/*

// This implementation does not implement composite functions,functions with variable number of arguments, and unary operators.

while there are tokens to be read:
    read a token.
    if the token is a number, then:
        push it to the output queue.
    if the token is a function then:
        push it onto the operator stack 
    if the token is an operator, then:
        while ((there is a function at the top of the operator stack)
               or (there is an operator at the top of the operator stack with greater precedence)
               or (the operator at the top of the operator stack has equal precedence and is left associative))
              and (the operator at the top of the operator stack is not a left bracket):
            pop operators from the operator stack onto the output queue.
        push it onto the operator stack.
    if the token is a left bracket (i.e. "("), then:
        push it onto the operator stack.
    if the token is a right bracket (i.e. ")"), then:
        while the operator at the top of the operator stack is not a left bracket:
            pop the operator from the operator stack onto the output queue.
        pop the left bracket from the stack.
        //if the stack runs out without finding a left bracket, then there are mismatched parentheses.
if there are no more tokens to read:
    while there are still operator tokens on the stack:
        // if the operator token on the top of the stack is a bracket, then there are mismatched parentheses. 
        pop the operator from the operator stack onto the output queue.
exit.
*/

// 首先实现一个最简版本的调度场算法，将中缀表达式转为后缀字符串，表达式中仅有+-*/以及一位数字
// 思路：使用一个栈s存放符号，逐字符读取c，遇到数字直接输出
// 		遇到符号则先判别优先级：如果栈顶符号优先级较高，将栈中所有优先级高于c的符号输出
string infix_to_postfix_simple(string &exp) {
	string r;
	stack<char> s;
	for (int i = 0; i < exp.size(); i++) {
		char c = exp[i];
		if (isspace(c))
			continue;
		else if (isdigit(c)) {
			r += c;
		} else {
			while (!s.empty() && precedence[s.top()] >= precedence[c]) {
				r += s.top(); s.pop();
			}
			s.push(c);
		}
	}
	while (!s.empty()) {
		r += s.top(); s.pop();
	}
	return r;
}

// 允许使用括号调整优先级
// 遇到左括号入符号栈，右括号则直接输出栈中符号，直到遇到左括号
string infix_to_postfix(string &exp) {
	assert(exp.size() > 0);
	string r;
	stack<char> s;
	for (int i = 0; i < exp.size(); i++) {
		char c = exp[i];
		if (isspace(c))
			continue;
		else if (isdigit(c)) {
			r += c;
		} else if (c == '(') {
			s.push(c);
		} else if (c == ')') {
			while (!s.empty() && s.top() != '(') {
				r += s.top(); s.pop();
			}
			s.pop();	// 把左括号弹出
		}
		else {
			while (!s.empty() && precedence[s.top()] >= precedence[c]) {
				r += s.top(); s.pop();
			}
			s.push(c);
		}
	}
	while (!s.empty()) {
		r += s.top(); s.pop();
	}
	return r;
}

// 直接计算表达式的值，不再输出后缀式
// 整合calculate_postfix和infix_to_postfix_2即可，要用两个栈，一个符号栈一个数字（运算）栈
// 处理中序表达式围绕符号栈操作，计算后缀表达式围绕数字入数字栈操作
// 简言之，遇到数字入数字栈，遇到符号入符号栈，需要运算时，从两个栈中分别取出数字和符号，计算结果入数字栈
double calculator_simple(string &exp) {
	stack<char> s_op;
	stack<double> s_num;
	for (int i = 0; i < exp.size(); i++) {
		char c = exp[i];
		if (isspace(c))
			continue;
		else if (isdigit(c)) {
			s_num.push(c-'0');
		} else if (c == '(') {
			s_op.push(c);
		} else if (c == ')') {
			while (!s_op.empty() && s_op.top() != '(') {
				char symbol = s_op.top(); s_op.pop();	//①
				double r = s_num.top(); s_num.pop();
				double l = s_num.top(); s_num.pop();
				s_num.push(op(symbol, l, r));
			}
			s_op.pop();									// 把左括号弹出
		}
		else {
			while (!s_op.empty() && precedence[s_op.top()] >= precedence[c]) {
				char symbol = s_op.top(); s_op.pop();	// ②
				double r = s_num.top(); s_num.pop();
				double l = s_num.top(); s_num.pop();
				s_num.push(op(symbol, l, r));
			}
			s_op.push(c);
		}
	}
	while (!s_op.empty()) {
		char symbol = s_op.top(); s_op.pop();	// ③ （①②③处代码完全一致的，都是后缀表达式的运算）
		double r = s_num.top(); s_num.pop();
		double l = s_num.top(); s_num.pop();
		s_num.push(op(symbol, l, r));
	}
	return s_num.top();
}

// 允许使用多位数字且允许用小数点，只需修改数字的读取方式即可
double calculator(string &exp) {
	stack<char> s_op;
	stack<double> s_num;
	for (int i = 0; i < exp.size(); i++) {
		char c = exp[i];
		if (isspace(c))
			continue;
		else if (isdigit(c)) {	// 只需要改动这里，用了C++11的substr和stod，如果想兼容旧版可以用C的sscanf
			int j;
			for (j = i+1; isdigit(exp[j]) || exp[j] == '.'; j++);
			double num = stod(exp.substr(i, --j));
			s_num.push(num);
			i = j;
		} else if (c == '(') {
			s_op.push(c);
		} else if (c == ')') {
			while (!s_op.empty() && s_op.top() != '(') {
				char symbol = s_op.top(); s_op.pop();	//①
				double r = s_num.top(); s_num.pop();
				double l = s_num.top(); s_num.pop();
				s_num.push(op(symbol, l, r));
			}
			s_op.pop();									// 把左括号弹出
		}
		else {
			while (!s_op.empty() && precedence[s_op.top()] >= precedence[c]) {
				char symbol = s_op.top(); s_op.pop();	// ②
				double r = s_num.top(); s_num.pop();
				double l = s_num.top(); s_num.pop();
				s_num.push(op(symbol, l, r));
			}
			s_op.push(c);
		}
	}
	while (!s_op.empty()) {
		char symbol = s_op.top(); s_op.pop();	// ③ （①②③处代码完全一致的，都是后缀表达式的运算）
		double r = s_num.top(); s_num.pop();
		double l = s_num.top(); s_num.pop();
		s_num.push(op(symbol, l, r));
	}
	return s_num.top();
}

int main(int argc, char const *argv[])
{
	string exp = "1+2*(3+9*2)-2";
	cout << is_brackets_match(exp) << endl;
	string post = infix_to_postfix(exp);
	cout << exp << " => " << post << endl;
	exp = "100.0*(1+3*4/(2-5))";
	cout << exp << "=" << calculate_postfix(post) << endl;
	cout << exp << "=" << calculator(exp) << endl;
	return 0;
}
