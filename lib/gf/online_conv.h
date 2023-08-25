// decl
#ifndef TEMPLATE_CPP_ONLINE_CONV
#define TEMPLATE_CPP_ONLINE_CONV 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/poly.h"
namespace gf {

template <const unsigned int Mod>
struct OnlineConv {
	using Ele = modular::Z<Mod>;
  using Func = gf::Poly<Mod>;
  static const int BASE_CASE_SIZE = 32;
	int n, unit;
  vector<Ele> fg_, f_, g_;
  vector<Func> f_dft, g_dft;

	OnlineConv() :n(0), fg_(8), f_(0), g_(0) {
		f_dft.clear();
		g_dft.clear();
		unit = 2;
	}
	Ele get(int x) {
		if (x < 0 || x >= (int)fg_.size())
			return 0;
		return fg_[x];
	}
	// return (fg)[n] - f[0]g[n] - f[n]g[0]
	Ele current() { return get(n); }
	inline void extend() {
		if (++ n <= 3)
			return;
		if ((n & (n - 1)) == 0) {
			unit = n;
			fg_.resize(n << 2);
			auto &&l = f_dft.emplace_back(f_.cbegin() + (n >> 1), f_.cend());
			auto &&r = g_dft.emplace_back(g_.cbegin() + (n >> 1), g_.cend());
			l.resize(n), r.resize(n);
			int k = get_log(n);
			l.dft(k), r.dft(k);

			Func tmp(n);
			for (int i = 0; i < n; i ++)
				tmp[i] = l[i] * r[i];
			tmp.idft(k);
			for (int i = 0; i < n - 1; i ++)
				fg_[i + n] += tmp[i];
		}
	}
	Ele set(Ele fn, Ele gn) {
		f_.push_back(fn);
		g_.push_back(gn);
		if (n == 0)
			fg_[0] = fn * gn;
		else if (n == 1)
			fg_[1] = f_[0] * gn + fn * g_[0],
			fg_[2] = fn * gn;
		else if (n == 2) 
			fg_[2] += fn * g_[0] + gn * f_[0],
			fg_[3] = fn * g_[1] + gn * f_[1];
		else {
			fg_[n] += f_[0] * gn + fn * g_[0];
			fg_[n + 1] += f_[1] * gn + fn * g_[1];
			for (int p = n + 1 - unit, pos = 1, len = 2;
				(p & 1) == 0 && len != unit; p >>= 1, ++ pos, len <<= 1) {
				if (len <= BASE_CASE_SIZE) {
					for (int i = 0, delta = n + 1 - len; i != len; i ++)
						for (int j = 0; j != len; j ++)
							fg_[n + 1 + i + j] += f_[i + delta] * g_[j + len] + f_[i + len] * g_[j + delta];
				}
				else {
					int nlen = len << 1;
					Func tf(f_.cend() - len, f_.cend());
					Func tg(g_.cend() - len, g_.cend());
					tf.resize(nlen), tg.resize(nlen);
					tf.dft(pos + 1), tg.dft(pos + 1);
					for (int i = 0; i != nlen; i ++)
						tf[i] = tg[i] * f_dft[pos - 1][i] + tf[i] * g_dft[pos - 1][i];
					tf.idft(pos + 1);
					for (int i = 0; i != nlen - 1; i ++)
						fg_[n + 1 + i] += tf[i];
				}
			}
		}
		extend();
		return fg_[n];
	}
	vector<Ele> f() { return f_; }
	vector<Ele> g() { return g_; }
	vector<Ele> fg() { return fg_; }
};

template <const unsigned int Mod>
struct OnlineConvSingle {
	using Ele = modular::Z<Mod>;
  using Func = gf::Poly<Mod>;
  static const int BASE_CASE_SIZE = 32;
	int n, unit;
  vector<Ele> fg_, f_;
  vector<Func> f_dft;

	OnlineConvSingle() :n(0), fg_(8), f_(0) {
		f_dft.clear();
		unit = 2;
	}
	Ele get(int x) {
		if (x < 0 || x >= (int)fg_.size())
			return 0;
		return fg_[x];
	}
	// return (fg)[n] - f[0]f[n] * 2
	Ele current() { return get(n); }
	inline void extend() {
		if (++ n <= 3)
			return;
		if ((n & (n - 1)) == 0) {
			unit = n;
			fg_.resize(n << 2);
			auto &&l = f_dft.emplace_back(f_.cbegin() + (n >> 1), f_.cend());
			int k = get_log(n);
			l.resize(n), l.dft(k);
			Func tmp(n);
			for (int i = 0; i < n; i ++)
				tmp[i] = l[i] * l[i];
			tmp.idft(k);
			for (int i = 0; i < n - 1; i ++)
				fg_[i + n] += tmp[i];
		}
	}
	Ele set(Ele fn) {
		f_.push_back(fn);
		if (n == 0)
			fg_[0] = fn * fn;
		else if (n == 1)
			fg_[1] = f_[0] * fn * 2,
			fg_[2] = fn * fn;
		else if (n == 2) 
			fg_[2] += fn * f_[0] * 2,
			fg_[3] = fn * f_[1] * 2;
		else {
			fg_[n] += f_[0] * fn * 2;
			fg_[n + 1] += f_[1] * fn * 2;
			for (int p = n + 1 - unit, pos = 1, len = 2;
				(p & 1) == 0 && len != unit; p >>= 1, ++ pos, len <<= 1) {
				if (len <= BASE_CASE_SIZE) {
					for (int i = 0, delta = n + 1 - len; i != len; i ++)
						for (int j = 0; j != len; j ++)
							fg_[n + 1 + i + j] += f_[i + delta] * f_[j + len] * 2;
				}
				else {
					int nlen = len << 1;
					Func tf(f_.cend() - len, f_.cend());
					tf.resize(nlen), tf.dft(pos + 1);
					for (int i = 0; i != nlen; i ++)
						tf[i] = tf[i] * f_dft[pos - 1][i] * 2;
					tf.idft(pos + 1);
					for (int i = 0; i != nlen - 1; i ++)
						fg_[n + 1 + i] += tf[i];
				}
			}
		}
		extend();
		return fg_[n];
	}
	vector<Ele> f() { return f_; }
	vector<Ele> fg() { return fg_; }
};

}
using OnlineConv = gf::OnlineConv<Md>;
#endif