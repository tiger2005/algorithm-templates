### `gf` - 多项式类

实现了固定模数的多项式，并给出了各种操作的优秀实现。除了基础操作的卡常外，还包括多点求值和快速插值的转置写法。

文件目录：
- `poly.h`：整个 `Poly` 的实现，依赖于 `Z`，`log2` 和二次剩余；
- `multipoint.h`：实现了多项式多点求值；
- `interpolate.h`：实现了多项式快速插值；
- `poly_trigon.h`：实现了多项式三角函数和多项式反三角函数；
- 还有很多其他的东西。

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

#### 2023/7/4：更新了其他操作

这次更新增加了对斯特林数行 / 列求解的支持、Chirp Z 变换的实现，以及函数在上升幂 / 下降幂域上的相关操作。同时，这次更新也对一些函数的操作进行了微调。

| 模板题 | 提交记录（最优解排名） | 时间（最大 / 全部） | 规模 |
| :-: | :-: | :-: | :-: |
| [P6800 【模板】Chirp Z-Transform](https://www.luogu.com.cn/problem/P6800) | [#](https://www.luogu.com.cn/record/113853163) (#7) | 261ms / 1.86s | $10^6$ |
| [P5408 第一类斯特林数·行](https://www.luogu.com.cn/problem/P5408) | [#](https://www.luogu.com.cn/record/113890000) (#3) | 90ms / 439ms | $2^{18}$ |
| [P5409 第一类斯特林数·列](https://www.luogu.com.cn/problem/P5409) | [#](https://www.luogu.com.cn/record/113887359) (#3) | 93ms / 446ms | $2^{17}$ |
| [P5395 第二类斯特林数·行](https://www.luogu.com.cn/problem/P5395) | [#](https://www.luogu.com.cn/record/113884399) (#2) | 66ms / 352ms | $2 \times 10^5$ |
| [P5396 第二类斯特林数·列](https://www.luogu.com.cn/problem/P5396) | [#](https://www.luogu.com.cn/record/113885860) (#4) | 92ms / 446ms | $2^{17}$ |
| [P5394 【模板】下降幂多项式乘法](https://www.luogu.com.cn/problem/P5394) | [#](https://www.luogu.com.cn/record/113920364) (#4) | 155ms / 1.15s | $10^5$ |
| [P5383 普通多项式转下降幂多项式](https://www.luogu.com.cn/problem/P5383) | [#](https://www.luogu.com.cn/record/113923967) (#17) | 476ms / 3.35s | $10^5$ |
| [P5393 下降幂多项式转普通多项式](https://www.luogu.com.cn/problem/P5393) | [#](https://www.luogu.com.cn/record/113926495) (#5) | 356ms / 2.50s | $2 \times 10^5$ |

#### 2023/8/21：更新了半在线卷积和全在线卷积的类

这次更新重新封装了 `poly.h` 中使用到的半在线卷积类并加以暴露，与此同时加入了 `online_conv.h` 类，对全在线卷积加以支持。

需要注意的是，两个类中实现的 `set()` 和 `current()` 函数返回的均为 **当前等待确定的位置** 对应的对角线中 **其余已经确定下来的位置** 的和。对于半在线卷积而言，这个对角线和缺了一项 $f_0 \times g_n$，而全在线卷积缺的是 $f_0 \times g_n + g_0 \times f_n$。

半在线卷积部分使用 Middle Product 加速，全在线卷积部分采用基础的四分叉做法。

#### 2023/8/25：更新 Idle 部分，并进行了大型常数优化

这次更新加入了 `Idle` 类，表示一个等待被求出各项的多项式类。你可以使用 `=` 覆盖原先的缓存指针，也可以使用 `equal()` 函数联立一个方程。最后可以调用 `get()` 或者 `await()` 求出各项的值。

具体细节请参考 https://fjzzq2002.blog.uoj.ac/blog/7281 中对 `pipeline` 和 `ocpoly` 的定义，分别对应了此处的 `Idle` 和 `PolyCache`。