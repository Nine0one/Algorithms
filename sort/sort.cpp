#include <iostream>
#include <vector>

using namespace std;

/*
快速排序
*/
int partition(vector<int> &a, int lo, int hi) {
  //partition
  int i = lo+1, j = hi;
  int pivot = a[lo];
  while(1){
    for(; a[i] < pivot; i++)  if (i == hi)  break;  // 通过哨兵方式可以把这两句边界检查去掉
    for(; a[j] > pivot; j--)  if (j == lo)  break;  // 冗余的边界检查
    if (i >= j)
      break;
    swap(a[i], a[j]);
  }
  swap(a[j], a[lo]);
  return j;
}

void quicksort(vector<int> &a, int lo, int hi) {
  if (lo >= hi)
    return;
  int j = partition(a, lo, hi);
  quicksort(a, lo, j-1);
  quicksort(a, j+1, hi);
}

void quicksort(vector<int> &a) {
  int max = 0;
  for (int i=1; i < a.size(); i++)
    if (a[i] > a[max])
      max = i;
  // 哨兵最大元素挪到尾部作为哨兵
  swap(a[a.size()-1], a[max]);
  quicksort(a, 0, a.size()-1);
}

// （不重要）三向切分的快速排序
void quicksort3way(vector<int> &a, int lo, int hi) {
  if (lo >= hi)
    return;
  int lt = lo, i = lo+1, gt = hi;
  int v = a[lo];
  while (i <= gt) {
    if (a[i] < v)
      swap(a[i++], a[lt++]);
    else if(a[i] > v)
      swap(a[i], a[gt--]);
    else
      i++;
  }
  quicksort3way(a, lo, lt-1);
  quicksort3way(a, gt+1, hi);
}



/*
快速排序应用
*/

// 第k大元素
int Kth(vector<int> a, int k) {
  k--;
  int lo = 0, hi = a.size()-1;
  while (lo <= hi) {
    int p = partition(a, lo, hi);
    if (k < p) {
      hi = p-1;
    } else if (k > p) {
      lo = p+1;
    } else {
      return a[p];
    }
  }
  return -1;
}

// 奇数在前，偶数在后，利用partition
void odd_front(vector<int> &a) {
  int n = a.size(), i = 0, j = n - 1;
  while(1) {
    for(; i < n && a[i] % 2 == 1; i++);
    for(; j >= 0 && a[j] % 2 == 0; j--);
    if (i >= j)
      break;
    swap(a[i], a[j]);
  }
}

/*
插入排序
将第i个元素插入到它前面的元素中，前i-1个元素有序
在未优化的情形下，插入排序能够比冒泡排序更早地结束有序情形下的排序
*/
void insertion_sort(vector<int> &a) {
  for (int i=1; i < a.size(); i++)
    for (int j=i; j > 0 && a[j] < a[j-1]; j--)
      swap(a[j], a[j-1]);
}

// 插入排序应用：奇数在前，偶数在后
void odd_even_sort_stable1(vector<int> &a) {
  int j;
  for (int i=1; i < a.size(); i++) {
    for(int j=i; j>0 && a[j]%2==1 && a[j-1]%2==0; j--)
      swap(a[j], a[j-1]);
  }
}

/*
冒泡排序
不断将最小元素挪至前部（或者反之）
*/
void bubble_sort(vector<int> &a) {
  for (int i=0; i < a.size(); i++)
    for (int j=a.size()-1; j>i; j--)
      if (a[j] < a[j-1])
        swap(a[j], a[j-1]);
}

// 冒泡排序应用：奇数在前，偶数在后
void odd_even_sort_stable(vector<int> &a) {
  int j;
  for (int i=0; i < a.size(); i++) {
    for(int j=a.size()-1; j>i; j--)
      if (a[j]%2==1 && a[j-1]%2==0)
        swap(a[j], a[j-1]);
  }
}

/*
希尔排序
插入排序的进化: 以h步长做插入排序，直到步长为1排序完成
*/
void shell_sort(vector<int> &a) {
  int h = 1;
  while (h < a.size()/3)
    h = h*3+1;
  // 步长h=[1, 4, 13, 40]
  for (; h >= 1; h /= 3)
    for (int i = h; i < a.size(); i++)
      for (int j = i; j >= h && a[j] < a[j-h]; j--)
        swap(a[j], a[j-h]);
}


int main(int argc, char const *argv[])
{
  vector<int> a = {'K', 'R', 'A', 'T', 'E', 'L', 'E', 'P', 'U', 'I', 'M', 'Q', 'C', 'X', 'O', 'S'};
  cout << (char) Kth(a, 5) << endl;
  // quicksort(a);
  // odd_even_sort_stable1(a);
  quicksort3way(a, 0, a.size()-1);
  for (int i:a) {
    cout << (char)i << " " ;
  }
  cout << endl;
  return 0;
}