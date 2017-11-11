#ifndef FIB_H_
#define FIB_H_

class Fib {
  int f, g;
public:
  Fib(int n) : f(1), g(0) { while (g < n) next(); }
  int get() { return g; }
  int next() { g += f; f = g - f; return g; }
  int prev() { f = g - f; g -= f; return g; }
}

#endif // FIB_H_
