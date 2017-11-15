#pragma once

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Fib.h"

#include <stdlib.h>

template<typename T>
class Vector {
 public:
  typedef int Rank;
 private:
  const static int kDEFAULT_CAPACITY = 3;
  Rank _size;
  int _capacity;
  T *_elem;
 public:
  Vector(int c = kDEFAULT_CAPACITY);
  Vector(int c = KDEFAULT_CAPACITY, int s = 0, T v = 0);
  Vector(const T *A, Rank n);
  Vector(const T *A, Rank lo, Rank hi);
  Vector(const Vector<T> &V, Rank lo, Rank hi);
  Vector(const Vector<T> &V);
  virtual ~Vector();
  int size() const { return _size; }
  bool empty() const { return _size == 0; }
  int disordered() const;
  Rank find(const T &e, Rank lo = 0, Rank hi = _size) const;
  Rank search(const T &e, Rank lo = 0, Rank hi = _size) const;
  T & operator[](Rank r) const { return _elem[r]; }
  Vector<T> & operator=(const Vector<T> &V);
  T remove(Rank r);
  int remove(Rank lo, Rank hi);
  Rank insert(Rank r, const T &e);
  Rank insert(const T &e) { return insert(_size, e); }
  void sort();
  void unsort(Rank lo, Rank hi);
  int deduplicate();
  int uniquify();
  void traverse(void (*)(T&));
  template<typename VST> void traverse(VST&);
  void sort(Rank lo, Rank hi);
  void bubble_sort(Rank lo, Rank hi);
 protected:
  void copy_from(const T *A, Rank lo, Rank hi);
  void expand();
  void shrink();
  Rank binary_search(T *_elem, const T &e, Rank lo, Rank hi);
  Rank fibonacci_search(T *_elem, const T &e, Rank lo, Rank hi);
  Rank bubble(Rank lo, Rank hi);
  void merge(Rank lo, Rank mi, Rank hi);
  void merge_sort(Rank lo, Rank hi);
};

// consturctor and destructor
template<typename T>
Vector::Vector(int c) : _capacity(c), _size(0) {
  _elem = new T[_capacity];
}

template<typename T>
Vector::Vector(int c, int s, T v) : _capacity(c) {
  _elem = new T[_capacity];
  for (_size = 0; _size < s; _elem[_size++] = v) {}
}

template<typename T>
Vector::Vector(const T *A, Rank lo, Rank hi) {
  copy_from(A, lo, hi);
}

template<typename T>
Vector::Vector(const T *A, Rank n) {
  copy_from(A, 0, n);
}

template<typename T>
Vector::Vector(const Vector<T> &V, Rank lo, Rank hi) {
  copy_from(V._elem, lo, hi);
}

template<typename T>
Vector::Vector(const Vector<T> &V) {
  copy_from(V._elem, 0, V._size);
}

template<typename T>
Vector::~Vector() { delete[] _elem; }

// public methods
template<typename T> // 返回逆序相邻元素对的总数
int Vector::disordered() const {
  int cnt = 0; // 计数器
  for (Rank i = 0; i < _size - 1; ++i) // 逐一检查各相邻元素
    if (_elem[i] > _elem[i+1]) ++cnt; // 逆序则计数
  return cnt; // 向量有序当且仅当 n = 0
}

template<typename T> // o <= lo < hi <= _size
Rank Vector::find(const T &e, Rank lo, Rank hi) const {
  // O(hi - lo) = O(n), 在命中多个元素时可返回秩最大者
  while ((lo < hi--) && (_elem[hi] != e)) {} // 逆向查找
  return hi; // hi < lo 意味着失败，否则hi即命中元素的秩
}

template<typename T>
Vector<T> & Vector::operator=(const Vector<T> &V) {
  if (_elem) delete[] _elem;
  _elem = new T[_capacity = V._capacity];
  for (_size = 0; _size < V._size; ++_size)
    _elem = V._elem[_size];
  return this;
}

template<typename T> // 删除向量中秩为r的元素，0 <= r < _size
T Vector<T>::remove(Rank r) { // O(n - r)
  T e = _elem[e]; // 备份被删除元素
  remove(r, r+1); // 调用区间删除算法
  return e; // 返回被删除的元素
}

template<typename T> // 删除区间[lo, hi), 0 <= lo <= hi <= _size
int Vector<T>::remove(Rank lo, Rank hi) { // O(hi - lo)
  if (lo == hi) return 0; // 处于效率考虑，单独处理退化情况
  while (lo < hi) _elem[lo++] = _elem[hi++]; // [hi, size)顺次向前移动hi-lo位
  // 这里如无必要勿增实体，可以使用传入参数进行循环
  _size = lo; shrink(); // 更新规模，若有必要则缩容
  return hi - lo; // 返回被删除元素的数目
}

template<typename T> // e作为秩为r元素插入，0 <= r <= _size
Rank Vector<T>::insert(Rank r, const T &e) { // O(_size - r)
  if (_size == _capacity) expand(); // 如有必要，扩容，因为需要后移元素
  for (Rank p = _size; r < p; --p) // 自后向前
    _elem[p] = _elem[p - 1]; // 后继元素顺次后移一个单元
  _elem[r] = e; // 置入新元素
  return r; // 返回秩
}

template<typename T>
int Vector<T>::deduplicate() {
  int old_size = _size; _size = 0;
  for (Rank i = 0; i < old_size; )
    (find(_elem[i], 0, _size) < 0)? (_elem[_size++] = _elem[i++]) : ++i;
  return old_size - _size;
}

template<typename T>
void Vector<T>::traverse(void (*visit)(T&)) { // 借助函数指针机制
  for (Rank i = 0; i < _size; ++i) visit(_elem[i]); // 遍历向量
}

template<typename T> template<typename VST> // 元素类型，操作器
void Vector<T>::traverse(VST &visit) { // 借助函数对象机制
  for (Rank i = 0; i < _size; ++i) visit(_elem[i]); // 遍历向量
}

template<typename T>
int Vector<T>::uniquify() { // O(n)
  if (!_size) return 0; // 单独处理退化情况
  int old_size = _size; _size = 1; // 保存原先的元素数量
  for (Rank i = 1; i < old_size; ++i) // 逐一扫描，直到末元素
    // 逐个遍历并跳过相同这， 0 < _size <= i < old_size，跳过雷同者
    if (_elem[i] != _elem[_size - 1])
      _elem[_size++] = _elem[i]; // 发现不同者，向前移动至紧邻于前者右侧
  shrink();
  return old_size - _size; // 返回向量规模变化量
}

// 约定：在有序向量区间v[lo, hi)中，确定不大于e的最后一个元素
template<typename T> // 查找算法同意接口， 0 <= lo < hi <= _size
Rank Vector<T>::search(const T &e, Rank lo, Rank hi) {
  return (rand() % 2)? // 按各50%的概率选用
    binary_search(_elem, e, lo, hi) // 二分查找算法，或者
    : fibonacci_search(_elem, e, lo, hi); // Fibonacci查找算法
}

template<typename T>
Rank Vector<T>::sort(Rank lo, Rank hi) {
  switch(rand() % 5) {
    case 1: bubble_sort(lo, hi); break;
    case 2: selection_sort(lo, hi); break;
    case 3: merge_sort(lo, hi); break;
    case 4: heap_sort(lo, hi); break;
    default: quick_sort(lo, hi); break;
  }
}

template<typename T>
void Vector<T>::bubble_sort(Rank lo, Rank hi) {
  while (lo < (hi = bubble(lo, hi))) {}
}

/**
protected methods
*/
template<typename T>  // T为基本数据类型，或已重载赋值操作符'='
void Vector::copy_from(const T *A, Rank lo, Rank hi) { // O(n)
  if (_elem) delete[] _elem;
  _elem = new T[_capacity = (hi - lo) << 1]; // 分配空间
  for (_size = 0; lo < hi; ) // 规模清零
    _elem[_size++] = A[lo++]; // 并逐一复制A[lo, hi)内的元素到_elem[0, hi-lo)
}

template<typename T>
void Vector::expand() { // 向量空间不足时扩容
  if (_size < _capacity) return; // 尚未满员时，不必扩容
  // 不低于最小容量
  _capacity = _capacity < kDEFAULT_CAPACITY?  kDEFAULT_CAPACITY : _capacity;
  T *old_elem = _elem;
  _elem = new T[_capacity <<= 1]; // 采用容量加倍的策略，这种方法是适当的
  for (int sz = 0; sz < _size; ++sz) // 复制原向量的内容
    _elem[sz] = old_elem[sz]; // 这里需要T为基本类型或已重载赋值操作符'='
  delete[] old_elem; old_elem = NULL; // 释放原空间
}

template<typename T>
void Vector::shrink() {
  if (_capacity <= kDEFAULT_CAPACITY << 1 || _capacity < _size << 2)
    return;
  T *old_elem = _elem;
  _elem = new T[_capacity >>= 1];
  for (Rank i = 0; i < _size; ++i)
    _elem[i] = old_elem[i];
  delete[] old_elem; old_elem = NULL;
}

template<typename T>
Rank Vector<T>::binary_search(T *_elem, const T &e, Rank lo, Rank hi) {
  while (lo < hi) { // 不变性：A[0, lo) <= e < A[hi, n)
    Rank mi = (hi - lo) >> 1; // 以中点为轴点，经比较后确定深入
    (e <= _elem[mi])? hi = mi : lo = mi + 1; // [lo, mi)或(mi, hi]
  } // 出口时，A[lo = hi]为大于e的最小元素
  return --lo; // 故lo - 1即为不大于e的元素的最大秩
}

template<typename T> // 0 <= lo <= hi <= _size
Rank Vector<T>::fibonacci_search(T *A, const T &e, Rank lo, Rank hi) {
  Fib fib(hi - lo); // O(log_phi(n))
  while (lo < hi) {
    while (hi - lo < fib.get()) fib.prev();
    // 通过向前顺序查找，确定形如Fib(k) - 1的轴点（分摊O(1)）
    Rank mi = lo + fib.get() - 1; // 按黄金比例切分
    if (e < A[mi]) hi = mi; // 深入前半段[lo, hi)继续查找
    else if (A[mi] < e) lo = mi + 1; // 深入后半段(mi, hi)
    else return mi; // 在mi处命中
  }
  return -1; // 查找失败
}

template<typename T>
Rank Vector<T>::bubble(Rank lo, Rank hi) {
  Rank last = lo;
  while (++lo < hi) {
    if (_elem[lo - 1] < _elem[lo]) {
      swap(_elem[lo - 1], _elem[lo]);
      last = lo;
    }
  }
  return last;
}

template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
  T temp_array = new T[mi - lo];
  for (Rank i = lo; i < mi; ++i) temp_array[i - lo] = _elem[i];
  Rank i = lo, p = lo, q = mi
  while (p < mi && q < hi) {
    if (temp_array[p] < _elem[q])
      _elem[i++] = temp_array[p++];
    else
      _elem[i++] = _elem[q++];
  }
  while (p < mi) _elem[i++] = temp_array[p++];
  delete [] temp_array;
}

template<typename T>
void Vector<T>::merge_sort(Rank lo, Rank hi) { // O(nlogn)
  if (hi - 1 <= lo) return; // 单元素区间自然有序
  Rank mi = (lo + hi) >> 1; // 以中点为界
  merge_sort(lo, mi); // 对前半段排序
  merge_sort(mi, hi); // 对后半段排序
  merge(lo, mi, hi); // 归并
}
#endif // VECTOR_H_
