// by: Zhiping

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <fstream>
#include <map>

#include <parser.h>

using namespace std;

int main(int argc, char **argv) {
  bool verbose;

  /* Check the input */
  if( argc < 2 ) {
  std::cerr << "Usage: " << argv[ 0 ] << " SOURCE [-v]" << std::endl;
    return 1;
  }
  int32_t opt;
  while ((opt = getopt(argc, argv, "v")) != -1) {
    switch (opt) {
      case 'v':
        verbose = true;
        break;

      default:
        std::cerr << "Usage: " << argv[ 0 ] << "[-v] SOURCE" << std::endl;
        return 1;
    }
  }

  L2::Program p = L2::L2_parse_func_file(argv[optind]);

  cout << argv[optind] << endl;


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
