#include "./event/module.h"

#include <iostream>
#include <cstring>

#define BUS_ARBITER_GROUP "bus-arbit"
#define MAIN_MEMORY_GROUP "ram"
#define CACHE_GROUP "cache"

class BusArbiter : public module
{
    enum BusArbiterState
    {
        IDLE,
        QUERYING_CACHES,
        WAITING_CACHE_ACK,
        WAITING_MEMORY_ACK
    } state;
    
    /* TODO:
    void startQueryingCaches();
    void propagateOperation();
    void readFromCache();
    void writeToCache();
    */
    
    //TODO: these functions need types for parameters and return value
    void onLoadOperation();
    void onStoreOperation();
    void onAckFromCache();
    void onAckFromMemory();
    
    //utility functions
    bool isMessageFrom(message* m, const char src[]);
    
public:
    BusArbiter();
    
    event* notify(message* m);
};

