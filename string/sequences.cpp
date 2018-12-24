#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

// 最大连续子序列(暴力)
void max_subsequence_sum_brute(vector<int> a) {
	int max_sum=0, start = 0, end = 0;
	for (int i=0; i < a.size(); i++) {	//左边界
		for (int j=i; j < a.size(); j++) {		//右边界
			int sum = 0;
			for (int k = i; k <= j; k++)
				sum += a[k];
			if (sum > max_sum) {
				max_sum = sum;
				start = i;
				end = j;
			}
		}
	}
	cout << max_sum << " " << a[start] << " " << a[end] << endl;
}

// 最大连续子序列（暴力）
void max_subsequence_sum_brute2(vector<int> a) {
	int max_sum=0, start = 0, end = 0;
	for (int i=0; i < a.size(); i++) {	//左边界
		int sum = 0;
		for (int j=i; j < a.size(); j++) {		//右边界
			sum += a[j];
			if (sum > max_sum) {
				max_sum = sum;
				start = i;
				end = j;
			}
		}
	}
	cout << max_sum << " " << a[start] << " " << a[end] << endl;
}

// 最大连续子序列（线性复杂度）：非常不易理解的写法，没必要
// https://my.oschina.net/itblog/blog/267860
void max_subsequence_sum(vector<int> a) {
	int max_sum = 0, sum = 0, start = 0, end = 0;
	for (int i=0; i < a.size(); i++) {
		if (sum < 0) {
			sum = 0;
			start = i;
		}
		sum += a[i];
		if (sum > max_sum) {
			max_sum = sum;
			end = i;
		}
	}
	cout << max_sum << " " << a[start] << " " << a[end] << endl;
}

int main(int argc, char const *argv[])
{
	int n;
	while(cin>>n) {
		if (n <= 0)
			break;
		vector<int> a(n);
		for (int i = 0; i < n; i++)
			cin >> a[i];
		max_subsequence_sum(a);
	}
	return 0;
}
