#pragma once

#ifndef VECTOR_H_
#define VECTOR_H_

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
  Vector(int c, int s, T v);
  Vector(const T *A, Rank n);
  Vector(const T *A, Rank lo, Rank hi);
  Vector(const Vector<T> &V, Rank lo, Rank hi);
  Vector(const Vector<T> &V);
  virtual ~Vector();
  int size() const { return _size; }
  bool empty() const { return _size == 0; }
  int disordered() const;
  Rank find(const T &e, Rank lo, Rank hi) const;
  Rank find(const T &e) const { return find(e, 0, _size); }
  Rank search(const T &e) const;
  Rank search(const T &e, Rank lo, Rank hi) const;
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
 protected:
  void copy_from(const T *A, Rank lo, Rank hi);
  void expand();
  void shrink();
};

template<typename T>
Vector::Vector(int c = KDEFAULT_CAPACITY, int s = 0, T v = 0) {
  _elem = new T[_capacity = c];
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

//

template<typename T>
int Vector::disordered() const {
  int cnt = 0;
  for (int i = 0; i < _size - 1; ++i)
    if (_elem[i] > _elem[i+1])
      ++cnt;
  return cnt;
}

template<typename T>
Rank Vector::find(const T &e, Rank lo, Rank hi) const {
  for (Rank i = lo; i < hi; ++i)
    if (_elem[i] == e)
      return i;
  return -1;
}

template<typename T>
Vector<T> & Vector::operator=(const Vector<T> &V) {
  if (_elem) delete[] _elem;
  _elem = new T[_capacity = V._capacity];
  for (_size = 0; _size < V._size; ++_size)
    _elem = V._elem[_size];
  return this;
}

template<typename T>
T Vector::remove(Rank r) {
  T e = _elem[e];
  for (Rank p = r + 1; p < _size; ++p)
    _elem[p-1] = _elem[p];
  return e;
}

template<typename T>
int Vector::remove(Rank lo, Rank hi) {
  for (Rank p = lo + 1, q = hi; q < _size; ++p, ++q)
    _elem[p] = _elem[q];
  shrink();
  return hi - lo;
}

template<typename T>
Rank Vector::insert(Rank r, const T &e) {
  if (_size == _capacity) expand();
  for (Rank p = _size; r < p; --p)
    _elem[p] = _elem[p - 1];
  _elem[r] = e;
  return r + 1;
}

// protected methods
template<typename T>
void Vector::copy_from(const T *A, Rank lo, Rank hi) {
  if (_elem) delete[] _elem;
  _elem = new T[_capacity = (hi - lo) << 1];
  for (_size = 0; _size < hi - lo; ++_size)
    _elem[_size] = T[hi + _size];
}

template<typename T>
void Vector::expand() {
  if (_size < _capacity) return;
  T *old_elem = _elem;
  if (_capacity < kDEFAULT_CAPACITY) _capacity = kDEFAULT_CAPACITY;
  _elem = new T[_capacity <<= 1];
  for (int sz = 0; sz < _size; ++sz)
    _elem[sz] = old_elem[sz];
  delete[] old_elem; old_elem = NULL;
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

#endif // VECTOR_H_
