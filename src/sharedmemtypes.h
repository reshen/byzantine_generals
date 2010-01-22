#ifndef SHAREDMEMTYPES_H
#define SHAREDMEMTYPES_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

//Typedefs
typedef boost::interprocess::managed_shared_memory::segment_manager 
    shmSegmentManager;
typedef boost::interprocess::allocator<char, shmSegmentManager> 
    shmCharAllocator;
typedef boost::interprocess::basic_string<char, std::char_traits<char>, shmCharAllocator> 
    shmString;
typedef boost::interprocess::allocator<shmString, shmSegmentManager> 
    shmStringAllocator;
typedef boost::interprocess::vector<shmString, shmStringAllocator> 
    shmStringVector;

/* pair < communication path = string, [attack = true || retreat = false] > */
/*
 *typedef std::pair<shmString, bool> 
 *    orderMessage;
 */

//Alias a map of ints that uses the previous STL-like allocator.
//Note that the third parameter argument is the ordering function
//of the map, just like with std::map, used to compare the keys.
//typedef boost::interprocess::map<shmString, order, std::less<shmString>, shmStringAllocator> shmOrderMap;

// TODO 

#endif // SHAREDMEMTYPES_H
