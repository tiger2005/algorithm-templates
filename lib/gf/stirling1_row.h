// decl
#ifndef TEMPLATE_CPP_STIRLING1_ROW
#define TEMPLATE_CPP_STIRLING1_ROW 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/inc_power.h"
namespace gf {
template <const unsigned int Mod = Md>
vector<modular::Z<Mod>> stirling1_row(int n) {
  return inc_power<Mod>(n).raw();
}
}
using gf::stirling1_row;
#endif