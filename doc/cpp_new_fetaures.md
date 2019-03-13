## C++ New Features

自从C++11开始, 经历了C++14/C++17, 以至于现在的C++20. C++已经趋向于一门成熟的现代化语言.
使用C++的新特性, 一方面可以提高效率, 另一方面也能减少编程的负担.

在`platinum`中, 使用了这些C++的新特性 [基于C++11, 使用了少部分C++14特性]

### Smart Pointer

C++11提供了智能指针设施: `std::shared_ptr`, `std::unique_ptr`位于(`<memory>`下).
- `std::shared_ptr` 共享资源, 使用引用计数, RAII, 我们可以不需要小心翼翼的控制对象生命周期
- `std::unique_ptr` 独享资源, 不能进行拷贝 (`return std::move()`例外)

其中对于, `std::unique_ptr`我们基本上可以使用此设施, 完全替代项目中的裸指针
`platinum`中, 对于所有的类内资源, (值语义的例外, 如`fd_`)使用智能指针进行资源管理.

历史遗留:
`std::auto_ptr`(C++98). 在拷贝时会自行`release`, 无法配合容器.(`EffectiveSTL`建议禁用)
`boost::shared_ptr` 是`Boost`库中可共享的智能指针实现, 标准库的实现基于此版本
`boost::scoped_ptr` 是`Boost`库中独占资源的智能指针实现, 标准库中`std::unique_ptr`与此类似

那么, 在使用上, 我们建议采用C++14的两种设施: `std::make_shared`及`std::make_unique`
除特殊情况, 一般都可以用一次分配的方式, 取代先`new`之后再分配引用计数块的形式

### callable object

在C++目前版本中, C++中对于函数(`Function`)有这么几种形式的设施:
- 函数

- 函数指针 (C/C++) 如: 
```cpp
void func(int) { ; } 
void(*)(int) p = func;
```
- 函数对象(functor) 如:
```cpp
struct Func {
  bool operator()(int a) {
    std::cout << a << std::endl;
  }
};
Func(3);
```
- std::bind (C++11 bind更强劲)

- lambda expr (C++11 引入)
```cpp
auto f = []() { std::cout << 1 << std::endl; };
f();
```
那么比较一下上面几类内容:
- 函数是最自然的存在, 直接调用即可
- 函数指针使函数的入口地址, 在传参是函数和函数指针用法基本一致, 类型判定上会有区别
- 函数对象用法同函数指针, 但是它可以**保存状态**, 不过由于类型并非函数指针, 用处受限制
- std::bind的返回值, 也是可调用对象, (不过C++11, 可以支持数十个`placeholder`)
- lambda expr 本质上来讲: **匿名函数对象**

那么, 这样常见的四种形式, 并不完全兼容, 很多时候都是互相不通用.
C++11提供了`std::function`进行类型消除, 以统一的可调用对象形式 `callable object`来定义
所有的可调用对象, 只要调用形式相同(返回值和参数)便可以被包裹, 从而统一调用.

对于上面的几种形式, 函数无法替代, 函数指针基本可以不使用 (除库设施之外)
推荐: 1. lambda expr, 2. std::bind

lambda中的好处在于:
1. 同样是函数对象, 但是他立即可见
2. 它是匿名对象, 用完出块即释放, 不会污染命名空间
3. 相比与`std::bind`, lambda expr有效率上的提升
4. 使用lambda, 减少心智负担 (回调机制使得代码支零破碎)
5. 使用lambda包裹, 可以减少很多`function adaptor`的使用

`std::bind` 暂且不提. 因为~~个人实在用的有限, 基本都能被lambda expr取代~~

`platinum`中, 回调函数全部使用lambda expr
(注: 此版本未更新, 仍然是muduo的形式, 使用`std::function`作参, 但是传参仍然是函数指针
在新实现的项目 `levelnet`中, 完全使用lambda, 网络层基本和`platinum`一致)

### auto

C++11 中赋予了`auto`关键字新的含义: 自动类型推导 (根据返回值进行推导)
不用多说, 好用谁用谁知道.

起码不用写`std::vector<int>::iterator iter = vec.cbegin();`了

### R-Value Reference

在提升效率方面: C++11着重于减少拷贝
新增加的右值语义, 很好的解决了这个问题.
相比于左值, 右值表示即将被销毁的对象. 早期的C++并不能移动, 只能强行拷贝

是的, **有的对象在用过即销毁的时候, 还要被拷贝**
为何不直接用其赋值 ?

之前只有左值, 左值拷贝, 需要分配内存, 之后拷贝. 右值就不一样了, **直接赋值就好了**
在对于大对象的操作的时候, 可以有效的减少拷贝
使用`std::move`即可.

而且, 支持移动语义了之后,我们也可以将某些对象放入容器.
是的, 比如我们可以不再使用`boost::ptr_vector`等等pointer container

`platinum`中使用了移动语义, 不过范围比较小.
*1. 移动语义的实际用处并非很熟练*
*2. 移动语义针对大对象拷贝效率提升明显, 小对象一般*

### Container::emplace

C++新标准之前, 我们使用容器时会使用对应的操作进行容器元素的添加, 如:
顺序容器: `Container::push`, 关联容器: `Container::insert`

但是, 这些操作都是先拷贝, 然后有一次插入的, 中间有一次无用的拷贝.

新标准中添加了`emplace`(C++11), `emplace_hint`(C++14)
`emplace`减少了一次拷贝, 直接在容器中进行构造, 对顺序容器, 关联容器都适用
`emplace_hint`表示在指定位置构造, 会尽可能靠近指定的iterator

`platinum`中尽可能多的使用`emplace`, 以此减少拷贝, 提高效率