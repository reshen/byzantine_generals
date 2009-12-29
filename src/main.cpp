/* 
 * Entry point for byzantine generals coding sample.  This simple program
 * demonstrates the Byzantine General's Problem in which multiple generals 
 * of the Byzantine Empire's army need to coordinate to attack or retreat from 
 * a city and they must be able to detect traitors in their midst.  For a good 
 * synopsis of the problem, see Wikipedia 
 * [http://en.wikipedia.org/wiki/Byzantine_generals].
 *
 * Author: Reshen Amin <reshen@zensrc.com>
 *
 * Copyright 2009 Reshen Amin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "byzgeneralthread.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/node_allocator.hpp>
#include <boost/interprocess/containers/map.hpp>

#include <iostream>

/* Return values (Error codes) */
#define NO_ERROR            (0)
#define ERR_INVALID_INPUT   (1)

/* Shared memory area name */
const char* sharedMemName = "MySharedMemory";

int main(int argc, char** argv)
{
    using namespace boost::interprocess;

    /* Safety check input */
    if (argc < 3)
    {
        std::cout << "Invalid usage." << std::endl;
        std::cout << "bgenerals.exe [total number of generals] [traitor generals]" << std::endl;
        return ERR_INVALID_INPUT;
    }

    /* Use input from the console to determine:
     * - the number of generals (n)
     * - the number of traitor generals (t) 
     * 
     * If n >= (3t + 1), then the Generals will be able to 
     * solve the problem.
     */
    const int totalNumGenerals = atoi(argv[1]);
    const int traitorGenerals = atoi(argv[2]);
    std::cout << "Total number of generals (n): " << totalNumGenerals << std::endl;
    std::cout << "Total number of traitor generals (t): " << traitorGenerals << std::endl;

    std::cout << std::endl;

    if ((3 * traitorGenerals + 1) <= totalNumGenerals)
    {
        std::cout << "This problem is solvable." << std::endl;
    }
    else
    {
        std::cout << "This problem is NOT solvable." << std::endl << "We need at least (3t + 1) total generals, where t is the number of traitor generals in order to sove this problem!" << std::endl << "Exiting..." << std::endl;

        return ERR_INVALID_INPUT;
    }


    /* -------------------------------------------------------------------------------- */
    /* Create shared memory inboxes */
    /* -------------------------------------------------------------------------------- */

    //Shared memory front-end that is able to construct objects
    //associated with a c-string. Erase previous shared memory with the name
    //to be used and create the memory segment at the specified address and initialize resources
    shared_memory_object::remove(sharedMemName);

    try
    {
        managed_shared_memory segment (create_only ,sharedMemName ,65536);

        //Note that map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
        //so the allocator must allocate that pair.
        typedef const char* KeyType;
        typedef order* MappedType;
        typedef std::pair<KeyType, MappedType> ValueType;

        //Alias an STL compatible allocator of for the map.
        //This allocator will allow to place containers
        //in managed shared memory segments
        typedef allocator<ValueType, managed_shared_memory::segment_manager> 
            ShmemAllocator;

        //Alias a map of ints that uses the previous STL-like allocator.
        //Note that the third parameter argument is the ordering function
        //of the map, just like with std::map, used to compare the keys.
        typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;

        //Initialize the shared memory STL-compatible allocator
        ShmemAllocator alloc_inst (segment.get_segment_manager());

        //Construct a shared memory map.
        //Note that the first parameter is the comparison function,
        //and the second one the allocator.
        //This the same signature as std::map's constructor taking an allocator
        MyMap *mymap = 
            segment.construct<MyMap>("MyMap")      //object name
            (std::less<KeyType>() //first  ctor parameter
             ,alloc_inst);     //second ctor parameter
        

        const char* foo = "Hello world";

        mymap->insert(std::pair<KeyType, MappedType>(foo, new order()));

    }
    catch(...)
    {
        shared_memory_object::remove(sharedMemName);
        throw;
    }
    shared_memory_object::remove(sharedMemName);



    /* -------------------------------------------------------------------------------- */
    /* END */
    /* -------------------------------------------------------------------------------- */



    /* Create a thread representing each byzantine general */
    byzgeneralthread *ltGenerals[totalNumGenerals];

    for (int i = 0; i < totalNumGenerals; i++)
    {
        bool isLoyal = true;

        if (i < traitorGenerals)
        {
            /* Set traitor general loyalty */
            isLoyal = false;
        }

        /* Spin off general threads. */
        ltGenerals[i] = new byzgeneralthread(sharedMemName, i, isLoyal, totalNumGenerals);
        ltGenerals[i]->start();
    }

    sleep(3);

    for (int i = 0; i < totalNumGenerals; i++)
    {
        /* Shut down general threads. */
        ltGenerals[i]->stop();
        delete ltGenerals[i];
        ltGenerals[i] = NULL;
    }
    return NO_ERROR;
}
