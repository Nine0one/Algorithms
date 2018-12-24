#include <iostream>
#include <stack>
using namespace std;


class Node{
public:
	int value;
	Node* next;
	Node (int value=0, Node *next=NULL) {
			this->value = value;
			this->next = next;
	}
};


class List {
public:
	Node* first;
	Node* last;
	int N;	//长度计数

public:
	List() {
		// 头结点不保存数据
		first = new Node();
		last = first;
		first->next = NULL;
		N = 0;
	}

	void push(int x) {
		Node *node = new Node(x, first->next);
		first->next = node;
		if (last == first)
			last = node;
		N++;
	}

	void append(int x) {
		Node *node = new Node(x, NULL);
		last->next = node;
		last = node;
		N++;
	}

	int remove(int x) {
		Node *k=first;
		while(k->next) {
			if (k->next->value == x) {
				k->next = k->next->next;
				N--;
				return x;
			}
			k = k->next;
		}
		return -1;
	}

	int pop() {
		int x = first->next->value;
		Node *next = first->next->next;
		delete first->next;
		first->next = next;
		N--;
		return x;
	}

	void reverse() {
		Node *k = first->next, *next;
		first->next = NULL;
		while (k) {
			push(k->value);
			k = k->next;
		}
	}

	~List() {
		Node *i = first, *next;
		while(i) {
			next = i->next;
			delete i;
			i = next;
		}
	}

	string toString() {
		string s;
		for (Node *i = first->next; i != NULL; i = i->next) {
			s += (char)i->value;
			if (i->next)
				s += "->";
		}
		return s;
	}
};

/*
两个链表的公共结点（Y形链表）

简单做法：
利用List.N计数，或单独遍历一次求出长度

复杂做法：
https://github.com/hit9/oldblog/blob/gh-pages/blog-src/blog/C/posts/25.mkd
假设两个链表a,b.a比b长k个结点(k>=0).
那么当a_ptr,b_ptr两个指针同时分别遍历a,b的时候, 必然b_ptr先到达结尾(NULL),而此时a_ptr落后a的尾巴k个结点.
如果此时再从a的头发出一个指针t,继续和a_ptr 一起走,当a_ptr达到结尾(NULL)时,t恰好走了k个结点.此时从b的头发一个指针s, s和t一起走,因为a比b长k个结点,所以,t和s会一起到达交点.

::p,q分别遍历链表a,b,假设q先到达NULL,此时从a的头发出一个指针t,当p到达NULL时,从b的头发出s,当s==t的时候即交点.
*/

int lenth(Node *p) {
	int count = 0;
	for (; p; p = p->next)
		count++;
	return count;
}

Node *intersect(Node *l1, Node *l2) {
	// l1 存放较长的链表
	int k = lenth(l1) - lenth(l2);
	if (k < 0) swap(l1, l2);
	k = abs(k);
	// 使得两链表等长
	for (; k > 0; k--, l1 = l1->next);
	for (; l1 != NULL && l1 != l2; l1 = l1->next, l2 = l2->next);
	return l1;
}

Node *intersect2(Node *l1, Node *l2) {
	Node *p, *q, *k, *t, *s;
	for (p=l1, q=l2; p&&q; p=p->next, q=q->next);
	k = p == NULL ? q : p;
	t = p == NULL ? l2 : l1;
	s = p == NULL ? l1 : l2;
	for (; k; t = t->next, k = k->next);
	for (; t != s && t && s; t = t->next, s = s->next);
	return t;
}

bool is_intersect(Node *l1, Node *l2) {
	Node *p, *q;
	for (p=l1; p->next; p = p->next);
	for (q=l2; q->next; q = q->next);
	return p==q;
}

/*
环路检测
*/ 
bool has_cycle(Node *l) {
	Node *p = l, *q = l;
	while (p && p->next){
		p = p->next->next;
		q = q->next;
		if (p == q)
			return true;
	}
	return false;
}

/*
查找中间结点: p走一步q跑两步
*/ 
Node *middle(Node *l) {
	Node *p = l, *q = l;
	while(p&&p->next) {
		p = p->next->next;
		q = q->next;
	}
	return q;
}

// 倒数第K个元素
int lastKth(Node *head, int k) {
	Node *p = head, *q = head;
	int count = 0;
	while (p) {
		p = p->next;
		count++;
		if (count > k)
			q = q->next;
	}
	return q->value;
}

int main(int argc, char const *argv[])
{
	Node e = {'e', 0}, d = {'d', &e}, c = {'c', &d}, b = {'b', &c}, a = {'a', &b}; 
	Node z = {'z', &c}, y = {'y', &z}, x = {'x', &y};
	cout << (char)lastKth(&a, 1) << endl;
	cout << (char)intersect2(&a, &x)->value << endl;
	cout << is_intersect(&a, &x) << endl;
	return 0;
}

