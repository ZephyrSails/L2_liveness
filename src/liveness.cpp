// by: Zhiping

#include <iostream>

#include <parser.h>

using namespace std;

int main(int argc, char **argv) {

  cout << "(\n";
  cout << "(in\n";
  cout << "(r12 r13 r14 r15 rax rbp rbx)\n";
  cout << "(myVar1 r12 r13 r14 r15 rax rbp rbx)\n";
  cout << "(myVar1 myVar2 r12 r13 r14 r15 rax rbp rbx)\n";
  cout << "(r12 r13 r14 r15 rax rbp rbx)\n";
  cout << ")\n";
  cout << "\n";
  cout << "(out\n";
  cout << "(myVar1 r12 r13 r14 r15 rax rbp rbx)\n";
  cout << "(myVar1 myVar2 r12 r13 r14 r15 rax rbp rbx)\n";
  cout << "(r12 r13 r14 r15 rax rbp rbx)\n";
  cout << "()\n";
  cout << ")\n";
  cout << "\n";
  cout << ")\n";
  return 0;
}
