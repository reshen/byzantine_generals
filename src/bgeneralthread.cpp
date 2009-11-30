#include <iostream>

#include "bgeneralthread.h"

bgeneralthread::bgeneralthread():
    stoprequested_(false)
{
}

bgeneralthread::~bgeneralthread()
{
    /* Make sure we stop upon destruction */
    stop();
}

void bgeneralthread::start() 
{
    std::cout << "bgeneralthread start()" << std::endl;

    assert(!thread_);
    thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&bgeneralthread::run, this)));
}

void bgeneralthread::stop()
{
    std::cout << "bgeneralthread stop()" << std::endl;

    assert(thread_);
    stoprequested_ = true;
    thread_->join();
}

void bgeneralthread::run()
{
    while (!stoprequested_)
    {
        std::cout << "hello world" << std::endl;
        sleep(1);
    }
} 
