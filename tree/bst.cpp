#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <list>
#include <exception>

using namespace std;


class BST
{
public:
	class Node
	{
	public:
		int val;
		Node *left, *right;
		Node(int val=0) {
			this->val = val;
			this->left = NULL;
			this->right = NULL;
		}
	};
private:
	Node *root;

// 注：从封装的角度，下面的一些方法应当声明为private，但考虑到应试以及可读性就都设为public了
public:
	BST() {
		this->root = NULL;
	}

	// 增
	void put(int val) {
		this->root = put(this->root, val);
	}

	Node *put(Node *x, int val) {
		if (x == NULL)
			return new Node(val);
		if (val < x->val)
			x->left = put(x->left, val);
		else if (val > x->val)
			x->right = put(x->right, val);
		else
			x->val = val;
		return x;
	}

	// 查
	int get(int val) {
		return get(root, val);
	}

	int get(Node *x, int val) {
		if (x == NULL) 
			return -1;
		if (val < x->val)
			return get(x->left, val);
		else if (val > x->val)
			return get(x->right, val);
		else
			return x->val;
	}

	Node *min(Node *x) {
		if (x->left == NULL)
			return x;
		return min(x->left);
	}

	// 删除最小键
	void del_min() {
		del_min(root);
	}

	Node *del_min(Node *x) {
		if (x->left == NULL) {
			// 最左结点就是最小结点，把当前结点删掉，它的右结点替代它的位置
			return x->right;
		}
		x->left = del_min(x->left);
		return x;
	}

	// 删除任意键
	void remove(int val) {
		remove(root, val);
	}

	Node *remove(Node *x, int val) {
		if (x == NULL)
			return NULL;
		if (val < x->val)
			x->left = remove(x->left, val);
		else if (val > x->val)
			x->right = remove(x->right, val);
		else {
			//
			if (x->right == NULL)
				return x->left;
			if (x->left == NULL)
				return x->right;
			Node *t = x;
			x = min(t->right);
			x->right = del_min(t->right);
			x->left = t->left;
		}
		return x;
	}

	void in_traverse() {
		in_traverse(root);
	}

	void pre_traverse() {
		pre_traverse(root);
	}
	
	void pre_traverse(Node *x) {
		if (x == NULL)
			return;
		say_something(x);
		pre_traverse(x->left);
		pre_traverse(x->right);
	}

	void in_traverse(Node *x) {
		if (x == NULL)
			return;
		in_traverse(x->left);
		say_something(x);
		in_traverse(x->right);
	}

	// 树高
	int depth() {
		return depth(root);
	}

	int depth(Node *x) {
		if (x == NULL)
			return 0;
		int l = depth(x->left);
		int r = depth(x->right);
		return l > r ? l + 1 : r + 1;
	}

	// 层序遍历
	void level_traverse() {
		queue<Node*> q;
		q.push(root);
		while (!q.empty()) {
			Node *x = q.front(); q.pop();
			say_something(x);
			if (x->left) q.push(x->left);
			if (x->right) q.push(x->right);
		}
	}

	// 树宽度：最宽的一层的节点数
	// 层序遍历应用
	int width() {
		Node *x = NULL;
		queue<Node*> q;
		q.push(root);
		int max_width = 0;
		while(!q.empty()) {
			// q.size()记录了上一层的节点数，依次取出上一层所有结点，并将其子结点入队
			int w = q.size();
			if (w > max_width)
				max_width = w;
			for (; w > 0; w--) {
				x = q.front();	q.pop();
				if (x->left)	q.push(x->left);
				if (x->right)	q.push(x->right);
			}
		}
		return max_width;
	}

	// 层次遍历应用：完全二叉树判定
	// 层序中一旦出现前面结点为NULL而后面不为NULL，即非完全二叉树
	bool is_complete() {
		if (!root)
			return true;
		queue<Node*> q;
		q.push(root);
		Node *x = NULL;
		bool has_null = false;
		while (!q.empty()) {
			x = q.front();	q.pop();
			if (x != NULL) {
				if (has_null) // x != NULL and has null before
					return false;
				q.push(x->left);
				q.push(x->right);
			} else if (!has_null){
				has_null = true;
			}
		}
		return true;
	}

	// 删除任意子树，释放内存
	void remove_subtree(Node *x) {
		if (x == NULL)
			return;
		remove_subtree(x->left);
		remove_subtree(x->right);
		delete x;
	}

	void say_something(Node *x) {
		cout << '\'' << x->val << '\'' << ", ";
	}

	~BST() {
		remove_subtree(root); 
	}
};

int main(int argc, char const *argv[])
{
	BST T;
	return 0;
}

