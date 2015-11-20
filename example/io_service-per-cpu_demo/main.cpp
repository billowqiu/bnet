#include <gflags/gflags.h>
#include <glog/logging.h>
#include "server.h"
#include <bnet/asyncprocessor.h>

DEFINE_int32(svr_port, 8611, "svr listen host port");
DEFINE_int32(svr_thread_num, 8, "svr io thread");

int main(int argc, char **argv)
{
    gflags::ParseCommandLineFlags(&argc, (char***)&argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO, "./log/info.log");
    google::SetLogDestination(google::ERROR, "./log/error.log");

    bnet::AsyncProcessor processor;
    EchoServer server(&processor, FLAGS_svr_port);
    server.SetThreadNum(FLAGS_svr_thread_num);
    server.Start();
    processor.Run();

    return 0;
}

