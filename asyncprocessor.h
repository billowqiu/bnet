//
// Copyright (c) billowqiu (billowqiu@163.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See www.boost.org/libs/asio for documentation.
//

#ifndef BNET_ASYNCPROCESSOR_H_
#define BNET_ASYNCPROCESSOR_H_

#include <stdint.h>
#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>

typedef boost::function<void ()> JobFunc;
typedef boost::function<void (const boost::system::error_code& ec)> TimerJobFunc;

namespace bnet
{

class AsyncProcessor : public boost::noncopyable
{
    typedef boost::shared_ptr<boost::asio::deadline_timer> timer_ptr;
public:
    AsyncProcessor();
    ~AsyncProcessor();
    /** 
     *  初始化异步处理框架线程数
     *  @param[in] io_thread_num，
     *             0表示直接在主线程中开启eventloop，
     *                              
     *  @return void.
    */
    void Initialize();
    void Finitialize();
    
    void AddJob(const JobFunc& func);
    bool AddTimerJob(uint32_t msec, const TimerJobFunc& func, uint16_t& id);    
    bool DelTimerJob(uint16_t id);
    
    boost::asio::io_service& Service() 
    {
        return service_; 
    }
    
    void Run();
    void RunOne();
    void Stop();
    
private:
    void SetTimer(timer_ptr timer, 
                  const boost::asio::deadline_timer::duration_type& duration, 
                  const TimerJobFunc& func);

    void HandleTimer(timer_ptr timer, 
                     const boost::asio::deadline_timer::duration_type& duration, 
                     const boost::system::error_code& ec,
                     const TimerJobFunc& func);

private:
    boost::asio::io_service service_;
    boost::asio::io_service::work emptywork_;
    uint16_t next_timerid_;
    
    typedef boost::unordered_map<uint16_t, timer_ptr> TimersMap;
    TimersMap timers_;
};

}

#endif // BNET_ASYNCPROCESSOR_H_
