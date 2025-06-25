/**
 * @file examples/hello.c
 * @brief Simple C program to test the Orion C Compiler
 */

int main() {
  int x = 42;
  int y = 7;
  int sum = x + y;
  
  if (sum > 40) {
    sum = sum * 2;
  }
  
  int i = 0;
  while (i < 5) {
    i = i + 1;
  }
  
  return sum;
}

// int add(int a, int b) {
//   return a + b;
// }
// 
// int simple_function() {
//   int result = add(10, 20);
//   return result;
// }