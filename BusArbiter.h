#include "./event/module.h"

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
    
public:
    BusArbiter();
    
    void onLoadOperation();
    void onStoreOperation();
    void onAckFromCache();
    void onAckFromMemory();
    
    event* notify(message* m);
};

