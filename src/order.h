#ifndef ORDER_H
#define ORDER_H

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <vector>

/* This class is defined a struct because it only carries plain 
 * old data (POD) and lacks any functionality other than accessing 
 * members directly. */
struct order
{
    enum orderType {
        OT_ATTACK = 0,
        OT_RETREAT,

        OT_COUNT,
        OT_INVALID = OT_COUNT
    };

    order();

    //Mutex to protect access to the queue
    boost::interprocess::interprocess_mutex mutex;

    orderType initialOrder_;

    std::vector<int> communicationPath_;
};

#endif // ORDER_H
