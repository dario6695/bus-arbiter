#include "BusArbiter.h"
using namespace std;

bool
BusArbiter::
isMessageFrom(message* m, const char src[])
{
    return strcmp(m->source, src) == 0;
}

void
BusArbiter::
onNotify(message* m)
{
    //error handling
    if (m == NULL)
    {
        cerr << "NULL message in notify()" << endl;
        return;
    }
        
    //ignore messages whose destination is not BUS_ARBITER_GROUP
    if (strcmp(m->dest, BUS_ARBITER_GROUP) != 0)
        return;
    
    switch (state)
    {
        case IDLE:
            if (isMessageFrom(m, CACHE_GROUP))
            {
                /*TODO:
                if is load operation
                    call onLoadOperation(), i.e. start querying caches
                else if is store operation
                    call onStoreOperation(), i.e. propagate to main memory*/
                ;
            }
            break;
            
        case QUERYING_CACHES:
                /*TODO:
                count number of hits in other caches
                if # of hits is 0
                    propagate to main memory
                else if # of hits is 1
                    read from cache
                else if # of hit is 2 or more
                    propagate to main memory
                */
            break;
            
        case WAITING_CACHE_ACK:
            if (isMessageFrom(m, CACHE_GROUP))
                onAckFromCache();
            break;
            
        case WAITING_MEMORY_ACK:
            if (isMessageFrom(m, MAIN_MEMORY_GROUP))
                onAckFromMemory();
            break;
    }
}

BusArbiter::
BusArbiter(string name, int priority) : module(name, priority)
{
    //TODO: should take a set of CPUs and a reference to main memory
    state = IDLE;
}

void
BusArbiter::
onLoadOperation()
{
    //TODO: call function to start querying caches
    state = QUERYING_CACHES;
}

void
BusArbiter::
onStoreOperation()
{
    //TODO: call function to propagate write operation to main memory
    state = WAITING_MEMORY_ACK;
}

void
BusArbiter::
onAckFromCache()
{
    //TODO: call function to transfer data to cache
    state = IDLE;
}

void
BusArbiter::
onAckFromMemory()
{
    //TODO: call function to transfer data to cache
    state = IDLE;
}

