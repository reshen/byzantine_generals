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
#include "sharedmemtypes.h"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

/* Return values (Error codes) */
#define NO_ERROR            (0)
#define ERR_INVALID_INPUT   (1)

/* Shared memory areas' names */
const char* sharedMemName = "MySharedMemory";
const char* byzGeneralsNamesVector = "Generals Names";

int main(int argc, char** argv)
{
    using namespace boost::interprocess;
    using std::cout;
    using std::endl;

    /* Safety check input */
    if (argc < 3)
    {
        cout << "Invalid usage." << endl;
        cout << "bgenerals.exe [total number of generals] [traitor generals]" << endl;
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
    cout << "Total number of generals (n): " << totalNumGenerals << endl;
    cout << "Total number of traitor generals (t): " << traitorGenerals << endl;

    cout << endl;

    if ((3 * traitorGenerals + 1) <= totalNumGenerals)
    {
        cout << "This problem is solvable." << endl;
    }
    else
    {
        cout << "This problem is NOT solvable." << endl << "We need at least (3t + 1) total generals, where t is the number of traitor generals in order to sove this problem!" << endl << "Exiting..." << endl;

        return ERR_INVALID_INPUT;
    }


    /* -------------------------------------------------------------------------------- */
    /* Create shared memory inboxes */
    /* -------------------------------------------------------------------------------- */

    shared_memory_object::remove(sharedMemName);

    // TODO remove previous string vector / order vector

    try
    {
        //Create allocators
        managed_shared_memory segment(create_only, sharedMemName, 65536);
        shmCharAllocator charallocator(segment.get_segment_manager());
        shmStringAllocator stringallocator(segment.get_segment_manager());
        shmOrderAllocator orderallocator(segment.get_segment_manager());

        //Create a vector of strings in shared memory.
        shmStringVector *namesVector = 
            segment.construct<shmStringVector>(byzGeneralsNamesVector)(stringallocator);

        /* Create a thread representing each byzantine general */
        byzgeneralthread *ltGenerals[totalNumGenerals];

        for (int i = 0; i < totalNumGenerals; i++)
        {
            shmString byzGeneralName(charallocator);
            byzGeneralName = (string("Byzantine General #").append(boost::lexical_cast<string>(i))).c_str();
            namesVector->push_back(byzGeneralName);

            /* Construct a message queue for this general */
            shmOrderDeque *orderDeque = segment.construct<shmOrderDeque>(byzGeneralName.c_str())(orderallocator); 

            if (!(strcmp(byzGeneralName.c_str(), "Byzantine General #2")))
            {
                order temp("1231", charallocator);
                orderDeque->push_back(temp);
            }

            cout << "orderDeque->size() " << orderDeque->size() << endl;

        }

        for (int i = 0; i < totalNumGenerals; i++)
        {
            bool isLoyal = true;

            if (i < traitorGenerals)
            {
                /* Set traitor general loyalty */
                isLoyal = false;
            }

            /* Spin off general threads. */
            ltGenerals[i] = new byzgeneralthread(sharedMemName, byzGeneralsNamesVector, i, isLoyal, totalNumGenerals);
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
    }
    catch(...)
    {
        cout << "Exception..." << endl;
    }

    shared_memory_object::remove(sharedMemName);

    return NO_ERROR;
}
