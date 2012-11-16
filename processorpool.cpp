#include "bnet/processorpool.h"
#include <exception>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "bnet/asyncprocessor.h"

namespace bnet
{

ProcessorPool::ProcessorPool():
next_processor_(0),
pool_size_(0)
{
}

ProcessorPool::~ProcessorPool()
{

}

void ProcessorPool::SetPoolSize( std::size_t size )
{
    assert(0 <= size);
    pool_size_ = size;
}

void ProcessorPool::Start()
{
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (std::size_t i = 0; i < pool_size_; ++i)
    {
        AsyncProcessor* processor = new AsyncProcessor;
        processors_.push_back(processor);
        
        boost::shared_ptr<boost::thread> thread(
            new boost::thread(boost::bind(&AsyncProcessor::Run, processor)));
        threads.push_back(thread);
    }
    
    //for (std::size_t i = 0; i < threads.size(); ++i)
    //{
    //    threads[i]->join();
    //}
}

void ProcessorPool::Stop()
{
    for (std::vector<AsyncProcessor*>::iterator it=processors_.begin(); 
         it!=processors_.end(); ++ it)
    {
        (*it)->Stop();
    }                
}

AsyncProcessor* ProcessorPool::GetAsyncProcessor()
{
    if (processors_.empty())
    {
        return  NULL;
    }
    
    //round-robin
    AsyncProcessor* processor = processors_[next_processor_];
    ++next_processor_;
    if (next_processor_ == processors_.size())
    {
        next_processor_ = 0;
    }
    return processor;
}

}


