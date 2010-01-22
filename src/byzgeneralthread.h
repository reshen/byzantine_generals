#ifndef BYZGENERALTHREAD_H
#define BYZGENERALTHREAD_H

/* <http://stackoverflow.com/questions/614302/c-header-order>
 * Order of headers should follow
 * - corresponded header file
 * - necessary project headers
 * - 3rd party libraries headers
 * - standard libraries headers
 * - system headers
 */

#include "order.h"
#include "sharedmemtypes.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

class byzgeneralthread
{
    public:

        /** 
         * @brief  Constructor
         * // TODO 
         */
        byzgeneralthread(const char* sharedMemName, const char* byzGeneralsNamesVector, unsigned int uniqueID, bool isLoyal, unsigned int totalNumGenerals);

        /** 
         * @brief  Destructor
         */
        ~byzgeneralthread(void);

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
        unsigned int uniqueID_;
        unsigned int totalNumGenerals_;
        bool stopRequested_;
        bool stopFinished_;
        bool isLoyal_;
        void run();

        boost::interprocess::managed_shared_memory *segment_;
        shmOrderDeque *orderDeque_;
};

#endif // BYZGENERALTHREAD_H
