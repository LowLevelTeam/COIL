// Test program for demonstrating Orion++ binary format
// This program will be compiled to both binary (.orionpp) and human (.hopp) formats

int add(int a, int b) {
  return a + b;
}

int multiply(int x, int y) {
  int result = 0;
  int i = 0;
  
  while (i < y) {
    result = result + x;
    i = i + 1;
  }
  
  return result;
}

int factorial(int n) {
  if (n <= 1) {
    return 1;
  } else {
    int temp = n - 1;
    int fact = factorial(temp);
    return multiply(n, fact);
  }
}

int main() {
  int a = 5;
  int b = 3;
  
  int sum = add(a, b);
  int product = multiply(a, b);
  int fact5 = factorial(a);
  
  if (sum > 0) {
    return 1;
  } else {
    return 0;
  }
}