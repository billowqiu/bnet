#include "bnet/asyncprocessor.h"
#include <boost/thread.hpp>

namespace bnet
{

AsyncProcessor::AsyncProcessor():
emptywork_(service_),
next_timerid_(0)
{

}

AsyncProcessor::~AsyncProcessor()
{

}

void AsyncProcessor::Initialize()
{

}

void AsyncProcessor::Finitialize()
{
    
}

void AsyncProcessor::Run()
{
    service_.run();
}

void AsyncProcessor::RunOne()
{
    service_.run_one();
}

void AsyncProcessor::Stop()
{
    service_.stop();
}

void AsyncProcessor::AddJob( const JobFunc& func )
{
    service_.post(func);
}

bool AsyncProcessor::AddTimerJob(uint32_t msec, const TimerJobFunc& func, uint16_t& id)
{
    next_timerid_++;
    timer_ptr timer( new boost::asio::deadline_timer(service_, boost::posix_time::milliseconds(msec)));
    
    timer->async_wait(boost::bind(&AsyncProcessor::HandleTimer, 
                                  this, 
                                  timer, 
                                  boost::posix_time::milliseconds(msec), 
                                  _1,
                                  func));
    
    std::pair<TimersMap::iterator, bool> ret = timers_.insert(std::make_pair(next_timerid_, timer));
    if (ret.second)
    {
        id = next_timerid_;
    }
    
    return ret.second;
}

bool AsyncProcessor::DelTimerJob( uint16_t id )
{
    return true;
}

void AsyncProcessor::HandleTimer(timer_ptr timer, 
                                 const boost::asio::deadline_timer::duration_type& duration, 
                                 const boost::system::error_code& ec,
                                 const TimerJobFunc& func )
{
    //执行该任务
    func(ec);
    //重新调度定时器
    timer->expires_from_now(duration);
    timer->async_wait(boost::bind(&AsyncProcessor::HandleTimer, 
                                  this, 
                                  timer, 
                                  duration, 
                                  _1,
                                  func));
}

void AsyncProcessor::SetTimer(timer_ptr timer, 
                              const boost::asio::deadline_timer::duration_type& duration, 
                              const TimerJobFunc& func )
{

    timer->async_wait(boost::bind(&AsyncProcessor::HandleTimer, 
                                  this, 
                                  timer, 
                                  duration, 
                                  _1,
                                  func));
}

}
