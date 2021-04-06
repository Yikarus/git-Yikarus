## Set

​	set的特性是，所有元素都会根据键值自动被排序，set的元素不像Map那样可以同时拥有value和key，set元素的键值就是实值，**实值就是键值**，不允许两个元素有相同键值。

​	<font color=red>不可以通过set的迭代器改变set的元素值</font>，因为set的元素值就是键值，关系到set元素的排列规则，如果任意改变，严重破坏set组织。set<T>::iterator被定义为底层RB-tree的const_iteartor，set_iterators是一种constant	iteartors。

​	set与list一样，新建或删除元素不会导致迭代器失效。

​	所有set的操作，只是调用RB-tree的接口罢了。

```C++
template <class Key, class Compare=less<Key>,class Alloc=alloc>
    class set{
        //截取
    public:
        typedef	Key	key_type;
        typedef	Key	value_type;//键值一致
        typedef	Compare	key_compare;
        typedef	Compare	value_compare;
    private:
        template <class T>
            struct	identity:public	unary_function<T,T>	{
                const	T&	operator()(const T& x)const{
                    return x;//即identity<T>(x)=x，为了从键取值
                }
            }
        typedef	rb_tree<key_type,value_type,identity<value_type>,key_compare,Alloc> rep_type;
        rep_type	t;
    public:
        typedef	typename	rep_type::const_pointer	pointer;
        typedef	typename	rep_type::const_pointer	const_pointer;//iterator和const_iterator都是const
        typedef	typename	rep_type::const_reference	reference;
        typedef	typename	rep_type::const_reference	const_reference;
        typedef typename	rep_type::const_iterator	iterator;
        typedef	typename	rep_type::const_iterator	const_iterator;
        typedef	typename	rep_type::const_reverse_iterator	reverse_iterator;
        typedef	typename	rep_type::const_reverse_iterator	const_reverse_iterator;
        typedef typename	rep_type::size_type			size_type;
        typedef	typename	rep_type::difference_type	difference_type;
        
        set():t(Compare()){}
        explicit	set(const Compare&	comp):t(comp){}//当有这个的时候不准从Compare隐式转换
        template<class InputIterator>
            set(InputIterator	first,InputIterator	last):t(Compare()){
            t.insert_unique(first,last);//set使用insert_unique()，multiset使用insert_equal()
        }
        typedef	pair<iterator,bool>	pair_iteartor_bool;
        pair<iterator,bool>	insert(const value_type&x){
            pair<typename rep_type::iterator,bool>p=t.insert_unique(x);
            return pair<iterator,bool>(p.first,p.second);
        }
        iterator insert(iterator position,const value_type&x){
            typedef typename rep_type::iterator	rep_iterator;
            return t.insert_que((rep_iterator&)position,x);
        }
        template<class InputIterator >
            void insert(InputIterator first,InputIterator last){
            t.insert_unique(first,last);
        }
        void erase(iterator position);
        size_type erase(const key_type&x);
        void erase(iterator first,iterator last);
        void clear(){t.clear();}
        iterator find(const key_type& x)const;
        size_type count(const key_type& x)const;
        iterator lower_bound(const key_type&x)const;
        iterator upper_bound(const key_type&x)const; 
        iterator equal_range(const key_type&x)const;
    }
```

## map

​	map特性是，所有元素都会根据元素的键值自动被排序，map的所有元素都是pair，同时拥有实值和键值，map不允许使用两个相等的键值，底层使用RB-tree实现。

​	定义类似set，只是这里key!=value。

## multiset和multimap

​	实现类似，区别是允许元素拥有相同的键值，底层实现上，RB-tree插入使用insert_equal()而不是insert_unique()。

## hashtable

​	hashtable可以提供对任何有名项的存取和删除操作，由于操作对象是有名项，所以hashtable被视为字典，这种结构的用以在于提供常数时间的基本操作。	

​	使用hash function将需要比较的值映射到一个可以接受的范围(避免索引太大，表太空)，但是有一个问题，可能有不同的元素映射到同一个位置，称作碰撞问题，解决碰撞问题现在有三种方法，线性探测，二次探测，开链。

### 线性探测

​	冲突则线性的寻找下一个地址，只要表格足够大，就能找到空间，元素的删除使用惰性删除，标记为删除，等表格重新整理时再删除。

​	会导致主集团问题，当一系列操作后，我们手上有的时一大团已被使用的方格，插入操作可能需要不断解决碰撞问题，再次扩大主集团。

### 二次探测

​	二次探测用来解决主集团问题，如果碰撞，就依序尝试+1^2，+2^2，+3^2，而不是线性探测。

​	如果我们假设表格长度为质数，且永远保持负载系数再0.5一下，就可以确定插入新元素的探测次数不多于2。

​	二次探测的乘法成本可以通过技巧解决。

​	H(i)=H(0)+i*i%M							=>				H(i)-H(i-1)=(i\*i-(i-1)\*(i-1))%M

​	H(i-1)=H(0)+(i-1)*(i-1)%M			 =>				H(i)=H(i-1)+(2\*i-1)%M

​	转换为一个加法一个移位一个减法，一个取模。

​	二次探测可以消除主集团，但是却带来次集团：两个元素经hash function计算出来的位置若相同，则插入时所探测的位置也相同，形成某种浪费，消除次集团的方法有复式散列。

### 开链

​	在每一个表格元素中维护一个链表，相同的放入同一个链表，如果list足够短，速度还是够快的。

### iterator

​	hash_table是一个存储着自定义单向列表的vector。

```c++
template <class Value>
    struct	__hashtable_node{
        __hashtable_node *next;//只保存下一个节点的值，单向列表
        Value val;
    }
template <class Value,class Key,class HashFcn, class ExtractKey, class EqualKey,class Alloc>
    struct __hashtable_iterator{
        typedef hashtable<Value,key,HashFcn,ExtractKey,EqualKey,Alloc> hashtable;
        typedef	__hash_table_iterator<Value,key,HashFcn,ExtractKey,EqualKey,Alloc> iterator;
        typedef	__hashtable_const_iterator<Value,key,HashFcn,ExtractKey,EqualKey,Alloc> const_iterator;
        typedef	__hashtable_node<Value> node;
        typedef	forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef	ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef Value& reference_type;
        typedef Value* pointer;
        node*	cur;//迭代器目前指向的节点
        hashtable* ht;//保存容器的连接，为了从一个bucket跳到另一个bucket
        __hashtable_iterator(node *n,hashtable * tab):cur(n),ht(tab){}
        __hashtable_iterator(){}
        reference	operator*()const {return cur->val;}
        pointer 	operator->()const {return &(operator*());}
        iteartor&	operator++();
        iterator&	operator++(int);
        bool		operator==(const iterator& it)const{return cur==it.cur;}
        bool		operator!=(const iterator& it)const{return cur!=it.cur;}
    }
```

​	hash_table实现的是一个单向迭代器，单也可以快速跳到前一个节点，只要已知前一个节点的hash_value，就可以通过保存的hash_table指针跳转过去。

### hashtable数据结构

```C++
template<class Value,class Key,class Hashfcn,class ExtractKey,class EqualKey,class Alloc=alloc>
    class hashtable{
    public:
        typedef	HashFcn		hasher;
        typedef EqualKey	key_equal;
        typedef	size_t		size_type;
    private:
        hasher		hash;
        key_equal	equals;
        ExtractKey	get_key;
        typedef		__hashtable_node<Value> node;
        typedef 	simple_alloc<node,Alloc> node_allocator;
        vector<node*,Alloc> buckets;
        size_type	num_elements;
    public:
        size_type 	bucket_count()const{return buckets.size();}//坑位
    }
```

​	hash_table中存储了28个质数，作为hash_table的候选长度，虽然开链法没有长度限制，但是质数能够大幅降低冲突，提高效率。

​	hash_table中处理数据时，常常需要知道他在hash_table中位置，内部使用取模处理，有些数据类型例如字符串无法直接取模，除非用户提供相应转换函数，这些无法直接处理。

### hash_set

### hash_map

### hash_multiset

### hash_multimap



