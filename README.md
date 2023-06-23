# algorithm-templates

由 tiger2005 编写的模板库，包括针对 Sublime Text 的插件。这个模板库仅供学习参考，若要拉取使用，本人并不敢保证所有代码的准确性。

## 插件介绍

插件位于 `sublime` 文件夹下，包含了 `TemplateExpand` 文件夹，以及 `README.md`，描述了插件的工作流程。

若要在 Sublime Text 中安装这个插件，需要进行如下操作：
- 选择 `首选项` > `浏览插件目录`，将 `TemplateExpand` 文件夹复制到其中；
- 打开内部的 `TemplaceExpand.sublime-settings` 文件，将其中的 `file-prefix` 字段修改为电脑中包含 `lib` 的文件夹路径（
不应包含 `lib`，也不应在末尾添加 `/` 或者 `\`）
- 在 Sublime Text 中和 C++ 文件相关的 compiler 和 linter 中添加对模板库的引用，对于模板库地址在 `/user/tiger2005/path/to/lib/` 的情况，应当在编译命令和运行命令后加上：`-I "/user/tiger2005/path/to/"`。设置方式视插件而定，例如一些插件会给出 `include` 的列表，只需要添加进去即可。
- 重启 Sublime Text，稍稍配置一下模板文件，在其中加入诸如 `#include "lib/z.h"` 的引用，然后：
  - 输入 `ctrl + k` 和 `ctrl + l`（按住 `ctrl`，然后两根手指分别输入 `k` 和 `l` 即可），展开所有这样的引用，其中自动引用了模板的依赖项；
  - 输入 `ctrl + l` 和 `ctrl + k`，折叠所有这样的引用，并且自动删除依赖项。

上述快捷键可以在插件文件中改变，详情见 `sublime` 文件夹下的 `README.md`。

## 模板库标准

除了一些全局的定义之外，每个模板库的文件遵循一定的规则，例如：

```cpp
// decl
// 上面这行注释没有实际意义，可以省略。
// 下面的注释保证了每个头文件只会被引用一次，防止重复定义。
#ifndef TEMPLATE_CPP_EXAMPLE
#define TEMPLATE_CPP_EXAMPLE 1
// 下面是对基础库的调用，保证最初的模板文件引用了这个模板库所需要的所有基础库。
// 这一行为 linter 服务。在 cpp 文件中需要包含：
// #define NO_TEMPLATE_IMPORT
// 以此保证在 cpp 文件中，linter 可以正常进行。
// 同时，在头文件中，由于这个字段没有被定义，linter 依然可以正常进行。
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
#endif
// 下面这一行标志了模板实现的开始，是插件识别的指标，请不要随意删除或移动这一行。
// 在植入模板时，将会从这个注释的下一行开始，将所有空行去除后加入到主程序中。
// impl
// 下面是对模板的引用，路径均会以 lib 开始，方便插件识别。
#include "lib/z.h"
// 在一定的命名空间中实现类或者函数
namespace example {
template <typename T>
struct Example {
  vector<T> n;
};
}  // namespace example
// 下面利用 using 对类或者函数进行导出，并运用默认设置。
using Fact = example::Example<Z>;
#endif
```

插件的行为依赖于文件内容，在新建模板的时候，请务必保证符合上述介绍，至少符合 `// impl` 的要求。