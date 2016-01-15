#include <net/m_socket.h>
#include <net/m_net_event_loop.h>
#include <net/m_net_connector.h>
#include <thread/m_thread.h>
#include <mutex>
#include <functional>
#include <iostream>
#include <cstring>
#include <list>

class NetThread
    :public MThread
{
public:
    NetThread() {}
    virtual ~NetThread(){}
    NetThread(const NetThread &) = delete;
    NetThread& operator=(const NetThread &) = delete;
public:
    bool Init()
    {
        if (event_loop_.Create() == MNetError::No)
        {
            return Start() == MThreadError::No;
        }
        return false;
    }
    void Close() { StopAndJoin(); event_loop_.Close(); }
    MNetEventLoop& GetEventLoop() { return event_loop_; }
    void AddCallback(const std::function<void ()> &callback)
    {
        std::lock_guard<std::mutex> lock(callback_mutex_);
        callback_list_.push_back(callback);
        event_loop_.Interrupt();
    }
private:
    virtual void DoRun()
    {
        event_loop_.ProcessEvents();
        std::list<std::function<void ()> > cb_list;
        {
            std::lock_guard<std::mutex> lock(callback_mutex_);
            cb_list.swap(callback_list_);
        }
        for (auto &cb : cb_list)
        {
            if (cb)
            {
                cb();
            }
        }
    }
private:
    MNetEventLoop event_loop_;
    std::list<std::function<void ()> > callback_list_;
    std::mutex callback_mutex_;
};

void OnRead(MNetConnector *p_connector)
{
    char tmp[11];
    if (p_connector->ReadBuf(tmp, 10) == MNetError::No)
    {
        tmp[10] = 0;
        std::cout << "recv:" << tmp << std::endl;
    }
}

void OnWrite(MNetConnector *p_connector)
{
    std::cout << p_connector << " write complete" << std::endl;
}

void WriteCallback(MNetConnector *p_connector, char *p, size_t len)
{
    p_connector->WriteBuf(p, len);
    delete p;
}

int main()
{
    NetThread th;
    if (!th.Init())
    {
        return 0;
    }
    MSocket *p_sock = new MSocket();
    if (p_sock->Create(MSocketFamily::IPV4, MSocketType::TCP, MSocketProtocol::Default) != MNetError::No)
    {
        return 0;
    }
//    if (p_sock->Bind("127.0.0.1", 3231) != MNetError::No)
//    {
//        return 0;
//    }
//    if (p_sock->SetReUseAddr(true) != MNetError::No)
//    {
//        return 0;
//    }
    if (p_sock->Connect("127.0.0.1", 3232) != MNetError::No)
    {
        return 0;
    }
    if (p_sock->SetBlock(false) != MNetError::No)
    {
        return 0;
    }
    MNetConnector *p_connector = new MNetConnector(p_sock, nullptr, &(th.GetEventLoop()), nullptr, nullptr, nullptr, true, 1024, 1024);
    p_connector->SetReadCallback(std::bind(OnRead, p_connector));
    p_connector->SetWriteCompleteCallback(std::bind(OnWrite, p_connector));
    if (p_connector->EnableReadWrite(true) != MNetError::No)
    {
        return 0;
    }

    char tmp[100];
    while (std::cin.getline(tmp, 1024))
    {
        size_t len = strlen(tmp);
        char *p = new char[len];
        memcpy(p, tmp, len);
        th.AddCallback(std::bind(WriteCallback, p_connector, p, len));
    }

    return 0;
}
