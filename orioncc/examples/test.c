// Simple test program for Orion C Compiler
// This demonstrates the basic C features supported

int main() {
  int a = 10;
  int b = 20;
  
  if (a < b) {
    return 1;
  } else {
    return 0;
  }
}

int add(int x, int y) {
  return x + y;
}

int factorial(int n) {
  if (n <= 1) {
    return 1;
  } else {
    int temp = n - 1;
    int result = factorial(temp);
    return n * result;
  }
}
