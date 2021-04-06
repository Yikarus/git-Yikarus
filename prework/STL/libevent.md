# libevent

## 多路复用

### select

- 跨平台
- 每次都要从用户空间拷贝到内核空间
- 需要便利所有fd_set
- 最大数量收到FD_SETSIZE限制

### poll

类似select，没有长度限制

### epoll

- 内核态使用，以红黑树处理
- 共享内存交互，用mmap
- 不需要每次全部复制，返回活跃的双向链表
- 有电平触发和边缘触发之分
- windows不支持

### iocp

类似epoll，但是linux不支持，使用线程池，线程多的时候比epoll好，类似reactor模式，猜的。

libevent默认实现多路复用优先级epoll>poll>select>iocp。

