#include "ServerPublib.h"

class CEpoll
{
public:
    bool EpollInit(int uEpollSize, char* uHost, int uPort, int uListenLog = 1024);
    bool EpollInit(int uEpollSize, int sockfd);
    bool Initialize(int uEpollSize, int sockHandle);
    void Uninit();

public:
    bool AddToEpoll(int fd, epoll_event* ev);
    bool DeleteFromEpoll(int fd);
    bool ModifyFromEpoll(int fd, epoll_event* ev);
    virtual void Routine(int uEpollTime);

protected:
    virtual void OnAccept(int hListenSock);
    virtual void OnEvent(const epoll_event* pEv);
    

private:

    epoll_event*    m_Events;
    int             m_ListenSock;
    int             m_EpollHandle;
    int             m_EvNum;
    bool            m_IsAttached;
};