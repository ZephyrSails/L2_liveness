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

// std::set <std::string> callee_save_regs = ;
std::set<std::string> callee_save_regs = {"r12", "r13", "r14", "r15", "rbp", "rbx"};
std::set<std::string> caller_save_regs = {"r10", "r11", "r8", "r9", "rax", "rcx", "rdi", "rdx", "rsi"};
// std::set<std::string> args_regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
std::vector<std::string> args_regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

// utility
void insert_item_to_set(std::set<std::string> * s, L2::Item * i) {
  if (i->type == L2::ITEM::REGISTER || i->type == L2::ITEM::VAR) {
    if (i->name != "rsp" && i->name != "print" && i->name != "allocate" && i->name != "array-error") {
      s->insert(i->name);
    }
  }
}

void union_set(std::set<std::string> * s, std::set<std::string> * t) {
  s->insert(t->begin(), t->end());
}

void union_set(std::set<std::string> * s, std::vector<std::string> * t, int n) {
  for (int i = 0; i < n; i++) {
    s->insert(t->at(i));
  }
}

std::set<std::string> minus_set(std::set<std::string> * s, std::set<std::string> * t) {
  std::set<std::string> result;
  std::set_difference(s->begin(), s->end(), t->begin(), t->end(), std::inserter(result, result.end()));
  return result;
}

void gen_gen_kill(std::set<std::string> * GEN, std::set<std::string> * KILL, L2::Instruction * i) {
  switch (i->type) {
    case L2::INS::RETURN:
            GEN->insert(callee_save_regs.begin(), callee_save_regs.end());
            GEN->insert("rax");
            break;
    // case L2::INS::LABEL_INS:
    //         break;
    case L2::INS::MEM_START:
            insert_item_to_set(KILL, i->items.at(0));
            if (i->op != "<-") {
              insert_item_to_set(GEN, i->items.at(0));
            }
            insert_item_to_set(GEN, i->items.at(1));
            break;
    case L2::INS::W_START:
            insert_item_to_set(KILL, i->items.at(0));
            if (i->op != "<-") {
              insert_item_to_set(GEN, i->items.at(0));
            }
            insert_item_to_set(GEN, i->items.at(1));
            break;
    case L2::INS::CALL:
            union_set(GEN, &args_regs, i->items.at(0)->value);
            insert_item_to_set(GEN, i->items.at(0));
            union_set(KILL, &caller_save_regs);
            KILL->insert("rax");
            break;
    // case L2::INS::GOTO:
    //         break;
    case L2::INS::INC_DEC:
            insert_item_to_set(KILL, i->items.at(0));
            insert_item_to_set(GEN, i->items.at(0));
            break;
    case L2::INS::CISC:
            insert_item_to_set(KILL, i->items.at(0));
            insert_item_to_set(GEN, i->items.at(1));
            insert_item_to_set(GEN, i->items.at(2));
            break;
    case L2::INS::CMP:
            insert_item_to_set(KILL, i->items.at(0));
            insert_item_to_set(GEN, i->items.at(1));
            insert_item_to_set(GEN, i->items.at(2));
            break;
    case L2::INS::CJUMP:
            insert_item_to_set(KILL, i->items.at(0));
            insert_item_to_set(GEN, i->items.at(1));
            insert_item_to_set(GEN, i->items.at(2));
            break;
    case L2::INS::STACK:
            insert_item_to_set(KILL, i->items.at(0));
            // KILL->insert("rsp");
            break;
    default:
            break;
  }

  // cout << "\nGEN: ";
  // for (auto reg : *GEN)
  // {
  //     cout << reg << " ";
  // }
  // cout << " ///// KILL: ";
  // for (auto reg : *KILL)
  // {
  //     cout << reg << " ";
  // }
}

void liveness_analyze(L2::Function *func) {
  int n = func->instructions.size();

  std::set<std::string> GEN[n];
  std::set<std::string> KILL[n];

  for (int k = 0; k < n; k++) {
    L2::Instruction *i = func->instructions.at(k);

    gen_gen_kill(&GEN[k], &KILL[k], i);
    // We need to build GEN and KILL here
  }

  std::set <std::string> IN[n];
  std::set <std::string> OUT[n];
  int converge_count;
  while (converge_count != n) {
    converge_count = 0;

    for (int k = 0; k < n; k++) {
      std::set <std::string> newIn;
      std::set <std::string> newOut;

      // IN[i] = GEN[i] U (OUT[i] - KILL[i])
      union_set(&newIn, &GEN[k]);
      std::set <std::string> diff = minus_set(&OUT[k], &KILL[k]);
      union_set(&newIn, &diff);

      // OUT[i] = U (s a successor of i) IN[s]
      if (k != n-1) {
        // cjump & goto

        // L2::INS::GOTO
        // L2::INS::CJUMP

        // For normal situation
        union_set(&newOut, &IN[k+1]);
      }


      if (IN[k] == newIn && OUT[k] == newOut) {
        converge_count++;
      } else {
        IN[k] = newIn;
        OUT[k] = newOut;
      }
    }
  }
  // print in & out
  cout << "(\n(in\n";
  for (int k = 0; k < n; k++) {
    cout << "(";
    for (auto reg : IN[k]) {
        cout << reg << " ";
    }
    cout << ")\n";
  }
  cout << ")\n\n(out\n";
  for (int k = 0; k < n; k++) {
    cout << "(";
    for (auto reg : OUT[k]) {
        cout << reg << " ";
    }
    cout << ")\n";
  }

  cout << ")\n\n)";

}

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

  for (auto f : p.functions) {
    // cout << "\n\n_" << f->name << ":";

    liveness_analyze(f);
    // if (f->locals > 0) {
    //   outputFile << "\n\tsubq $" << std::to_string(f->locals * 8) << ", %rsp";
    // }
  }

  // cout << "\n" << argv[optind] << endl;


  // cout << "(\n";
  // cout << "(in\n";
  // cout << "(r12 r13 r14 r15 rax rbp rbx)\n";
  // cout << "(myVar1 r12 r13 r14 r15 rax rbp rbx)\n";
  // cout << "(myVar1 myVar2 r12 r13 r14 r15 rax rbp rbx)\n";
  // cout << "(r12 r13 r14 r15 rax rbp rbx)\n";
  // cout << ")\n";
  // cout << "\n";
  // cout << "(out\n";
  // cout << "(myVar1 r12 r13 r14 r15 rax rbp rbx)\n";
  // cout << "(myVar1 myVar2 r12 r13 r14 r15 rax rbp rbx)\n";
  // cout << "(r12 r13 r14 r15 rax rbp rbx)\n";
  // cout << "()\n";
  // cout << ")\n";
  // cout << "\n";
  // cout << ")\n";
  return 0;
}
