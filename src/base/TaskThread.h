#ifndef _TASKTHREAD_
#define _TASKTHREAD_

#include <boost/noncopyable.hpp>
#include <thread>
#include <mutex>
#include <functional>
#include <map>
#include <queue>
#include <string>
#include <sstream>

#include "EventLoop.h"
#include "Data.h"
#include "Log.h"
#include "BlockingQueue.h"

namespace base{

class TaskThread : boost::noncopyable{
    public:
        TaskThread() : que_(),
                    loop_(NULL),
                    started_(false),
                    tid(0){
            ;
        }

        void start(){
            if(started_){
                LOG_ERROR("thread has been started");
                return ;
            }

            thread_ = std::make_shared<std::thread>(std::bind(&TaskThread::run, this));

            std::ostringstream oss;
            oss << thread_->get_id();
            std::string stid = oss.str();
            tid = std::stoull(stid);

            std::string info = "tid:" + std::to_string(tid) + " start";
            LOG_INFO(info);

            thread_->detach();
            
            started_ = true;
        }

        EventLoop *get_thread_loop(){
            return loop_;
        }

    private:
        void run(){
            EventLoop loop;
            loop_ = &loop;
            loop.loop();
            loop_ = NULL;
        }

        static const int overtime = 10000;

        std::shared_ptr<std::thread> thread_;
        std::queue<int> que_;
        EventLoop *loop_;
        bool started_;
        std::map<int, std::shared_ptr<struct data>> map_;
        unsigned long long tid;
};

}

#endif