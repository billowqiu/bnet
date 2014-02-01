#include <iostream>
#include "server.h"
#include <bnet/asyncprocessor.h>

int main(int argc, char **argv)
{
    bnet::AsyncProcessor processor;
    EchoServer server(&processor, 10086);
    server.SetThreadNum(4);
    server.Start();
    processor.Run();

    return 0;
}
