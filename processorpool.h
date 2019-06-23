//
// Copyright (c) billowqiu (billowqiu@163.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See www.boost.org/libs/asio for documentation.
//

#ifndef BNET_PROCESSORPOOL_H
#define BNET_PROCESSORPOOL_H

#include <vector>
#include <boost/noncopyable.hpp>

namespace bnet
{
class AsyncProcessor;

class ProcessorPool : public boost::noncopyable
{
public:
    explicit ProcessorPool();
    ~ProcessorPool();

    void SetPoolSize(std::size_t size);
    void Start();
    void Stop();
    AsyncProcessor* GetAsyncProcessor();

private:
    std::vector<AsyncProcessor*> processors_;
    std::size_t next_processor_;
    std::size_t pool_size_;
};

}

#endif // BNET_PROCESSORPOOL_H

