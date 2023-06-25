### `gf` - 多项式类

实现了固定模数的多项式，并给出了各种操作的优秀实现。除了基础操作的卡常外，还包括多点求值和快速插值的转置写法。

文件目录：
- `poly.h`：整个 `Poly` 的实现，依赖于 `Z`，`log2` 和二次剩余；
- `multipoint.h`：实现了多项式多点求值；
- `interpolate.h`：实现了多项式快速插值；
- `poly_trigon.h`：实现了多项式三角函数和多项式反三角函数。

待完成：
- 复数类 `FFT`；
- 下降幂多项式相关计算；
- 任意模数多项式计算方案。

感谢 hly1204 的悉心教导（？），以及以下博客的启发：
- https://oi-wiki.org/math/poly/intro/ OI Wiki 的全套介绍；
- https://loj.ac/d/3165 让我重新理解了 FFT；
- https://www.luogu.com.cn/blog/vocalise/multipoint-interpolation-vandermonde 教会我转置做法的思想；
- https://negiizhao.blog.uoj.ac/blog/4671 提供了非常优质的基础操作卡常小技巧；
- https://hly1204.blog.uoj.ac/blog/7319 exp 实现方案使用到的半在线卷积，具体可参考 `poly.h` 的末尾。

下面是对这个模板库的简单测试，引用了 IO 库和 Poly 类实现必要的库。若无特殊说明，统一禁用 **块读**，并使用 `C++20` 和 `O2` 编译开关。

|                            模板题                            |                     提交记录                     |  时间（最大 / 全部）   | 规模    |
| :-: | :-: | :-: | :-: |
| [P3803 【模板】多项式乘法（FFT）](https://www.luogu.com.cn/problem/P3803) |  [#](https://www.luogu.com.cn/record/113164712)  |     293ms / 754ms      | $10^6$  |
| [P4238 【模板】多项式乘法逆](https://www.luogu.com.cn/problem/P4238) | [#](https://www.luogu.com.cn/record/113165037) |      37ms / 262ms      | $10^5$  |
| [P4725 【模板】多项式对数函数（多项式 ln）](https://www.luogu.com.cn/problem/P4725) |  [#](https://www.luogu.com.cn/record/113165104)  |      53ms / 265ms      | $10^5$  |
| [P4726 【模板】多项式指数函数（多项式 exp）](https://www.luogu.com.cn/problem/P4726) |  [#](https://www.luogu.com.cn/record/113165166)  |      42ms / 204ms      | $10^5$  |
| [P5273 【模板】多项式幂函数（加强版）](https://www.luogu.com.cn/problem/P5273) |  [#](https://www.luogu.com.cn/record/113165716)  |      82ms / 669ms      | $10^5$  |
| [P5277 【模板】多项式开根（加强版）](https://www.luogu.com.cn/problem/P5277) |  [#](https://www.luogu.com.cn/record/113165818)  |      40ms / 356ms      | $10^5$  |
| [P4512 【模板】多项式除法](https://www.luogu.com.cn/problem/P4512) |  [#](https://www.luogu.com.cn/record/113166141)  |      62ms / 328ms      | $10^5$  |
| [P5050 【模板】多项式多点求值](https://www.luogu.com.cn/problem/P5050) |  [#](https://www.luogu.com.cn/record/113166346)  |     236ms / 2.30s      | $64000$ |
| [P5158 【模板】多项式快速插值](https://www.luogu.com.cn/problem/P5158) |  [#](https://www.luogu.com.cn/record/113166857)  |     746ms / 5.30s      | $10^5$  |
| [P5264 多项式三角函数](https://www.luogu.com.cn/problem/P5264) |  [#](https://www.luogu.com.cn/record/113167333)  |      70ms / 423ms      | $10^5$  |
| [P5265 多项式反三角函数](https://www.luogu.com.cn/problem/P5265) |  [#](https://www.luogu.com.cn/record/113167794)  |      94ms / 463ms      | $10^5$  |
|           [#150. 挑战多项式](https://loj.ac/p/150)           |          [#](https://loj.ac/s/1801919)           |     156ms / 2849ms     | $10^5$  |
| [#150. 挑战多项式](https://loj.ac/p/150)（`-Ofast`，启动块读） |          [#](https://loj.ac/s/1801921)           | **106ms** / **2065ms** | $10^5$  |
