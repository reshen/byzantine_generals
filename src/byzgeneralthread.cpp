#include "byzgeneralthread.h"
#include "order.h"
#include "sharedmemtypes.h"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include <iostream>
#include <string>

byzgeneralthread::byzgeneralthread(const char* sharedMemName, const char* byzGeneralsNamesVector, unsigned int uniqueID, bool isLoyal, unsigned int totalNumGenerals):
    stopRequested_(false),
    stopFinished_(false),
    segment_(NULL)
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
        segment_ = new managed_shared_memory(open_only, sharedMemName);

        // TODO
        //shmCharAllocator charallocator(segment_->get_segment_manager());
        //shmStringAllocator stringallocator(segment_->get_segment_manager());

        shmStringVector *namesVector = 
            segment_->find<shmStringVector>(byzGeneralsNamesVector).first;

        orderDeque_ = 
            segment_->find<shmOrderDeque>(((*namesVector)[uniqueID_]).c_str()).first; 

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
    assert(segment_ != NULL);

    thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&byzgeneralthread::run, this)));
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

void byzgeneralthread::run()
{
    using namespace boost::interprocess;
    using std::cout;
    using std::endl;

    cout << "Unique ID : " << uniqueID_ << " loyal? " << isLoyal_ <<  " orders size? " << orderDeque_->size() << endl; 

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

