#include <iostream>
#include <stack>
#include <queue>
#include <list>
using namespace std;

int main(int argc, char const *argv[]);

/*
** Binary Search Tree （二叉搜索树，BST）的基本方法非递归实现及相关应用扩展
*/

class Node
{
public:
	int key;
	Node *left, *right;
	Node(int key=-1) : key(key), left(NULL), right(NULL) {}
};

class BST
{
private:
	Node *root;
public:
	BST() : root(NULL) {}

/*
基本操作: 增删改查
		 非递归的写操作需要额外记录：①记录父节点指针 ②记录左右
*/


	// 实际上，树常用来实现键值对（key-value pair/ dict / map）的高效增删改查，但考试中的树是做集合（set）
	// 因此这里get叫做contains、返回值为bool更合适
	int get(int key) {
		Node *x = root;
		while (x) {
			if (key < x->key)
				x = x->left;
			else if (key > x->key)
				x = x->right;
			else
				return key;
		}
		return -1;
	}

	void put(int key) {
		if (!root) {
			root = new Node(key);
			return;
		}
		Node *x = root, *p = NULL;
		bool go_left = true;
		while (x) {
			if (key < x->key) {
				p = x;
				x = x->left;
				go_left = true;
			}
			else if (key > x->key) {
				p = x;
				x = x->right;
				go_left = false;
			}
			else {
				cout << "Duplicate key: " << key << endl;
				return;
			}
		}
		if (!x && p) {
			if (go_left)
				p->left = new Node(key);
			else
				p->right = new Node(key);
		}
	}

	// 删除任意结点：①查找 ②使用右子树中的最小结点替代目标结点
	//（借助del_min删除右子树中的最小结点，min查找右子树中的最小结点）
	// 写的不好，递归版更清晰易读，没耐心请跳过这个大方法
	void remove(int key) {
		if (!root)
			return;
		// 冗余：需要修改root的情形
		if (root->key == key) {
			if (root->right == NULL) {
				Node *t = root->left;
				delete root;
				root = t;
				return;
			}
			if (root->left == NULL) {
				Node *t = root->right;
				delete root;
				root = t;
				return;
			}

			Node *t = new Node(min(root->right)->key);
			del_min(root->right);
			t->left = root->left;
			t->right = root->right;
			delete root;
			root = t;
			return;
		}
		Node *x = root, *p = NULL;
		bool go_left = true;
		while (x) {
			if (key < x->key) {
				p = x;
				x = x->left;
				go_left = true;
			}
			else if (key > x->key) {
				p = x;
				x = x->right;
				go_left = false;
			} else
				break;
		}
		if (x == NULL) {
			cout << "No such key." << endl;
			return;
		}
		if (x->right == NULL) {
			if (go_left)
				p->left = x->left;
			else
				p->right = x->left;
			delete x;
		} else if (x->left == NULL) {
			if (go_left)
				p->left = x->right;
			else
				p->right = x->right;
			delete x;
		} else {
			Node *t = new Node(min(x->right)->key);
			del_min(x->right);
			t->left = x->left;
			t->right = x->right;
			delete(x);
			if (go_left)
				p->left = t;
			else
				p->right = t;
		}
	}

	Node *min(Node *x) {
		for (; x->left; x = x->left);
		return x;
	}

	// 删除最小键
	void del_min(Node *&r) {
		// r: subtree root
		if (!r)
			return;
		if (!r->left) {
			Node *x = r;
			r = r->right;
			delete x;
			return;
		}
		// 找到最左
		Node *p = NULL, *x = r;
		for (; x->left; p = x, x = x->left);
		// 此时 x->left == NULL, p->left == x
		p->left = x->right;
		delete x;
	}

	
	void say_something(Node *x) {
		cout << '\'' << x->key << '\'' << ", ";
	}

/*
深度优先遍历
*/
	// 先序遍历：完全借助栈来决定遍历顺序（先中后序的非递归写法中，先序遍历最好写）
	// 另一种写法是：调整中序遍历中的say_something(x);到s.push(x)之前即可
	void pre_traverse() {
		if (!root)
			return;
		Node *x = root;
		stack<Node*> s;
		s.push(x);
		while (!s.empty()) {
			x = s.top(); s.pop();
			say_something(x);
			if (x->right)	s.push(x->right);
			if (x->left)	s.push(x->left);
		}
	}

	// 中序遍历：①一路向左 ②适时调整方向右转
	void in_traverse() {
		if (!root)
			return;
		Node *x = root;
		stack<Node*> s;
		while (!s.empty() || x) {
			// x != NULL 时执行
			for (; x; x=x->left)
				s.push(x);
			// !s.empty() 一定是true
			x = s.top(); s.pop();
			say_something(x);
			x = x->right;			// 右转
		}
	}

	// 后序遍历：中序遍历基础上，需要判断何时可以访问当前结点，多用一个指针保存上一次访问的结点
	// 访问当前结点的条件：right is NULL or last visited is right ，否则，右转
	void post_traverse() {
		if (!root)
			return;
		Node *x = root, *last = NULL;
		stack<Node*> s;
		while (!s.empty() || x) {
			for (; x; x = x->left)
				s.push(x);
			x = s.top();
			// 当right为NULL或者上一次访问的结点为right
			if (x->right == NULL || last == x->right){
				say_something(x);
				last = x;
				x = NULL;			// 把x置为NULL，下一次会直接『向上回溯』	
				s.pop();			// 当且仅当当前结点访问完成，才从栈中移除该结点
			} else {
				x = x->right;		// last != x->right && x->right != NULL, 右转
			}
		}
	}

/*
广度优先遍历
*/
	// 层次遍历
	void level_traverse() {
		Node *x = root;
		queue<Node*> q;
		q.push(x);
		while (!q.empty()) {
			x = q.front(); q.pop();
			say_something(x);
			if (x->left)	q.push(x->left);
			if (x->right) 	q.push(x->right);
		}
	}

/*
非递归遍历算法应用
*/
	// 删除任意子树，应用非递归先序遍历
	// （可以用深度优先遍历删除，也可以广度优先，如果要求非递归，先序遍历和层序遍历最简单）
	void remove_subtree(Node *&t) {
		if (!t)
			return;
		stack<Node*> s;
		Node *x = t;
		s.push(x);
		while (!s.empty()) {
			x = s.top(); s.pop();
			if (x->left) s.push(x->left);
			if (x->right) s.push(x->right);
			delete x;
		}
		t = NULL;
	}

	/*
	删除小等于k的所有结点（非递归实现）
	分解：向左搜索小等于k的结点，①其左子树结点必全部小于k ②继续搜索其右子树
	简化：①使用remove_subtree方法 ②使用辅助根节点，省得单独处理root
	*/
	void remove_less_than(int k) {
		// 把root挂到aux左边，这样即使root将来变了也无需单独处理
		// 如开头所述，非递归的写操作通常要记录父节点指针，辅助结点就是给root也安了个父节点
		Node aux(k+1), *p = &aux, *x = root;
		aux.left = root;		// 
		while (x) {
			if (k < x->key) {
				p = x;
				x = x->left;
			}
			else { // x->key <= k
				p->left = x->right;
				x->right = NULL;	// x->right脱下来挂到父节点左边，因为总是删除左子树
				remove_subtree(x);
				x = p->left;
			}
		}
		root = aux.left;
	}

	~BST() {
		remove_subtree(root);
	}
};


int main(int argc, char const *argv[])
{
	BST T;
	vector<int> pre = {'S', 'E', 'A', 'C', 'R', 'H', 'M', 'X'};
	for (auto i : pre)
		T.put(i);
	T.level_traverse();
	cout << endl;
	T.pre_traverse();
	cout << endl;
	T.remove_less_than('M');
	T.in_traverse();
	cout << endl;
	return 0;
}