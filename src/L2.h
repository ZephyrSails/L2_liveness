// by: Zhiping
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <vector>

namespace L2 {

  struct L2_item {
    std::string labelName;
  };

  struct Item {
    int type;         // defined by L1::ITEM_*TYPE*
    std::string name; // for register & label
    int value;        // for register (offset) & number
  };

  struct Instruction {
    int type;     // defined by L1::INS_*TYPE*
    std::vector<L2::Item *> items;
    // L1::Opperand op;
    std::string op;
    // L1::Cmp cmp;  // 3 & 4 has cmp
  };

  struct Function {
    std::string name;
    int64_t arguments;
    int64_t locals;
    std::vector<L2::Instruction *> instructions;
  };

  struct Program {
    std::string entryPointLabel;
    std::vector<L2::Function *> functions;
  };
}
