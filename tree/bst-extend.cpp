#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <list>
#include <stack>
#include <exception>

using namespace std;


class BST
{
public:
	class Node
	{
	public:
		int key;
		Node *left, *right;
		Node(int key=0) : key(key), left(NULL), right(NULL){}
	};
private:
	Node *root;

public:
	BST() : root(NULL) {}

	/*
	已知中序遍历和先（后）序遍历，构建二叉树
	*/
	void rebuild(vector<int> pre, vector<int> vin) {
		if (pre.empty() || pre.size() != vin.size())
			throw;
		root = rebuild(pre, vin, 0, pre.size()-1, 0, vin.size()-1);

	}

	Node *rebuild(vector<int> pre, vector<int> vin, int lo1, int hi1, int lo2, int hi2) {
		if (lo1 > hi1 || lo2 > hi2)
			return NULL;
		int pivot = pre[lo1];
		Node *x = new Node(pivot);
		int i = lo2;
		for (; vin[i] != pivot && i <= hi2; i++);
		int l_size = i - lo2;	// 左子树节点数
		x->left = rebuild(pre, vin, lo1+1, lo1+l_size, lo2, i-1);
		x->right = rebuild(pre, vin, lo1+l_size+1, hi1, i+1, hi2);
		return x;
	}

	/* 
	遍历的应用：删除二叉树（子树）
	*/
	void remove_subtree(Node *x) {
		// 先序遍历
		if (!x)
			return;
		Node *p = x->left, *q = x->right;
		delete(x);
		remove_subtree(p);
		remove_subtree(q);
	}

	/* 
	进一步：删除小等于k的全部结点，查找+删除子树
	进一步简化版见bst-iterative
	*/
	void remove_less_than(int k) {
		while (root && root->key <= k) {
			Node *temp = root->right;
			root->right = NULL;
			remove_subtree(root);
			root = temp;
		}
		Node *x = root, *p = NULL;	//x is current node, p is parent node
		while (x) {
			if (k < x->key) {
				p = x;
				x = x->left;
			} else {
				p->left = x->right;
				x->right = NULL;
				remove_subtree(x);
				x = p->left;
				// 可以提前退出
			}
		}
	}

	/*
	所有祖先，即根节点到任意结点的完整路径
	*/
	void ansestors(int k) {
		stack<int> s;
		ansestors(root, k, s);
		cout << endl;
	}

	void ansestors(Node *x, int k, stack<int> &s) {
		if (!x)
			return;
		if (x->key == k) {
			// 注：C++的stack有个不好的特性：无法遍历，建议用list（双向链表）等结构替代stack
			stack<int> bak = s;
			while (!bak.empty()){
				cout << bak.top() << " ";
				bak.pop();
			}
			return;
		}
		s.push(x->key);
		ansestors(x->left, k, s);
		ansestors(x->right, k, s);
		s.pop();
	}

	/*
	将叶子结点链接起来，破坏了树的结构会导致无法释放内存，测试时请注释掉析构函数
	*/ 
	Node *link_leafs() {
		Node x, *pre=&x;
		link_leafs(root, pre);
		return x.right;
	}

	void link_leafs(Node *x, Node *&pre) {
		if (!x)
			return;
		if (!x->left && !x->right) {
			pre->right = x;
			pre = x;
		}
		link_leafs(x->left, pre);
		link_leafs(x->right, pre);
	}

	void say_something(Node *x) {
		cout << '\'' << x->key << '\'' << ", ";
	}

	~BST() {
		remove_subtree(root);
	}
};


int main(int argc, char const *argv[])
{
	BST T;
	vector<int> pre = {'1', '2','4','8','9','5','0','3','6','7'};
	vector<int> vin = {'8','4','9','2','0','5','1','6','3','7'};
	T.rebuild(pre, vin);
	T.ansestors('8');
	return 0;
}

