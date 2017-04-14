#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/contrib/raw_string.hh>

using namespace pegtl;
using namespace std;

namespace L2 {
  // meta element
  struct comment:
    pegtl::disable<
      pegtl::one< ';' >,
      pegtl::until< pegtl::eolf >
    > {};

  struct seps:
    pegtl::star<
      pegtl::sor<
        pegtl::ascii::space,
        comment
      >
    > {};

  struct aop:
    pegtl::sor<
      pegtl::string< '+', '=' >,
      pegtl::string< '-', '=' >,
      pegtl::string< '*', '=' >,
      pegtl::string< '&', '=' >
    > {};

  struct sop:
    pegtl::sor<
      pegtl::string< '<', '<', '=' >,
      pegtl::string< '>', '>', '=' >
    > {};

  struct cmp:
    pegtl::sor<
      pegtl::string< '<', '=' >,
      pegtl::string< '<'>,
      pegtl::string< '=' >
    > {};

  struct E:
    pegtl::sor<
      pegtl::one< '0' >,
      pegtl::one< '2' >,
      pegtl::one< '4' >,
      pegtl::one< '8' >
    > {};

  struct runtime_system_func:
    pegtl::sor<
    pegtl::string< 'p', 'r', 'i', 'n', 't' >,
    pegtl::string< 'a', 'l', 'l', 'o', 'c', 'a', 't', 'e' >,
    pegtl::string< 'a', 'r', 'r', 'a', 'y', '-', 'e', 'r', 'r', 'o', 'r' >
  > {};

  struct mem:
    pegtl::string < 'm', 'e', 'm' > {};

  struct inc_dec:
    pegtl::sor<
      pegtl::string < '+', '+' >,
      pegtl::string < '-', '-' >
    > {};

  struct call:
    pegtl::string < 'c', 'a', 'l', 'l' > {};

  struct left_arrow:
    pegtl::string< '<', '-' > {};

  struct plus_minus_op:
    pegtl::sor<
      pegtl::string< '+', '='>,
      pegtl::string< '-', '='>
    > {};

  // basic elements, definition.

  struct L2_var:
    pegtl::seq<
      pegtl::plus<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >
        >
      >,
      pegtl::star<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >,
          pegtl::digit
        >
      >
    > {};

  struct L2_label:
    pegtl::seq<
      pegtl::one<':'>,
      L2_var
    > {};

  struct L2_N:
    pegtl::seq<
      pegtl::opt<
        pegtl::sor<
          pegtl::one< '-' >,
          pegtl::one< '+' >
        >
      >,
      pegtl::plus<
        pegtl::digit
      >
    > {};

  struct L2_sx:
    pegtl::sor<
      pegtl::string< 'r', 'c', 'x' >,
      L2_var
    > {};

  struct L2_a:
    pegtl::sor<
      pegtl::string < 'r', 'd', 'i' >,
      pegtl::string < 'r', 's', 'i' >,
      pegtl::string < 'r', 'd', 'x' >,
      L2_sx,
      pegtl::string < 'r', '8' >,
      pegtl::string < 'r', '9' >
    > {};

  struct L2_w:
    pegtl::sor<
      L2_a,
      pegtl::string < 'r', 'a', 'x' >,
      pegtl::string < 'r', 'b', 'x' >,
      pegtl::string < 'r', 'b', 'p' >,
      pegtl::string < 'r', '1', '0' >,
      pegtl::string < 'r', '1', '1' >,
      pegtl::string < 'r', '1', '2' >,
      pegtl::string < 'r', '1', '3' >,
      pegtl::string < 'r', '1', '4' >,
      pegtl::string < 'r', '1', '5' >
    > {};

  struct L2_x:
    pegtl::sor<
      L2_w,
      pegtl::string < 'r', 's', 'p' >
    > {};

  struct L2_s:
    pegtl::sor<
      L2_x,
      L2_N,
      L2_label
    > {};

  struct L2_M:
    L2_N {};

  struct L2_t:
    pegtl::sor<
      L2_x,
      L2_N
    > {};

  struct L2_u:
    pegtl::sor<
      L2_w,
      L2_label
    > {};

  // special elements

  struct function_name:
    L2_label {};

  struct argument_number:
    L2_N {};

  struct local_number:
    L2_N {};

  struct prog_label:
    L2_label {};

  // element decleration

  struct var:
    L2_var {};
  struct label:
    L2_label {};
  struct N:
    L2_N {};
  struct sx:
    L2_sx {};
  struct a:
    L2_a {};
  struct w:
    L2_w {};
  struct x:
    L2_x {};
  struct s:
    L2_s {};
  struct M:
    L2_M {};
  struct t:
    L2_t {};
  struct u:
    L2_u {};

  // instructure

  struct mem_x_M:
    pegtl::seq<
      pegtl::one< '(' >,
      seps,
      mem,
      seps,
      x,
      seps,
      M,
      seps,
      pegtl::one< ')' >
    > {};

  struct stack_arg:
    pegtl::seq<
      pegtl::one<'('>,
      seps,
      pegtl::string<'s', 't', 'a', 'c', 'k', '-', 'a', 'r', 'g'>,
      seps,
      M,
      seps,
      pegtl::one<')'>
    > {};

  struct ins_w_start:
    pegtl::seq<
      w,
      seps,
      pegtl::sor<
        pegtl::seq<
          left_arrow,
          seps,
          pegtl::sor<
            pegtl::seq<
              s, // potential bug
              pegtl::star< pegtl::seq< seps, cmp, seps, t > >
            >,
            mem_x_M,
            stack_arg
          >
        >, // potential bug
        pegtl::seq<
          aop,
          seps,
          pegtl::sor< t, mem_x_M >
        >,
        pegtl::seq< sop, seps, pegtl::sor< sx, N > >,
        inc_dec,
        pegtl::seq< pegtl::one< '@' >, seps, w, seps, w, seps, E >
      >
    > {};

  struct ins_mem_start:
    pegtl::seq<
      mem_x_M,
      seps,
      pegtl::sor<
        pegtl::seq< left_arrow, seps, s >,
        pegtl::seq< plus_minus_op, seps, t >
      >
    > {};

  struct ins_cjump:
    pegtl::seq<
      seps,
      pegtl::string< 'c', 'j', 'u', 'm', 'p' >,
      seps,
      t,
      seps,
      cmp,
      seps,
      t,
      seps,
      label,
      seps,
      label
    > {};

  struct ins_label:
    label {};

  struct ins_goto:
    pegtl::seq< pegtl::string < 'g', 'o', 't', 'o' >, seps, label > {};

  struct ins_return:
    pegtl::seq< pegtl::string < 'r', 'e', 't', 'u', 'r', 'n' > > {};

  struct ins_call_func:
    pegtl::seq<
      call,
      seps,
      pegtl::sor< u, runtime_system_func >,
      seps,
      N
    > {};

  struct L2_instruction:
    pegtl::sor<
      pegtl::seq<
        pegtl::one<'('>,
        pegtl::sor<
          ins_w_start,
          ins_mem_start,
          ins_cjump
        >,
        pegtl::one<')'>
      >,
      ins_label,
      pegtl::seq<
        pegtl::one<'('>,
        seps,
        pegtl::sor<
          ins_goto,
          ins_return,
          ins_call_func
        >,
        seps,
        pegtl::one<')'>
      >
    > {};

  struct L2_function_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      function_name,
      seps,
      argument_number,
      seps,
      local_number,
      seps,
      pegtl::star<
        L2_instruction,
        seps
      >,
      seps,
      pegtl::one< ')' >,
      seps
    > {};

  struct L2_functions_rule:
    pegtl::seq<
      seps,
      pegtl::plus< L2_function_rule >
    > {};

  struct entry_point_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      prog_label,
      seps,
      L2_functions_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > {};

  struct L2_grammer:
    pegtl::must<entry_point_rule
  > {};
}
