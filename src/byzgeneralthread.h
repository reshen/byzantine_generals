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

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <vector>

class byzgeneralthread
{
    public:
        // TODO 
/*
 *        struct bGeneralIdentifier
 *        {
 *            enum rankType {
 *                RT_COMMANDING_GENERAL = 0,
 *                RT_LIEUTENANT_GENERAL,
 *
 *                RT_COUNT,
 *                RT_INVALID = RT_COUNT
 *            };
 *
 *            bGeneralIdentifier() 
 *                : uniqueID(0)
 *                , rank(RT_INVALID)
 *            {};
 *
 *            unsigned int uniqueID;
 *            rankType rank;
 *        };
 */

        /** 
         * @brief  Constructor
         * // TODO 
         */
        byzgeneralthread(const char* sharedMemName, unsigned int uniqueID, bool isLoyal, unsigned int totalNumGenerals);

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

        void sendOrder(unsigned int destinationID, order *orderToSend);
        order *recvOrder();
};

#endif // BYZGENERALTHREAD_H
