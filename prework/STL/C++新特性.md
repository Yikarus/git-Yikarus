# C++新特性

## 列表初始化

​	用花括号来初始化变量。

```C++
int a=0;
int a={0};
int a{0};
int a(0);
```

​	若是初始化类型不对隐式转换无用会引起编译器报警。

## 常量表达式

​	一般说的时初始化const变量，但是很难分辨初始值是否是常量表达式，新标准允许将变量声明为constexpr来验证变量初始值是否为常量表达式，也可定义constexpr，本质上还是常量，这种函数在编译时就由编译器计算得结果了。

## 类型别名

​	类型别名使用typedef或using定义的，using是新标准。当别名混合其他类型定义，会有意想不到的结果。

```C++
typedef char *pstring;
const	pstring	cstr=0;//指向char的常量指针
const 	pstring *ps;//ps是指针，他的对象指向const char* 
//而不能翻译成 const char * *ps，指向char*的常量指针
```

## auto

​	自动推断类型

## decltype

​	得到对象的类型，而不计算他的值，decltype可以获得引用和顶层const。

## 类内初始化

​	支持类内直接赋值初始化，顺序按声明顺序？以及构造函数列表初始化的顺序？(这里感觉是按声明顺序)。

##  for

```C++
for(auto i : vector )
for(auto &i: vector	)
```

## vector列表初始化

```C++
vector<string>  list{"hh"};//一个元素
vector<string> 	list("hh");//不能使用"hh"构建vector
vector<string>	list{10};//10个默认值
vector<string>	list{10,"hh"};//10个“hh"，这两处列表初始化失败，转而调用构造函数
vector<int>	v;
v={1,2,3,4,5,6};//列表直接赋值
```

## 容器的cbegin和cend

​	是相应的const_iterator，不能改变对象的值。

## 除法舍入规则

​	向0取整。