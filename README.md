#bnet(billow-net or boost-net)

基于boost.asio封装的一个网络库，有以下特性：

* 支持多线程，主线程accept，通过round-robin方式分发到线程池

* 支持自定义协议解析器

* 发送/接收缓冲
