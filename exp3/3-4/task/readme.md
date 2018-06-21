# 利用linux的共享内存通信机制实现两个进程间的通信
## 构建:
    gcc sender.c -o sender -lpthread
    gcc receiver.c -o receiver -lpthread
## 运行
    打开两个终端，一个执行./sender运行sender，另一个执行./receiver运行receiver
    sender程序输入内容，receiver负责接收并响应
    sender输入exit结束双方的运行
## 说明:
一共设置三个信号量

    资源信号量:receiver_response和sender_write
    互斥信号量:mutex

设置三个信号量是需要的,一开始以为只要一个信号量使得一个进程可以一定时间内单独访问就行了,但是这样的话，有可能会产生分配不均衡甚至让一个进程一直饥饿的现象,于是多设置两个信号量,receiver_response 0和sender_write 1.

类似生产者消费者模型那样receiver需要wait一个receiver_response才能执行,sender需要wait一个sender_write才能执行.一开始receiver需要wait一个receiver_response才能执行，而初始值为0,sender wait了sender_write，执行完任务signal一个receiver_response.这时候sender自己不能接连再次执行任务,因为sender_write为0了,而这时就轮到receiver wait receiver_response成功而得以执行.执行完他的任务以后signal sender_write,自己也一样不能同时接连执行,于是就又轮到sender执行了.
(分析省略了共享存储区的信号量)

说了这么多，我理解这里的资源信号量其实更像是一个推动两个进程协作的工具。我这边结束了一次操作就该你来执行，你那边结束一次操作就该我来执行.不能让其中一方连着进行几次操作，操作完一次让权给对方操作.互斥信号量起到多个进程互斥访问临界资源的作用，他保证的是能够同时使用这个临界资源的数量，而这道题要保证两个进程运行的逻辑顺序，就要加上另外的两个信号量.

当然一个信号量也是可以的
适当的给sender执行一次以后sleep，receiver就有机会可以执行了，但是这个方案不是很好，
你无法控制sleep多久，时间需要在用户等待和进程切换之间衡量.