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
    if (strcmp(m->dest, getName().c_str()) != 0)
        return;
		
	//create a pointer to our magic struct
	BusArbiterMagicStruct* magicStructPtr;
	magicStructPtr = (BusArbiterMagicStruct)m->magic_struct; 	
    
    switch (state)
    {
        case IDLE:
            if (isMessageFrom(m, CACHE_GROUP))
            {
				if(magicStructPtr->type == 0)
				{
					//we intercepted a load request from the  cache
					currentRequest = m;
					cacheIDInitiator = magicStructPtr->cacheID;
					requestedAddress = magicStructPtr->address;
					
					currentCPUQueried = (cacheIDInitiator + 1) % CPUs_NUMBER;
					
					onLoadOperation();
				}
				else if(magicStructPtr->type == 1)
				{
					//we intercepted a store request from the  cache
					currentRequest = m;
					cacheIDInitiator = magicStructPtr->cacheID;
					requestedAddress = magicStructPtr->address;
					
					passThroughMemory();
				}
            }
            break;
            
        case QUERYING_CACHES:
            if (isMessageFrom(m, CACHE_GROUP) && magicStructPtr->cacheID == currentCPUQueried)
			{    
				if(magicStructPtr->queryResult == 0)
				{	
					//the current cache queried answer with a HIT
					if(hitAlreadyReceived)
					{
						passThroughMemory();
						hitAlreadyReceived = false;
						return;
						
					}
					
					hitAlreadyReceived = true;
					cacheIDSupplier = currentCPUQueried;
				}
				
				currentCPUQueried = (currentCPUQueried + 1) % CPUs_NUMBER;
				
				if(currentCPUQueried == cacheIDInitiator)
				{
					//we have finished the query cycle
					if(hitAlreadyReceived)
					{
						readFromCache();
						hitAlreadyReceived = false;
					}
					else
					{
						passThroughMemory();
					}
				}
				else
				{
					queryCache();
				}			
			}	
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
    state = QUERYING_CACHES;
	queryCache();
}

void
BusArbiter::
passThroughMemory()
{
	state = WAITING_MEMORY_ACK;
	propagateOperation();
}


void 
BusArbiter::
propagateOperation()
{
	message* writeThroughMsg = currentRequest;
	writeThroughMsg->timestamp = getTime();
	strcpy(writeThroughMsg->source, getName().c_str());
	strcpy(writeThroughMsg->dest, MAIN_MEMORY_GROUP);
	
	sendWithDelay(writeThroughMsg, 10);
}

void
BusArbiter::
queryCache()
{
	message* queryMsg = new message();
	queryMsg->valid = 1;
	queryMsg->timestamp = getTime();
	strcpy(queryMsg->source, getName().c_str());
	strcpy(queryMsg->dest, CACHE_GROUP);
	queryMsg->magic_struct = (void)new busArbiterMagicStruct();
	queryMsg->magic_struct->cacheID = currentCPUQueried;
	queryMsg->magic_struct->address = requestedAddress;
				
	sendWithDelay(queryMsg, 10);
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

void
BusArbiter::
readFromCache()
{
    
}

void
BusArbiter::
writeToCache()
{
    // ???
}


