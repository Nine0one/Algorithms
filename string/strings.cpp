#include <iostream>
#include <stack>
#include <queue>
#include <cmath>
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>
#include <list>

using namespace std;

/*
字符串操作：多数字符串的操作都可以用暴力/简单逻辑来实现，也有一些著名的字符串算法和重要的编程思想值得了解（未必考试重要）。
以下几段程序都比较简单的
*/

// 简单分词
list<string> tokenize(string s) {
	string token;
	list<string> tokens;
	for (auto c : s) {
		if (isspace(c)) {
			if (token.size())
				tokens.push_back(token);
			token = "";
		} else {
			token += c;
		}
	}
	if (token.size())
		tokens.push_back(token);
	return tokens;
}

// 将单词中的字母反序
string &reverse_word(string &s) {
	for (int i = 0; i < s.size(); ++i) {
		if (isspace(s[i]))	continue;
		int j;
		for (j = i; j < s.size() && !isspace(s[j]); j++);
		j--;
		for (int lo=i, hi=j; lo < hi; lo++, hi--)
			swap(s[lo], s[hi]);
		i = ++j;
	}
	return s;
}

// 回文（对称）检测
bool is_palindrome(string s) {
	for (int i = 0, j = s.size()-1; i < j; i++, j--)
		if (s[i] != s[j])
			return false;
	return true;
}

/*
 最长回文子串
 错误思路：反转原串，将问题转化为最长公共子序列，如s=abcd123dcba，s' = abcd321dcba
 高级思路：Manacher 算法，线性时间复杂度O(N)，规定时间写不出来的2333，不如写个简单的甚至暴力的
*/

/*
 最长回文子串：暴力方式
 暴力找到字符串的所有子串，遍历每一个子串以验证它们是否为回文串。
 一个子串由子串的起点和终点确定，因此对于一个长度为n的字符串，共有n^2个子串。
 这些子串的平均长度大约是n/2，因此这个解法的时间复杂度是O(n^3)。
 暴力找到所有子串的方式：分别找长度为1~N的子串
*/

int longest_palindrome_brute_force(string &s) {
	int max_len = 1;
	for (int n = s.size(); n > 1; n--) {
		for (int i = 0; i < s.size()-n+1; i++) {
			string sub = s.substr(i, n);	// string.substr(start_pos, length);
			if (is_palindrome(sub))
				max_len = max(int(sub.size()), max_len);
		}
	}
	return max_len;
}

/*
 最长回文子串：两边扩展法（简单高效方式）
 长度为奇数回文串以最中间字符的位置为对称轴左右对称，而长度为偶数的回文串的对称轴在中间两个字符之间的空隙。
 我们知道整个字符串中的所有字符，以及字符间的空隙，都可能是某个回文子串的对称轴位置。
 可以遍历这些位置，在每个位置上同时向左和向右扩展，直到左右两边的字符不同，或者达到边界。
 对于一个长度为n的字符串，这样的位置一共有n+n-1=2n-1个，在每个位置上平均大约要进行n/4次字符比较，于是此算法的时间复杂度是O(n^2)。
*/
int palindrome_length(string &s, int l, int r) {
	for (; s[l] == s[r] && l >=0 && r < s.size(); l--, r++);
	return r-l-1;
}
int longest_palindrome(string &s) {
	int max_len = 1;
	for (int i = 1; i < s.size(); ++i) {
		if (s[i - 1] == s[i])
			max_len = max(palindrome_length(s, i-1, i), max_len);
		else if (s[i-1] == s[i+1])
			max_len = max(palindrome_length(s, i-1, i+1), max_len);
	}
	return max_len;
}

/*
 最长回文子串：动态规划（思路是通用的值得学习，没有基础的略过）
 动态规划（英语：Dynamic programming，简称DP）通过把原问题分解为相对简单的子问题的方式求解复杂问题的方法。
 动态规划常常适用于有重叠子问题和最优子结构性质的问题，动态规划方法所耗时间往往远少于朴素解法。
 动态规划背后的基本思想非常简单。大致上，若要解一个给定问题，我们需要解其不同部分（即子问题），再根据子问题的解以得出原问题的解。
 通常许多子问题非常相似，为此动态规划法试图仅仅解决每个子问题一次，从而减少计算量：
  一旦某个给定子问题的解已经算出，则将其记忆化存储，以便下次需要同一个子问题解之时直接查表。
 这种做法在重复子问题的数目关于输入的规模呈指数增长时特别有用。

 考虑 “ababa”  这个示例:
 如果我们已经知道 “bab”  是回文，那么很明显，“ababa”  一定是回文，因为它的左首字母和右尾字母是相同的。
 我们给出  P(i,j) 的定义如下：
	P(i,j)=true，如果子串 S[i]...S[j] 是回文子串
	P(i,j)=false ，其他情况
	P(i,j)=(P(i+1,j−1) && S[i]==S[j])   ***核心***
 这产生了一个直观的动态规划解法，我们首先初始化一字母和二字母的回文，然后找到所有三字母回文，并依此类推
 参考：https://zhuanlan.zhihu.com/p/38251499
*/

int longest_palindrome_dp(string &s) {
	int max_len = 1, n = s.size();
	bool is_pal[n][n];	//is_pal[i][j]：i-j是否是回文
	for (int i=0; i < n; i++) {
		for (int j=i; j>=0; j--)
			if (s[i] == s[j] && (i-j<2||is_pal[j+1][i-1])) {
				is_pal[j][i] = true;
				max_len = max(max_len, i-j+1);
			}
	}
	return max_len;
}


/*
最长公共子串（Longest Common Substring）与最长公共子序列（Longest Common Subsequence）的区别：
  子串要求在原字符串中是连续的，而子序列则只需保持相对顺序一致，并不要求连续。
  例如X = {a, Q, 1, 1}; Y = {a, 1, 1, d, f}那么，{a, 1, 1}是X和Y的最长公共子序列，但不是它们的最长公共字串。
https://www.kancloud.cn/digest/pieces-algorithm/163624
https://blog.csdn.net/lisonglisonglisong/article/details/41548557
*/

/*
 最长公共子序列
 设 C[i,j] = |LCS(x[1...i],y[1...j])|，
 即C[i,j]表示序列X[1...i]和Y[1...j]的最长公共子序列的长度，则 C[m,n] = |LCS(x,y)|就是问题的解。
 
 function LCSLength(X[1..m], Y[1..n])
    C = array(0..m, 0..n)
    for i := 0..m
       C[i,0] = 0
    for j := 0..n
       C[0,j] = 0
    for i := 1..m
        for j := 1..n
            if X[i] = Y[j]
                C[i,j] := C[i-1,j-1] + 1
            else
                C[i,j] := max(C[i,j-1], C[i-1,j])
    return C[m,n]
*/
int lcs_recursive(string &X, string &Y, int i, int j) {
	if (i == 0 || i == 0)
		return 0;
	if (X[i] == Y[j])
		return lcs_recursive(X, Y, i-1, j-1) + 1;
	else
		return max(lcs_recursive(X, Y, i, j-1), lcs_recursive(X, Y, i-1, j));
}

int LCSLength(string X, string Y) {
	int m = X.size(), n = Y.size();
	int C[m][n];
	for (int i = 0; i < m; i++)
		C[i][0] = 0;
	for (int i = 0; i < n; i++)
		C[0][i] = 0;

	for (int i=0; i < m; i++){
		for (int j=0; j < n; j++) {
			if (X[i] == Y[j])
				C[i][j] = C[i-1][j-1] + 1;
			else
				C[i][j] = max(C[i][j-1], C[i-1][j]);
		}
	}
	return C[m-1][n-1];
}


// 排列对称串：将对称的字符串以长度大小排序并输出，非对称的不输出（相同长度以字符比较排序）
// less_than是辅助函数
bool less_than(string s, string q) {
	// sort辅助函数，比较字符串大小
	if (s.size() < q.size())
		return true;
	else if (s.size() > q.size())
		return false;
	else
		return s < q;	// 交给string库比较
}
void test_palindrome() {
	vector<string> strs;
	string s;
	while(cin >> s) {
		if (s == "0")
			break;
		string t = s;
		reverse(t.begin(), t.end());
		if (t == s)
			strs.push_back(s);
	}
	sort(strs.begin(), strs.end(), less_than);
	for(auto s : strs)
		cout << s << endl;
}


// 暴力子串查找，返回子串首字母所在位置
int substring(string txt, string pat) {
	int M = pat.size(), N = txt.size();
	for (int i = 0; i <= N-M; ++i) {
		int j;
		for (j = 0; j < M; ++j)
			if (txt[i+j] != pat[j])
				break;
		if (j == M)
			return i;
	}
	return -1;
}

// 子串计数
int substring_count(string txt, string pat) {
	int M = pat.size(), N = txt.size();
	int i, j, count = 0;
	for (i=0, j=0; i < N; i++) {
		if (txt[i] == pat[j]) {
			j++;
			if (j == M) {
				count++;
				j = 0;
			}
		}
		else {
			i -= j;
			j = 0;
		}
	}
	return count;
}


/*
水题练手
给定一个只包含'A' ~ 'Z'的字符串，使用下面的方法给它编码：
（1）将字符串中的k个相同的字符写成『kX』，X是字符串中的字符
（2）如果子串的长度是1，那么『1』要忽略
输入：第一行为正整数N，代表测试用例个数。下面N行包含N个字符串，每个字符串仅包含'A'~'Z'，长度小于100.
输出：将每行输入字符串的编码输出为一行
样例：
	2
	ABC
	ABBCCC
输出：
	ABC
	A2B3C
*/
void encode_string() {
	string s, r;
	int lines;
	cin >> lines;
	while (lines--) {
		cin >> s;
		char last = 0;	//last char
		int count = 0;
		for (auto c : s) {
			if (c != last) {
				// 编码并写入
				if (count > 1)
					r += to_string(count);
				r += last;
				// 重置
				count = 1;
				last = c;
			}
			else {
				// 计数
				count++;
			}
		}
		if (count > 1)
			r += to_string(count);
		r += last;
		r += "\n";
	}
	cout << r;
}

/*
与上一题类似：
给定一个字符串，使用下面的方法给它编码：将字符串中的k个相同的字符写成『kX』，X是字符串中的字符
样例：
	2
	ABC
	ABBCCC
	1111111111
输出：
	1A1B1C
	1A2B3C
	101
*/

void encode_string2() {
	int lines;
	cin >> lines;	// 输入行数
	string result;
	while (lines--) {
		string s;	// 输入字符串
		cin >> s;
		int count = 0;
		char last = s[0];	// 注意这里的逻辑，首个字符输入后，count将自增，但不会输出
		for (auto c : s) {
			if (c != last) {
				result += to_string(count) + last;	// 仅这里与上一题不一致
				last = c;
				count = 1;
			} else {
				count++;
			}
		}
		// 到达结尾后，把剩余字符输出
		result += to_string(count) + last;
		result += '\n';
	}
	cout << result;
}


int main(int argc, char const *argv[])
{
	string s = "  To be or not to be beautiful";
	cout << reverse_word(s) << endl;
	string X = "ABCBDAB";
	string Y = "BDCABA";
	cout << lcs_recursive(X, Y, X.size(), Y.size()) << endl;
	return 0;
}


