#include "BusArbiter.h"
using namespace std;

BusArbiter::
BusArbiter()
{
    //TODO: should take a set of CPUs and a reference to main memory
    state = IDLE;
}

void
BusArbiter::
onLoadOperation()
{
    state = QUERYING_CACHES;
    //TODO: start querying caches
}

void
BusArbiter::
onStoreOperation()
{
    state = WAITING_MEMORY_ACK;
    //TODO: propagate write operation to main memory
}

void
BusArbiter::
onAckFromCache()
{
    state = IDLE;
    //TODO: transfer data to cache
}

void
BusArbiter::
onAckFromMemory()
{
    state = IDLE;
    //TODO: transfer data to cache
}

