# muduoC++库多线程

## 线程安全的对象生命期管理

​	在编写线程安全的类不是难事，使用同步原语就可以保护类的内部状态，但是同步原语例如mutex难以保护类的构造和释放，类的构造时，刚开始建立整个对象，mutex尚未被构建，整个对象也是半成品，析构时，Mutex一定先于整个对象被析构，此时若是另一个线程访问该对象，就会造成竟态条件。

​	析构时：

- 当即将析构一个对象时，怎么知道是不是有另一个线程正在访问该对象的成员函数？
- 怎么保证执行成员函数的时候，该对象没有被另一个线程析构？
- 再调用一个对象的函数之前，怎么知道这个对象还活着，会不会析构函数执行到一半，问的时候还活着，其实半死不活，中间就死了，就会有问题。

## 线程安全的定义

根据JCP(Java Community Process)，一个线程安全的类需要满足以下条件：

- 多个线程同时访问的时候，表现出正确的行为，针对上面问题，就要求能够得知对象死活，是不是能够正常使用。
- 无论操作系统如何调度这些线程，执行顺序如何
- 调用的代码不需要其他的同步协调动作，这个意思我不需要在我使用的时候还要自己判断他怎么了，同步防止出现问题这些事情应该是类自己在封装时都考虑好了。

### 构造过程

​	构造过程中不要泄露this指针，就是不要在对象还没构造完成就把自己交出去处理任务，还没有成熟，相当于雇佣童工，不清楚她有没有能力。

- 不要再构造函数中注册任何回调，注册回调就是类似于你的杯子还没买，东西已经到了，可能注册的容器还没准备好。
- 不要再构造函数中把this传给跨线程的对象。构造传入另一个对象的指针，再构造函数中把自己注册到另一个对象中，这时另一个对象就会使用还未完成的对象执行程序，严重错误。
- 即便最后一行也不行，最后一行只要在构造函数中，该对象就还没有准备好。

### 智能指针救我狗命

​	考虑之前的问题，如何判断对象是否活着，是否还可以用，我用到一半是不时你就死了？

​	例如一下代码：

```C++
//Object1用来传递
~Object1(){
    Mutex lock(mutex_);//获取锁以释放整个对象，假设有另一个在使用则等他用完再释放
    					
}
//Object2调用该接口
void Object2::register(Object1 * p){
    regist.push_back(p);
}
void Object2::use(Object1 * p )
{
    if(p)
    {//如果在此时Object1在析构函数之中，p还未释放，能进入函数，mutex被析构函数持有，等待mutex释放，然后mutex被析构了，整个p都没有了，这怎么执行
        p->lock();
    	p->fun();
    	p->unlock();
    }
}
global Object2 b;
//Thread1,Object1
int main(){
    Object1 a;
    b.register(&a);
    a.~Object1();
}
//Thread2
int  main(){
    b.isok();//注册列表有值
    b.use();
}
```

​	计数型指针很好的解决了该问题，shared_ptr<T>，作为栈上对象，保存着真正对象的地址，计数达到0，释放本身，释放真实对象。

​	weak_ptr是为了不增加shared_ptr计数并判断是否存在的，若不使用weak_ptr而使用shared_ptr，则使用一个shared_ptr时，若其实原始已经到0释放了，再获得也没有意义，以weak_ptr，判断当前是否还在，在就可以转成shared_ptr一起用。

​	对上述情况以shared_ptr/weak_ptr作为说明，上述形象比喻就是，Object1我自己用完了我就滚了，你Object2要注册我，好我给你我的位置，你来找我，我感觉不行我看看有没有人在找我，没有，我收拾收拾准备撤了，这时候Object2来了，我通过地址来找你了，你还在，我等你不忙了在说吧，你怎么在收东西？怎么过了会人没了，那我是谁我来干嘛的，作为Object1，我收东西了，你也没叫我啊，我就走了，你倒是拍拍我。

​	使用shared_ptr/weak_ptr，Object1我自己用完了我就滚了，你Object2要注册我，好我给你我的位置，你来找我，我感觉不行我看看有没有人在找我，没有，我收拾收拾准备撤了，这时候Object2来了，我通过地址来找你了，你还在不在啊，你说你在，那等会我，先别忙着收拾，先帮我干个活(此时shared_ptr未清0，真正的对象析构并未完成)，你不在了？那行吧，做不成了，我干其他的了，(shared_ptr清0，再转而释放Object1，此时Object1的析构函数并未调用，使用shared_ptr的效果是，我要么完整的在，要么就没了，爱恨就在一瞬间，没有那么慢的析构过程)。

​	shared_ptr的技术陷阱，例如若要实现注册回调函数，或任务队列，若使用vector<shared_ptr>作为队列，则会引起内存的浪费，vector<shared_ptr>本身就持有了智能指针，这就造成了指针计数最少为1，纯靠外界注册和取消，解决不了队列中保存的那一份，除非手动在观察者中调用unregister()。

​	shared_ptr作为函数参数时以引用传递就不会遇到反复拷贝shared_ptr遇到的问题。

​	析构动作在创建时被捕获，这意味着：

- 虚析构不在是必须的
- shared_ptr可以持有任何对象而且可以安全释放
- shared_ptr可以安全跨越模块边界，不会存在从模块A里分配的内存在模块B里被释放这种错误。
- 二进制兼容性，前提对象由工厂模式创造且返回shared_ptr。
- 析构动作可以定制。

析构函数的这一属性事实上是通过分别识别两种指针类型来实现的。智能指针中shared_ptr<typename Base*>(new Derived)，shared_ptr内部识别为两种指针，Base\*和Derived\*内部使用用的是Base\*类型，释放用的是Derived\*类型，故而没有虚析构也可以安全释放。

​	CSDN上一个示意实现如下:

```C++
#include <iostream>

template <typename Base>
class simple_ptr_internal_interface {
public:
    virtual Base* get() = 0;
    virtual void destruct() = 0;
}; // class simple_ptr_internal_interface

template <typename Base, typename Derived>
class simple_ptr_internal: public simple_ptr_internal_interface<Base>
{
public:
    struct DefaultDeleter {
        void operator()(Base* t) { delete static_cast<Derived*>(t); }
    };
    simple_ptr_internal(Base* p): pointer(p) {}

    virtual Base* get() override { return pointer; }
    virtual void destruct() override { deleter(pointer); }

private:
    Base* pointer;
    DefaultDeleter deleter;
}; // class simple_ptr_internal

template <typename Base>
class simple_ptr {
public:
    template <typename Derived>
    simple_ptr(Derived* d): internal(new simple_ptr_internal<Base, Derived>{d}) {}
    ~simple_ptr() { this->destruct(); }

private:
    void destruct() { internal->destruct(); }
    simple_ptr_internal_interface<Base>* internal;
}; 

class base
{
public:
    base(){std::cout << "base construct" << std::endl;}
    ~base(){std::cout << "base destruct" << std::endl;}
};

class derived: public base
{
public:
    derived(){std::cout << "derived construct " << std::endl;}
    ~derived(){std::cout << "derived destruct" << std::endl;}
};

int main()
{
    simple_ptr<base>(new derived());
    return 0;
}
————————————————
版权声明：本文为CSDN博主「曈默生」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/elija940818/article/details/102868664
```

### 例子

​	假设一个股票类，代表股票价格，可以使用map<shared_ptr> stocks保存每只股票的价格，需要的时候利用get方法取得他的智能指针，

问题1：使用map<shared_ptr>，即使没有人使用了，但是容器中保存的是shared_ptr，永远不会被释放。

改进：使用weak_ptr

问题2：使用weak_ptr后固然对象释放了，但是没有清理map，map里有残存的信息。

改进：使用shared_ptr定制的撤销动作，shared_ptr<stock>(new stock,&destruct,this,_1)，就能在都没有的时候调用工厂的一个删除map中对象的函数。

问题3：上面的问题带来新的问题，如果在调用工厂删除函数时工厂析构了怎么办？

改进：使用enable_shared_from_this，只是一个以其派生类为模板类型实参的积累模板，继承他，this将变成shared_ptr，就能保存一份备用的，使用shared_from_this()，类对象不能是栈对象，必须是堆对象并且由shared_ptr管理生命，如shared_ptr<StockFactory>(new StockFactory)。

问题4：这样使用enable_shared_from_this()延长了对象的生命期。

改进：在定制的析构中绑定weak_ptr，判断还活着么，分情况处理，就相对完美了。

## 线程同步原则

- 首要原则是尽量最低限度使用共享对象，减少同步场合，实在需要，优先暴露不可修改变量。
- 其次使用高级并发编程构件，如TaskQueue、Producer-Consumer Queue、CountDownLatch等。
- 若必须使用底层同步原语，尽量使用非递归的互斥器和条件变量，慎用读写锁，不用信号量。
- 除了使用atomic整数外，不自己编写lock-free代码也不使用内核级同步原语(消耗大)。

对于底层同步原语的选择，熟知的锁有mutex，condition_mutex，read-write lock，file lock，spin lock，陈硕建议只是用非递归的互斥量mutex和condition_mutex，mutex的消耗已经很小。

​	遇到频繁读而少写时，我们经常以rwlock替代mutex，不一定会带来性能提升，读锁的开销不比mutex小，且容易犯错，如果在加读锁的同时对对象进行了修改，那就跟没有加锁一样，如果rwlock允许提升为写锁，就会和递归锁一样，有造成一些后果的风险。

​	陈硕对非递归锁举例反驳了：

```C++
MutexLock mutex;
std::vector<Foo>	foos;
void post(const Foo& f){
    MutexLockGuard Lock(&mutex);
    foos.push_back(f);
}
void traverse(){
    MutexLockGuard lock(&mutex);
    for(std::vector<Foo>::const_iterator it =foos.begin();it!=foos.end();it++){
        it->doit();//如果doit()间接调用了post()，使用迭代器，若Mutex为非递归，没有问题，持有锁只有一个人在访问。
        		//若使用递归，post的push_back有几率导致迭代器失效导致程序崩溃。
    }
}
```

​	对于编程不良的人，有出错的风险，包括读写锁也是，但是只要程序设计合理，没有上述有的没的，使用读写锁确实能够提升性能，尽管开销差不多，但是读锁可以重复拥有，有并行的可能。

Yikarus:

​	书中p56的例子，提到了线程间数据读写同步需要注意的事情，也是一般数据库需要注意的事情之一，数据的一致性，可能导致误读。规则之一：

共享的数据可以多个线程同时读，只有一个事件写，这个读写锁好像可以正常解决，虽然有他固然的一些错误，靠良好的编程风格解决，muduo库摒弃其他复杂锁，只是用mutex和条件变量，读数据时加锁，内存中复制一份保存，释放锁，数据不可以并行读，但是可以并发读，同时只有一个线程在对共享数据操作，每个线程读的时候仅仅复制然后马上释放锁，以增进其他线程利用他的机会，另外写数据修改共享数据的线程此时拥有锁虽然保证了只有自己在对共享数据操作，但是此时读线程拥有共享数据副本，若是直接操作会导致读线程的误读，因此需要判断共享数据是否被独占，这个用智能指针就可以解决，绝配，但是在这个时候也需要等读线程结束，不在拥有数据，除了读线程可以并发多处理数据外，对写线程似乎并没有起到多么优化的作用，读写锁还可以并行读取，自己调度读写进行的执行，不需要直能指针的另外操作，虽然会引起一些程序员误操作的反复加锁的问题，性能上感觉会优于Mutex，虽然mutex可以明确业务逻辑上的问题，也可以解决上文可能存在的迭代器失效问题，但是若不存在上述问题，可以优先读写锁。

​	读写锁重入引起死锁：假设在一个大的事务中，先申请一个读锁，之后针对一些状态列表加读锁访问状态作相应处理，这些状态读取的读锁申请和释放很快，但是如果在大的读锁下，小的读锁之间，有个刷新线程申请写锁，此时他等待现有的读锁释放之后独占，因此当时正在执行的读锁执行完释放，此时由于在大的读锁内，大的读锁没有释放，其内事务尚未处理完，接着的读锁由于写锁正在申请因此阻塞，导致整个事务无法进行下去，导致整个大的读锁无法释放，形成读写锁相互等待，导致死锁。



​	muduo库中提供了Mutex的封装类MutexLockGuard，构造中上锁，析构中解锁，将MutexLockGuard放入局部作用域可以保护一整个作用域，可以用{}灵活的使用，省去了繁琐的加锁解锁操作，MutexLockGuard加锁时保存加锁线程id，在另线程需要使用数据时可先判断是否有人占用，不必在上面阻塞。

## 多线程应用场景

​	多线程的应用在于充分利用多核处理器的性能。

#### 推荐C++多线程服务端编程模式

​	one (even) loop per thread + thread pool

- event loop，也叫IO multiplexing，配合non-blocking IO 和定时器
- thread pool用来做计算，可以是任务队列或者生产消费者队列。

推荐进程间通信使用socket，在多服务器下以及单服务器都通用，能跨机器，TCP port有一个进程独占，作为文件描述符，进程退出时由操作系统自动回收。容易定位分布式系统的依赖关系，nestat检查连接对端即可，通过接受发送队列长度容易定位程序错误。

必须使用单线程的场合：程序可能会fork()（看门狗进程，并且可以fork，但是麻烦），或是**限制程序的CPU占有率**。

#### 多线程的阻抗匹配原则

在计算机中，开启多少线程比较合适，假设密集计算所占的时间比重为P，系统有C个CPU，线程池大小为C/P左右浮动，若是P<0.2不适用，因为越小倍数越大，这时取个5C差不多了。

#### 多线程思维方式

- 当前线程可能随时被切换出去。
- 多线程程序不再有全局统一的先后关系。