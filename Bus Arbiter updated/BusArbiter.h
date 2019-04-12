#include "./orchestrator/module.h"

#include <iostream>
#include <cstring>

#define MAIN_MEMORY_GROUP "ram"
#define CACHE_GROUP "cache"
#define CPUs_NUMBER 10

class BusArbiter : public module
{
    enum BusArbiterState
    {
        IDLE,
        QUERYING_CACHES,
        WAITING_CACHE_ACK,
        WAITING_MEMORY_ACK
    } state;
    
	struct BusArbiterMagicStruct
	{	
		int cacheID; 
		/* 
			this field is useful to know from/to which caches
			the message is received/addressed 
		*/
		int type; 
		/* 
		   0: LOAD
		   1: STORE
		   2: ACK
		*/
		unsigned short int address;
		int queryResult;
		/*
		   0: HIT
		   1: MISS
		*/
	};
	
	
	
	message* currentRequest;
	int cacheIDInitiator;
	unsigned short int requestedAddress;
	
	int cacheIDSupplier;
	int currentCPUQueried;
	bool hitAlreadyReceived = false;
	
	
    //TODO: these functions need types for parameters and return value
    void onAckFromCache();
    void onAckFromMemory();
	void readFromCache();
    void writeToCache(); //???
	
	
	void onLoadOperation();
    void passThroughMemory();
	void queryCache();
    void propagateOperation();
    
    //utility functions
    bool isMessageFrom(message* m, const char src[]);
    
public:
    BusArbiter(string name, int priority = 0);
    
    void onNotify(message* m);
};

