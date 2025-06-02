#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <iostream>
using namespace std;

#define re register
#define _for(i, a, b) for(register int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(register int (i) = (a); (i) <= (b); ++(i))
#define ll long long
#define swap(a, b) (a)^=(b)^=(a)^=(b)
#define mmin(a, b) (((b) < (a)) ? (b) : (a))
#define mmax(a, b) (((b) > (a)) ? (b) : (a))
#define maxn 50005
#define maxm 50005
#define size_t int


int readnt(){
	register int x = 0, sign = 1;
	register char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

template <typename T>
void print(T arr[], size_t beg, size_t end){
	cout << arr[beg];
	for(++beg; beg < end; ++beg){
		cout << " " << arr[beg];
	}
	cout << endl;
}

template <typename T>
void insertion_sort(T arr[], re size_t beg, re size_t end){
	re size_t i, j;
	for(i = beg + 1; i < end; ++i)
		for(j = i; (j != beg) && (arr[j] < arr[j-1]); --j)
			swap(arr[j], arr[j-1]);
}

size_t cutoff = 10;
template <typename T>
void quick_sort(T arr[], size_t beg, size_t end){
	if (end - beg > cutoff){
		insertion_sort(arr, beg, end);
		return;
	}
	--end;
	int mid = (beg + end)>>1, i, j;
	
	if (arr[mid] < arr[beg])
		swap(arr[mid], arr[beg]);
	if (arr[end] < arr[mid])
		swap(arr[end], arr[mid]);
	if (arr[mid] < arr[beg])
		swap(arr[mid], arr[beg]);
	
	T privot = arr[mid];
	swap(arr[mid], arr[end-1]);
	
	i = beg; j = end-1;
	while(arr[++i] < privot) {}
	while(arr[--j] > privot) {}
	while(i < j){
		swap(arr[i], arr[j]);
		while(arr[++i] < privot) {}
		while(arr[--j] > privot) {}
	}
	swap(arr[i], arr[end-1]);
	quick_sort(arr, beg, i);
	quick_sort(arr, i+1, end+1);
}

int temp[maxn] = {};
template <typename T>
void merge_sort(T arr[], size_t beg, size_t end){
	if (end < beg + 2) return;
	int mid = (end + beg)>>1, i, j, k;
	merge_sort(arr, beg, mid);
	merge_sort(arr, mid, end);
	i = beg, j = mid, k = beg;
	while(i < mid && j < end)
		if (arr[j] < arr[i])
			temp[k] = arr[j], ++k, ++j;
		else
			temp[k] = arr[i], ++k, ++i;
	
	if (i < mid)
		while(i < mid)
			temp[k] = arr[i], ++k, ++i;	
	else
		while(j < end)
			temp[k] = arr[j], ++k, ++j;
	k = beg;
	while(k < end)
		arr[k] = temp[k], ++k;
}

template <typename T>
void shell_sort(T arr[], re size_t beg, re size_t end){
	re int h = 1, i, j;
	while (3*h + 1 < (end-beg)/3)
		h = 3*h + 1;
	
	while(h){
		for (i = beg+h; i < end; i += h)
			for(j = i; (j>=beg+h) && (arr[j]<arr[j-h]); j -= h)
				swap(arr[j], arr[j-h]);
		h /= 3;	
	}
}

template <typename T>
void counting_sort(T arr[], size_t beg, size_t end, T inf, T ninf){
	re int i = beg, j, k;
	T min = inf, max = ninf;
	while (i < end){
		if (arr[i] < min) min = arr[i];
		if (arr[i] > max) max = arr[i];
		++i;
	}
	
	size_t n = max-min+1, *counter = new T[n];
	for(i = 0; i < n; ++i) counter[i] = 0;
	
	i = beg;
	while(i < end)
		++counter[arr[i]-min], ++i;
	
	k = 0;
	for(i = 0; i < n; ++i)
		for(j = 0; j < counter[i]; ++j)
			arr[k] = i+min, ++k;

	delete[] counter;
}

template <typename T>
void percolate_up(T heap[], re size_t i){
	T val = heap[i];
	for(; (i>>1) && val > heap[i>>1]; i = i>>1)
		heap[i] = heap[i>>1];
	heap[i] = val;
}

template <typename T>
void percolate_down(T heap[], re size_t i, re size_t n){
	re size_t ch;
	T val = heap[i];
	for(; (i<<1) <= n; i = ch){
		ch = i<<1;
		if (ch < n && heap[ch|1] > heap[ch]) ++ch;
		
		if (heap[ch] > val)
			heap[i] = heap[ch];
		else
			break;
	}
	heap[i] = val;
}

template <typename T>
void heap_sort(T arr[], re size_t beg, re size_t end){
	T *heap = arr + beg - 1;
	re int i, n = end - beg;
	//build heap
	i = 2;
	while(i <= n) percolate_up(heap, i), ++i;
	//sort
	i = n;
	while (i > 2){
		swap(heap[i], heap[1]);
		percolate_down(heap, 1, --i);
	}
	swap(heap[1], heap[2]);
}

int ori_arr[maxn] = {}, arr[maxn] = {};

void copy(size_t beg, size_t end){
	while(beg < end)
		arr[beg] = ori_arr[beg], ++beg;	
}

int main(){
	freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	
	re int n = readnt();
	_for(i, 0, n)
		ori_arr[i] = readnt();
	copy(0, n);
	cout << "ori:\n";
	print(arr, 0, n);
	
	cout << "ins:\n"; 
	insertion_sort(arr, 0, n);
	print(arr, 0, n);
	copy(0, n);
	
	cout << "qui:\n";
	quick_sort(arr, 0, n);
	print(arr, 0, n);
	copy(0, n);
	
	cout << "mer:\n";
	merge_sort(arr, 0, n);
	print(arr, 0, n);
	copy(0, n);
	
	cout << "she:\n";
	shell_sort(arr, 0, n);
	print(arr, 0, n);
	copy(0, n);
	
	cout << "buc:\n";
	int inf = 0x7fffffff, ninf = 0x80000000;
	counting_sort(arr, 0, n, inf, ninf);
	print(arr, 0, n);
	copy(0, n);
	
	cout << "hea:\n";
	heap_sort(arr, 0, n);
	print(arr, 0, n);
	copy(0, n);
	
	return 0;
}

