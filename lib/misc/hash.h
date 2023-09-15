// decl
#ifndef TEMPLATE_CPP_HASH
#define TEMPLATE_CPP_HASH 1
#ifndef NO_TEMPLATE_IMPORT
#include <cstdint>
#include <chrono>
#endif
// impl
namespace misc {
using std::uint64_t;
template <typename T>
struct safe_hash {
	static uint64_t splitmix64(uint64_t x) {
		x ^= x << 13;
		x ^= x >> 7;
		x ^= x << 17;
		return x; 
	}
	size_t operator () (T x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count(); // 时间戳
		return splitmix64(x + FIXED_RANDOM);
	}
};
}
using misc::safe_hash;
#endif