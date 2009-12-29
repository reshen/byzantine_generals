#include <iostream>

#include "byzgeneralthread.h"
#include "order.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include <string>

// TODO pitfals of using lots of namespaces
//using namespace boost::interprocess;
//using namespace boost;
//using namespace std;

byzgeneralthread::byzgeneralthread(const char* sharedMemName, unsigned int uniqueID, bool isLoyal, unsigned int totalNumGenerals):
    stopRequested_(false),
    stopFinished_(false)
{
    using namespace boost::interprocess;

    uniqueID_ = uniqueID;
    isLoyal_ = isLoyal;
    totalNumGenerals_ = totalNumGenerals;

    //Open already created shared memory object.
    shared_memory_object shm(open_only, sharedMemName, read_write);

    //Map the second half of the memory
    const int MEMSIZE = 10000;
    mapped_region region
        ( shm                   //Memory-mappable object
          , read_write          //Access mode
          , MEMSIZE * uniqueID  //Offset from the beginning of shm
          , MEMSIZE             //Length of the region
        );


    // Check that memory was initialized to 1
    char *mem = static_cast<char*>(region.get_address());
    for(std::size_t i = 0; i < region.get_size(); ++i)
    {
        if(*mem++ != 1)
        {
            assert(!"Unitialized memory!");
        }
    }

    // Initialize all the memory to 0 for our use.
    std::memset(region.get_address(), 0, region.get_size());

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
    std::string loyal = isLoyal_ ? "Loyal" : "Traitor";

    std::cout << uniqueID_ << " " << loyal << " byzgeneralthread start()" << std::endl;

    assert(!thread_);
    thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&byzgeneralthread::run, this)));
}

void byzgeneralthread::stop()
{
    std::cout << uniqueID_ << " byzgeneralthread stop()" << std::endl;

    assert(thread_);

    stopRequested_ = true;
    thread_->join();
    stopFinished_ = true;
}

void byzgeneralthread::run()
{
    while (!stopRequested_)
    {
#if 0
        //std::cout << "hello world" << std::endl;

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

void byzgeneralthread::sendOrder(unsigned int destinationID, order *orderToSend)
{
    // TODO 
    destinationID++;
    delete orderToSend;
    orderToSend = NULL;
#if 0
    try
    {
        std::stringstream os;
        //std::ofstream os("test");
        boost::archive::text_oarchive out(os);

        out << *orderToSend; 

        //cout << "out " << os.str().c_str() << " length " << os.str().length() << endl;

        const char *data = os.str().c_str();
        //int len = os.str().length() * sizeof(char);
        int len = strlen(data) * sizeof(char);

        cout << "out " << data << " length " << len << endl;

        communicationLines_.at(destinationID)->send(data, len, 0);

        //os.close();
    }
    catch (boost::interprocess::interprocess_exception &ex)
    {
        std::cout << ex.what() << " send code " << ex.get_error_code() << std::endl;
    }
#endif
}

order *byzgeneralthread::recvOrder()
{
    order *receivedOrder = new order();

#if 0
    try
    {
        unsigned int priority = 0;
        std::size_t recvd_size = 0;

        // TODO 
        char *data = (char *)calloc(sizeof(char), 1024);

        // TODO 
        communicationLines_.at(1)->receive(data, sizeof(char) * 1024, recvd_size, priority);

        std::stringstream is(data);
        boost::archive::text_iarchive in(is);
        in >> *receivedOrder; 

        cout << "Receieved order " << receivedOrder->initialOrder() << endl;

        free(data);
    }
    catch (boost::interprocess::interprocess_exception &ex)
    {
        std::cout << ex.what() << "recv code " << ex.get_error_code() << std::endl;
    }
#endif

    return receivedOrder;
}

