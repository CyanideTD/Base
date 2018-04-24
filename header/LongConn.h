#ifndef _LONGCONN_H_
#define _LONGCONN_H_

#include "ServerPublib.h"

class LongConn : public CEpoll
{
public:
    virtual void Routine(int uWaitTimeusec);

};

#endif