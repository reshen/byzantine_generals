#ifndef ORDER_H
#define ORDER_H

#include "sharedmemtypes.h"

//#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

/* This class is defined a struct because it only carries plain 
 * old data (POD) and lacks any functionality other than accessing 
 * members directly. */
struct order
{
    shmString communicationPath_;

    enum orderType
    {
        OT_ATTACK = 0,
        OT_RETREAT,

        OT_COUNT,
        OT_INVALID = OT_COUNT
    };

    order(const char *communicationPath, const shmCharAllocator &charallocator): 
        communicationPath_(communicationPath, charallocator),
        initialOrder_(OT_INVALID)
    {}

    /*
     *order(const order& o):
     *    communicationPath_(o.communicationPath_),
     *    initialOrder_(o.initialOrder_)
     *{
     *}
     */

    /*
     *order():
     *    initialOrder_(),
     *    communicationPath_()
     *{
     *}
     */

    orderType initialOrder_;
    orderType finalOrder_;
};

typedef boost::interprocess::allocator<order, shmSegmentManager> shmOrderAllocator;
//typedef boost::interprocess::vector<order, shmOrderAllocator> shmOrderVector;
typedef boost::interprocess::deque<order, shmOrderAllocator> shmOrderDeque;

#endif // ORDER_H
