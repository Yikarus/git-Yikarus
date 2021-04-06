# STL算法

总体上，STL源码剖析中将算法大体分为两类，质变算法和非质变算法，所有STL算法都作用在由迭代器[first,last)所标示出来的区间上，所谓质变算法，指运算过程中会更改区间内的元素内容，非质变算法则不会，质变算法通常提供两个版本，一个是在原地进行，另一个是拷贝进行，复制到另一个空间在其上进行，这样不会影响到原来的值。

## 数值算法

使用数值算法，必须包含头<numeric>。

##### accumulate

```C++
template <class InputIterator, class T>
    T accumulate(InputIterator first,InputIterator last,T init){
    for(;first!=last;++first)
        init=init+*first;
    return init;
}
template <class InputIterator, class T>
    T accumulate(InputIterator first,InputIterator last,T init,BinaryOperation binary_op){
    for(;first!=last;++first)
        init=binary_op(init,*first);
    return init;
}
```

accumulate默认将从first到last累加到init并返回，也可传入仿函数，自行定义处理算法。

##### adjacent_difference

```C++
template<class InputIterator,class OutputIterator>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result){
    if(first==last)return result;
    *result=*first;
    return __adjacent_difference(first,last,result,value_type(first));//这步通过value_type萃取到result的value_type
}
template<class InputIterator,class OutputIterator>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result,T*){
    T value=*first;
    while(++first!=last){
        T tmp=*first;
        *++result=tmp-value;
        value=tmp;
    }
    return ++result;//到达result末尾
}
//自定义
template<class InputIterator,class OutputIterator>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result,BinaryOperation binary_op){
    if(first==last)return result;
    *result=*first;
    return __adjacent_difference(first,last,result,value_type(first),binary_op);//这步通过value_type萃取到result的value_type
}
template<class InputIterator,class OutputIterator>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result,T*,BinaryOperation binary_op){
    T value=*first;
    while(++first!=last){
        T tmp=*first;
        *++result=binary_op(tmp,value);
        value=tmp;
    }
    return ++result;//到达result末尾
}
```

以下过程不再提供详细定义，详见STL源码剖析。

##### inner_product

```C++
template <class InputIterator1,class InputIterator2,class T>
    T inner_product(InputIterator first1,InputIterator last1,InputIterator first2,T init);
template <class InputIterator1,class InputIterator2,class T,class BinaryOperation1, class BinaryOperation2>
    T inner_product(InputIterator first1,InputIterator last1,InputIterator first2,T init,BinaryOperation1,BinaryOperation2);//binaryOpertion1指代内积的加法，binaryOperation指代内积的乘法。
```

对first1到last1内的迭代器i，从头到尾执行result=binary_op1(result,binary_op2(*i,\*(first2+(i-first1))))。

##### partial_sum

返回每一个值为相应前n位的和，或对前n个的自定义操作。

```C++
template<class InputIterator,class OutputIterator>
    OutputIterator partial_sum(InputIterator first,InputIterator last,OutputIterator result){
    //if(first==last)return result;
    //*result=*first;//初值
   	//return __partial_sum(first,last,result,value_type(first));
}
template<class InputIterator,class OutputIterator>
    OutputIterator partial_sum(InputIterator first,InputIterator last,OutputIterator result,BinaryOperation binary_op){
    //if(first==last)return result;
    //*result=*first;//初值
   	//return __partial_sum(first,last,result,value_type(first),binary_op);
}
```

##### power

SGI专属，计算某数的n次方或是对自己进行n次自定义运算。

```C++
template<class T,class Integer>
    inline T power(T x,Integer n);
template <class T,class Integer, class MonoidOpertion>
	T power(T x,Integer n,NonoidOpertion op);
```

##### iota

SGI专属，将指定序列中的一段替换位以value开头的递增序列。

```C++
template<class ForwardIterator,class T>
    void iota(TorwardIterator first, ForwardIterator last, T value);
```

## 基本算法

列出定义于<stl_algobase.h>中的所有算法，除了copy()和copy_backward()之外。

##### equal

```C++
template <class InputIterator1,class InputIterator2>
    inline bool equal(InputIterator1 first1,InputIterator last1,InputIterator2 first2);
template <class InputIterator1,class InputIterator2, class BinaryPredicate>
    inline bool equal(InputIterator1 first1,InputIterator last1,InputIterator2 first2,BinaryPredicate binary_pred);
```

注意隐含要求比较的范围必须第二个容器至少比第一个容器指定范围大。否则将访问空位置，出现运行时错误。

##### fill

将[first,last)中填入新值value

```C++
template<class ForwardIterator,class T>
    void fill(ForwardIterator first, ForwardIterator last,const T& value);
```

##### fill_n

将[first,last)中前n个元素填新值value，返回的迭代器指向被填入的元素的下一个位置。前面iota有相似作用。

```C++
template <class OutputIterator,class Size,class T>
OutputIterator fill_n(OutputIterator first,Size n,const T&value);
```

当然，这里的n若是大于容器对应长度，会有不可预期的后果。

##### iter_swap

将两个ForwardIterators所指向对象对调。

```C++
template<class ForwardIterator1,class ForwardIterator2>
    inline void iter_swap(ForwardIterator1 a,ForwardIterator2 b){
    __iter_swap(a,b,value_type(a));
}
template<class ForwardIterator1,class ForwardIterator2,class T>
    inline void __iter_swap(ForwardIterator1 a,ForwardIterator2 b,T*){
    T tmp=*a;
    *a=*b;
    *b=tmp;//正常交换。
}
```

##### lexiographical_compare

比较两个序列的大小，小于返回true，(包括小于和前面部分相等而长度小于)。

可指代仿函数，默认为less。

```C++
template <class InputIterator1,class InputIterator2,class Compare>
    bool lexicographical_compare(InputIterator1 first1,InputIterator1 last1,
                                	InputIterator2 first2,InputIterator2 last2,Compare comp)
{
    for(;first1!=last1&&first2!=last2;++first1,++first2){
        if(comp(*first1,*first2))
            return true;
        if(comp(*first2,*first1))
            return false;
    }
    return first1==last1&&first2!=last2;
}
```

##### max

##### min

##### mismatch

用来平行比较两个序列，指出两个序列第一个不匹配点，返回第一个序列和第二个序列第一个不匹配点的迭代器pair，默认使用equal比较，第一个序列必须长于或等于第二个序列的长度，否则溢出。

```C++
template<class InputIterator1,class InputIterator2,class BinaryPredicate=equal>
    pair<InputIterator1,InputIterator2>mismatch(InputIterator1 first1, InputIterator last1,
                                               	InputIterator2 first2, BinaryPredicate binary_pred){
 	while(first1!=last1&&binary_pred(*first1,first2))
    {
        ++first1;
        ++first2;
    }
    return pair<InputIterator1,InputIterator2>(first1,first2);
}
```

##### copy

将一段空间上的对象复制到目标区域，STL针对型别做了优化，若是基本类型直接调用底层操作memmove，若是迭代器类型，如果有trivival operator=也可使用memmove，若是nontrivial，有RandomAccessIterator可以用长度作为循环判断，比迭代器比较快。

返回目标区域的终点迭代器end()。

![](C:\Users\65377\Desktop\md\copy原理.jpg)

##### copy_backward

与copy相反的方向，从后往前复制。

## set算法

##### set_union

返回两个set的并集，不需要考虑元素是否可以重复的问题。返回值是result的end()，并集存在result中。

```C++
template<class InputIterator1,class InputIterator2,class OutputItertor>
    OutputIterator set_union(InputIterator1 first1,InputIterator1 last1 ,
                            	InputIterator2 first2,InputIterator2 last2,OutputIterator result){
    while(first1!=last1&&first2!=last2){
        if(*first1!<*first2){
            *result=*first1;
            ++first1;
        }
        else if(*first2<*first1){
            *result=*first2;
            ++first2;
        }
        else{
            *result=*first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return copy(first2,last2,copy(first1,last1,result));
}
```

##### set_intersection

构造两个集合的交集，和上面类似，只是只有相等的时候计入结果，所有元素都来自第一个范围。

```C++
template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_intersection(InputIterator1 first1,InputIterator1 last1,
                                   InputIterator2 first2,InputIterator2 last2,OutputIterator result);
```

##### set_difference

构造出现于第一个范围而不在第二个范围的，以上set都是排序范围。

```C++
template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_difference(InputIterator1 first1,InputIterator1 last1,
                                   InputIterator2 first2,InputIterator2 last2,OutputIterator result){
    while(first1!=last1&&first2!=last2){
        if(*first1<*first2){
            *result=*first1;
            ++first1;
          	++first2;
        }
        else if(*first2<*first1)
            ++first2;
        else if{
            ++first1;
          	++first2;
        }
    }
    return copy(first1,last1,result);
}
```

##### set_symmetric_difference

构造出现于第一个范围而不在第二个范围的和出现在第二个范围而不在第一个范围的并集，称作对称差集，即并集减去重叠的部分。

```C++
template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_symmetric_difference(InputIterator1 first1,InputIterator1 last1,
                                   InputIterator2 first2,InputIterator2 last2,OutputIterator result);
```

### heap算法

```C++
#include<stl_algo>
make_heap();
push_heap();
pop_heap();
sort_heap();
```

## 其他算法

```C++
template<class ForwardIterator>
    ForwardIterator ajacent_find(ForwardIterator first, ForwardIterator last);//找出第一组满足条件的相邻元素
template<class ForwardIterator,class BinaryPredicate>
    ForwardIterator ajacent_find(ForwardIterator first, ForwardIterator last,BinaryPredicate binary_pred);//找出第一组满足条件的相邻元素
template<class InputIterator , class T>
    typename iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last,const T& value);
	difference_type count_if(first,last,pred);//pred表示条件
	InputIterator find(first,last,value);
	InputIterator find_if(first,last,value,pred);
	Iterator find_end(first1,last1,first2,last2);//找第一个序列中最后一个出现的第二个子序列的迭代器，内部实现分为是否为双向迭代器，是的话可以使用逆向迭代器。
	InputIterator find_first_of(first1,last1,first2,last2);
	for_each(first,last,func);//对first到last采用func
    generate(first,last,gen);//gen仿函数不能有参数
	generate_n(first,n,gen);
	bool includes(first1,last1,first2,last2);//第一个是否包含第二个序列，应用于有序，因为他的实现
	bool includes(first1,last1,first2,last2,cmp);//cmp默认是less
	ForwardIterator max_element(first,last);//最大的元素的对应位置迭代器
	ForwardIterator max_element(first,last,cmp);
	OutputIterator merge(first1,last1,first2,last2,result);//result是结果存放位置，返回结果的最后迭代器，需要有序
	ForwardIterator min_element(first,last);//最小的元素的对应位置迭代器
	ForwardIterator min_element(first,last,cmp);
	BidirectionalIterator partition(first,last,pred);//将符合要求的移到序列前面，返回符合要求的最后迭代器
	ForwardIterator remove(first,last,value);//移除不删除，返回有效的最后迭代器
	ForwardIterator remove_copy(first,last,out,value);//结果放到指定位置
	ForwardIterator remove_if(first,last,pred);
	ForwardIterator remove_copy_if(first,last,out,pred);
	replace(first,last,old,new);
	replace_copy(first,last,out,old,new);
	replace_if(first,last,pred,new);
	replace_copy_if(first,last,out,pred,new);
	reverse(first,last);//颠倒重排
	reverse(first,last,out);
	rotate(first,middle,last);//一个序列从middle左右交换，左右可不平衡
	rotate(first,middle,last,out);
	ForwardIterator search(first1,last1,first2,last2);//查找第二个序列出现的第一个位置
	ForwardIterator search_n(first,last,n,value);//查找连续n个value
	ForwardIterator search_n(first,last,n,value,binary_pred);
	OutputIterator transform(first,last,out,unary_op);//对序列进行op操作放到out中
	OutputIterator transform(first1,last1,first1,out,binary_op);
	ForwardIterator	unique(first,last);//移除相邻元素，需要移除重复元素可以先排序
	ForwardIterator	unique_copy(first,last,out);

	ForwardIterator	lower_bound(first,last,value);//应用于有序，返回value所在地或是他最应该出现的地方，默认是不小于
    ForwardIterator	lower_bound(first,last,value,cmp);
    ForwardIterator	upper_bound(first,last,value);//应用于有序，返回value所在地或是他最应该出现的地方，默认是不大于
    ForwardIterator	upper_bound(first,last,value,cmp);

	bool binary_search(first,last,value);//已排序中查找存在
    bool binary_search(first,last,value,cmp);
	//排列算法
	bool next_permutation(first,last);//利用元素大小关系，表现排列顺序，例如abc最小，每一个都小于后一个
	//实现，从后往前寻找相邻的i,ii，i<ii，从最后往前找第一个大于i的，和i交换，将i之后的逆序，得到下一个，找不到则没有。
	bool prev_permutation(first,last);

	random_shuffle(first,last);//生成随机序列，利用随机数随机交换序列

	partial_sort(first,middle,last);//将区间内的最小的middle-first个元素排序放在[first,middle)内，实现方法是，将[first,middle)构建为heap，与[middle,last)逐一比较，将大的放进去
	partial_sort_copy(first,last,outf,outl);
    partial_sort_copy(first,last,outf,outl,cmp);
```

