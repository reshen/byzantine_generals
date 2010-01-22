#include "byzgeneralthread.h"
#include "order.h"
#include "sharedmemtypes.h"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include <iostream>
#include <string>

byzgeneralthread::byzgeneralthread(const char* sharedMemName, const char* byzGeneralsNamesVector, unsigned int uniqueID, bool isLoyal, unsigned int totalNumGenerals):
    stopRequested_(false),
    stopFinished_(false)
{
    using namespace boost::interprocess;
    using std::cout;
    using std::endl;

    uniqueID_ = uniqueID;
    isLoyal_ = isLoyal;
    totalNumGenerals_ = totalNumGenerals;

    try
    {
        //Create allocators
        managed_shared_memory segment(open_only, sharedMemName);
        shmCharAllocator charallocator(segment.get_segment_manager());
        shmStringAllocator stringallocator(segment.get_segment_manager());

        shmStringVector *namesVector = 
            segment.find<shmStringVector>(byzGeneralsNamesVector).first;

        orderDeque_ = 
            segment.find<shmOrderDeque>(((*namesVector)[uniqueID_]).c_str()).first; 

        cout << "Unique ID : " << uniqueID_ << " / " << (*namesVector)[uniqueID_] <<  " orders? " << orderDeque_->size() << " &orderDeque : " << &orderDeque_ << endl; 
    }
    catch(interprocess_exception& e)
    {
        cout << "Interprocess Exception: " << e.what() << endl;
    }
    catch(...)
    {
        cout << "Exception" <<  endl;
    }
}

byzgeneralthread::~byzgeneralthread()
{
    if (stopFinished_ == false)
    {
        /* Make sure we stop upon destruction */
        stop();
    }
}

void byzgeneralthread::start() 
{
    using std::string;
    using std::cout;
    using std::endl;

    string loyal = isLoyal_ ? "Loyal" : "Traitor";

    cout << uniqueID_ << " " << loyal << " byzgeneralthread start()" << endl;

    assert(!thread_);
    //thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&byzgeneralthread::run, this)));

    boost::thread deep_thought_2(&byzgeneralthread::run, boost::ref(orderDeque_));

}

void byzgeneralthread::stop()
{
    using std::cout;
    using std::endl;

    cout << uniqueID_ << " byzgeneralthread stop()" << endl;

    assert(thread_);

    stopRequested_ = true;
    thread_->join();
    stopFinished_ = true;
}

void byzgeneralthread::run(shmOrderDeque *tmp)
{
    using std::cout;
    using std::endl;

    cout << "Unique ID : " << uniqueID_ << " loyal? " << isLoyal_ <<  "&orderDeque : " << &orderDeque_ << endl;

    cout << "Unique ID : " << uniqueID_ << " orders? " << tmp->size() << endl; 

    while (!stopRequested_)
    {
        /*
         *while (orderDeque_->size() > 0)
         *{
         *    [> We've received orders, lets process them now <]
         *    cout << "Unique ID : " << uniqueID_ << " / Popping : " << (*orderDeque_)[0].communicationPath_ << endl; 
         *    orderDeque_->pop_front();
         *}
         */
#if 0
        wait for orders
            - pull order out of inbox, place received order in my tree

        append my id, broadcast order to everyone else

        check if order tree is complete
            - true: propogate decision up
            - false: start over


        //cout << "hello world" << endl;

        if (uniqueID_ == RANK_COMMANDING_GENERAL)
        {
            /* Send out initial order to each Lieutenant General */

            sendOrder(1, new order());
        }
        else
        {
            /* Wait for orders from Commanding General */
            delete recvOrder();

            /* Relay orders to other Lieutenant Genearls */

            /* After all generals orders have been received, 
             * decide what action to take. */
            // TODO timeout, for faulty communication lines
            // TODO forging a command order
        }

#endif 
        /*  Wait for orders from all generals before making a decision */

        sleep(1);
    }
} 

