#include <iostream>
#include <stack>
#include <queue>
#include <cmath>
#include <set>
using namespace std;


/*
数字相关：完全平方数、公约数、公倍数、回文数、素数
*/

// 素数检测
bool is_prime(int n) {
	if (n <= 1)
		return false;
	// 注：n=2时不会进入循环
	for (int i = 2; i <= sqrt(n); ++i)
		if (n % i == 0)
			return false;
	return true;
}

// 十进制数字反转（空间复杂度低）
int reverse_number(int n) {
	int r = 0;
	while(n != 0) {
		r = r * 10 + n % 10;
		n /= 10;
	}
	return r;
}

// 回文数检测（反转后比较）
bool is_palindrome(int n) {
	if (n < 0)
		return false;
	return reverse_number(n) == n;
}

// 回文数检测（没有空间限制条件下可以转成字符串更容易实现）
bool is_palindrome_simple(int n) {
	if (n < 0)
		return false;
	string s = to_string(n);
	for (int i = 0; i < (s.size() / 2); i++)
		if (s[i] != s[s.size()-i-1])
			return false;
	return true;
}

// 指定区间内的全部回文素数
void palindrome_prime(int max=100000) {
	for (int i = 0; i < max; ++i) {
		if (is_palindrome(i) && is_prime(i))
			cout << i << endl;
	}
}

/*
完全数判别
完全数，又称完美数或完备数，是一些特殊的自然数：它所有的真因子（即除了自身以外的约数）的和，恰好等于它本身。
例如：第一个完全数是6，它有约数1、2、3、6，除去它本身6外，其余3个数相加，1+2+3＝6，恰好等于本身。
	 第二个完全数是28，它有约数1、2、4、7、14、28，除去它本身28外，其余5个数相加，1+2+4+7+14＝28，也恰好等于本身。
	 后面的数是496、8128。
*/

bool is_perfect(int x) {
	int s = 0;	//因子和
	for (int i = 1; i < x; i++)
		if (x % i == 0)
			s += i;
	return s == x;
}

// 列出全部完全数
void perfect_numbers(int max=10000) {
	cout << max << ": " ;
	for (int i = 1; i <= max; i++)
		if (is_perfect(i))
			cout << i << " ";
	cout << endl;
}


/*
数根
   是将一正整数的各个位数相加(即横向相加)，若加完后的值大于等于10的话，则继续将各位数进行横向相加直到其值小于十为止;
或是，将一数字重复做数字和，直到其值小于十为止，则所得的值为该数的数根。
例如54817的数根为7，因为5+4+8+1+7=25，25大于10则再加一次，2+5=7，7小于十，则7为54817的数根。
*/
// 数字根
int digital_root(int x) {
	while (1){
		int r = 0;
		for (; x != 0; x /= 10) {
			r += x % 10;
		}
		if (r < 10)
			return r;
		else
			x = r;
	}
}

// 数字根（数学公式法）
// https://en.wikipedia.org/wiki/Digital_root#Congruence_formula
int digital_root_simple(int x) {
	return 1 + (x-1)%9;
}

// 素数对（将偶数分解成为两个素数，且要求两个素数最相近）
void prime_pair(int even_number) {
	for (int i = even_number/2; i > 0; i--) {
		if (is_prime(i) && is_prime(even_number-i)) {
			cout << i <<  " " << even_number-i << endl;
			break;
		}
	}
}
/*
 半素数
 数学中，两个素数的乘积所得的自然数我们称之为半素数（也叫双素数，二次殆素数）。开始的几个半素数是4, 6, 9, 10, 14, 15, 21, 22, 25, 26
*/
bool is_semiprime_simple(int n) {
	for (int i = n/2; i >= 2; i--)
		if (n%i == 0 && is_prime(i) && is_prime(n/i))
			return true;
	return false;
}


// 构建半素数表
set<int> semiprime_set;
void get_semiprimes(int max=100) {
	vector<int> primes(max);		// 构建素数表
	for (int i = 0; i <= max; ++i)
		if (is_prime(i))
			primes.push_back(i);
	for (auto i : primes)
		for (auto j : primes)
			semiprime_set.insert(i * j);
}


bool is_semiprime(int x) {
	if (semiprime_set.find(x) != semiprime_set.end())
		return true;
	else
		return false;
}

/*
求指定范围内所有数字的约数和
(亲密数也用到了约数和)
*/
vector<int> factors_sum(int max=5000) {
	vector<int> v(max, 1);		// max大小的数组，初始值1, v[i]即为i的约数和
	v[1] = 0;					// 1的约数和为0
	for (int i = 2; i < max; i++) {
		v[i*i] += i;
		for (int j = i+1; j <= max/i; j++)
			v[i*j] += i + j;	// i, j 均为i*j的约数
	}
	return v;
}

/*
进制转换
将x转为r进制（存为字符串），r<=16
*/

string number_encoding(int x, int r) {
	string s;
	char elements[] = "0123456789ABCDEF";
	for (int i=abs(x); i > 0; i /= r)
		s = elements[i%r] + s;
	if (x < 0)
		s = '-' + s;
	return s;
}

/*
完全平方数
*/

// 完全平方数判别: 暴力
bool is_square_brute(int x) {
	int i = 0;
	for (; i*i < x; i++);
	return i*i == x;
}

// 完全平方数判别: 二分
bool is_square_binary(int x) {
	int lo = 0, hi = x;
	while (lo <= hi) {
		int mid = (lo + hi) / 2;
		if (x < mid * mid)
			hi = mid-1;
		else if (x > mid * mid)
			lo = mid+1;
		else
			return true;
	}
	return false;
}

// 完全平方数判别: 数学，所有完全平方数都可以用奇数列相加得到
bool is_square(int x) {
	for (int i = 1; x > 0; i+=2) {
		x -= i;
	}
	return x == 0;
}


// 特殊的完全平方数：100~999中的完全平方数，且有两位数字相同
void special_square_numbers() {
	for (int i = 100; i < 1000; ++i)
	{
		if (!is_square(i))
			continue;
		// 十进制按位拆分多种方法，转换成字符串比较好写
		string s = to_string(i);
		char n1 = s[0], n2 = s[1], n3 = s[2];
		// int n1 = i / 100, n2 = (i % 100) / 10, n3 = (i % 100) % 10;
		// cout << n1 << n2 << n3 << endl;
		if (n1 == n2 || n1 == n3 || n2 == n3)
			cout << i << endl;
	}
}

// 如果整数A的全部因子（包括1，不包括A本身）之和等于B；
// 且整数B的全部因子（包括1，不包括B本身）之和等于A，则将整数A和B称为亲密数。
// ①求x的因子和y ②判断y的因子和z是否等于x
void amicable_numbers(unsigned long max=10000) {
	unsigned long x, y, z, i;
	for (x = 0; x <= max; ++x) {
		// ①
		for (i = 1, y = 0; i <= x/2; i++)
			if (x % i == 0)
				y += i;
		// ②
		for (i = 1, z = 0; i <= y/2; i++)
			if (y % i == 0)
				z += i;
		if (z == x && x < y)
			cout << x << ", " << y << endl;
	}
}


// 最大公约数
// 辗转相除法
/*有两整数a和b：
① a%b得余数c
② 若c=0，则b即为两数的最大公约数
③ 若c≠0，则a=b，b=c，再回去执行①
*/
int greatest_common_divisor(int a, int b) {
	int c = a % b;
	while (c != 0) {
		a = b;
		b = c;
		c = a % b;
	}
	return b;
}

// 最小公倍数：a*b/gcd(a, b)
int least_common_multiple(int a, int b) {
	return a*b / greatest_common_divisor(a, b);
}


int main(int argc, char const *argv[])
{
	get_semiprimes();
	cout << is_semiprime(2) << is_semiprime(25) << is_semiprime(123) << is_semiprime(26) << endl;
	cout << number_encoding(-4, 3);
	cout << is_semiprime_simple(24) << endl;
	prime_pair(24);
	return 0;
}


