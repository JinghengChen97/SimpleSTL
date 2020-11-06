# SimpleSTL

## 简介
+ 基于C++的SimpleSTL
+ 创建该项目的目的：
	1. 学习《STL源码剖析》的一个总结与实践
	2. 深入了解C++ STL库，弄清楚在程序中调用STL会发生啥事情、弄清楚实现STL所使用的算法与数据结构
+ 参考：
	+ 《STL源码剖析》
	+ github项目：[MyTinySTL](https://github.com/Alinshans/MyTinySTL) 
+ 需要：
	+ test文件需要CMake编译

## 文件描述

### 第一部分：construct.h/uninitialized.h/type_traits.h/alloc.h
+ 这一部分对应的是《STL源码剖析》的第二、三章

#### 1. type_traits.h
+ type_traits里面定义了__type_traits的泛化版本与特化版本，其中特化版本都是针对内置类型的。
+ 在stl的的算法中，常常需要知道迭代器的相应类型（迭代器所指之物的类型就是相应类型的一种），但C++没有支持直接提取指针相应类型的操作符或函数；那我们应该怎么解决这个问题呢？
	+ 第一个办法：利用C++的模板参数推导机制
	+ ~~~c++
    template <class T1, class T2>
    void _fun (T1 a, T2 b) {
	  ...
    }

    template <class T>
    void fun(T i) {
        _fun(i, *i);//_fun中的T1被推导为int*, T2被推导为int；就是说这里fun识别出了T的所指类型
    }

    int main() {
        int i = 0; 
        fun(&i);
        return 0;
    }
	  ~~~
+ 问题：不是每一种相应类型都能这么干，而且这种方案没法从迭代器中推导出函数的返回值。
	+ 从而引出第二个解决方案：利用内嵌类型
	+ ~~~c++
    template <class T>
    struct Iterator {
        typename T value_type;
        T* ptr;//其实这个才是Iterator的主要成员（本体）
        Iterator(T* p = 0) : ptr(p) { }
        T& operator* () const { return *ptr; }
        //... 
    };
    template <class T>
    typename T::value_type//这样就可以获取T的迭代器的相应类型了（就是T本身）
    fun () {
    }
    ~~~
+ 但这样会出现新的问题：内置类型是不能声明内嵌类型的。
	+ 最后，利用模板特化+模板参数推导机制，引出第三个解决方案，这个方案也是stl中采用的方案：traits技巧
	+ ~~~c++
    template <class T>
    typename type_traits<T>::value_type
    fun(I* i) {
        return *i;
    }

    其中，type_traits：
    template<class T> type_traits {
        typedef T::value_type value_type;//这一句说明了：任何一个想与stl算法完美融合的class，必须定义自己的value_type 
    }
    ~~~
	+ 当参数推导得到的T不是内置类型时，type_traits<T>::value_type就会匹配到上面那个版本，调用的value_type就是class自己定义的value_ytpe
	+ 但内置类型没有自己的value_type，怎么把内置类型的相应类型提取出来？
	+ 这里先弄清楚一件事情：type_traits<T> ... 中的T是什么？我们知道这一定是迭代器，那么怎么区分开非内置类型和内置类型的迭代器呢？
    	+ 当T是非内置类型的迭代器时，T是一个类而不是指针（即使迭代器的行为与指针类似）
		+ 当T是内置类型的迭代器时，T是指针（我们希望内置类型的迭代器就是指针，这是由设计目标所决定的）
    + 因此，两种类型的迭代器的关键区别在于：一个是指针，一个是class
    + 那么我们特化出一个指针的模板就行了：
    ~~~c++
    template <class T> type_traits<T*> {
        typedef T value_type;
    }
    ~~~
    + 当参数推导得到的T是内置类型的迭代器（也就是指针）时，type_traits，那么就可以得到T*的相应类型T了。


#### 2.iterator_traits.h
+ 利用traits技巧，定义了关于迭代器的iterator_traits
+ iterator_traits的职责是提取出迭代器相应类别
+ 迭代器的相应类别有以下几种：
	+  value_type
	+  difference_type
	+  pointer
	+  reference_type
	+  iterator_category
		+ 根据移动特性与操作特性划分的迭代器的种类，有五种：
		+ input_iterator
		+ output_iterator
		+ forward_iterator
		+ bidirectional_iterator
		+ random_access_iterator
+ 另外，还定义了`value_type()`,`distance_type()`,`iterator_category()`三个函数（事实上不会影响程序效率）这三个函数的作用在《stl源码剖析》中有具体描述
+ 其中让人迷惑的是`value_type()`函数：为何其返回的是一个`value_type*`，即一个指针？不可以直接返回`value_type`这个类型本身吗？
+ 我们先看`value_type()`这个函数是怎么用的：
	+ ~~~c++
	  template <class InputIterator, class ForwardIterator>
    inline ForwardIterator
    uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
    	return __uninitialized_copy(first, last, result, value_type(result));
    }
template <class InputIterator, class ForwardIterator, class T1>
inline ForwardIterator
    __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T1*) {
    typedef typename __type_traits<T1>::is_POD_type POD;
		return __uninitialized_copy_aux(first, last, result, POD());
	  }
	  ~~~
	+ 可以看到，首先是`uninitialized_copy()`向`__uninitialized_copy()`传入了`value_type(result)`,然后`__uninitialized_copy()`根据参数推导机制与`__type_traits`，推出了`result`所指之物的类型。
	+ 在这个过程中，如果假设`value_type(result)`返回的不是指针而是`result`所指之物的类型本身，那么`__uninitialized_copy`的`T1*`参数也要修改成`T1`,也就是说我们每次调用`__uninitialized_copy`都要创建一个`T1`类型的对象，这是不可接受的。