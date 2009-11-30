#ifndef BGENERALTHREAD_H
#define BGENERALTHREAD_H

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class bgeneralthread
{
    public:
        /** 
         * @brief  Constructor
         */
        bgeneralthread(void);

        /** 
         * @brief  Destructor
         */
        ~bgeneralthread(void);

        /** 
         * @brief Request that this thread stop
         */
        void stop();

        /** 
         * @brief  Create and run this thread
         */
        void start();

    private:
        boost::shared_ptr<boost::thread> thread_;
        bool stoprequested_;
        void run();
};

#endif // BGENERALTHREAD_H
