#include "header/CEpoll.h"

bool CEpoll::EpollInit(int uEpollSize, char* uHost, int uPort, int uListenLog)
{
    int flag = 1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("ERROR: Invalid SOCKET\n");
        return false;
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    struct sockaddr_in host = { 0 };
    host.sin_port = htons(uPort);
    inet_aton(uHost, &host.sin_addr);
    host.sin_family = AF_INET;
    flag = bind(sock, (struct sockaddr*)&host, sizeof(host));
    if (flag < 0)
    {
        printf("ERROR: Bind Fail!\n");
        return false;
    }

    flag = listen(sock, uListenLog);
    if (flag < 0)
    {
        printf("ERROR: Listen Fail\n");
        return false;
    }

    return Initialize(uEpollSize, sock);
}

bool CEpoll::Initialize(int uEpollSize, int sock)
{
    m_ListenSock = sock;
    m_Events = new epoll_event[uEpollSize + 1];
    m_EvNum = uEpollSize + 1;
    int epoll = epoll_create(m_EvNum);
    m_EpollHandle = epoll;

    epoll_event ev;
    ev.data.ptr = &m_ListenSock;
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    bool b = AddToEpoll(sock, &ev);
    if (!b)
    {
        Uninit();
    }
    return b;
}

bool CEpoll::AddToEpoll(int fd, epoll_event* ev)
{
    bool flag = false;
    if (m_EpollHandle != (int)INVALID_HANDLE && fd > 0)
    {
        flag = epoll_ctl(m_EpollHandle, EPOLL_CTL_ADD, fd, ev) == 0 ? true : false;
    }
    return flag;
}

bool CEpoll::DeleteFromEpoll(int fd)
{
    bool flag = false;
    if (m_EpollHandle != (int)INVALID_HANDLE && fd > 0)
    {
        flag = epoll_ctl(m_EpollHandle, EPOLL_CTL_DEL, fd, NULL) == 0 ? true : false;
    }
    return flag;
}

bool CEpoll::ModifyFromEpoll(int fd, epoll_event* ev)
{
    bool flag = false;
    if (m_EpollHandle != (int)INVALID_HANDLE && fd > 0)
    {
        flag = epoll_ctl(m_EpollHandle, EPOLL_CTL_MOD, fd, ev) == 0 ? true : false;
    }
    return flag;
}

void CEpoll::Routine(int waitTime)
{
    int nfds = epoll_wait(m_EpollHandle, m_Events, m_EvNum, waitTime);
    for (int i = 0; i < nfds; i++)
    {
        if (m_Events[i].data.ptr == &m_ListenSock)
        {
            if (m_Events[i].events & EPOLLIN)
            {
                OnAccept(m_ListenSock);
            }
            else
            {
                
            }
        }
        else
        {
            OnEvent(&m_Events[i]);
        }
    }
}