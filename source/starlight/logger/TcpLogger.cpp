
#include "starlight/logger/TcpLogger.hpp"


#define IP      "10.4.1.157"
#define PORT    6969

namespace starlight {
    int g_tcpSocket;
    bool g_loggerInit = false;


    std::list<char*>* g_msgQueue = nullptr;

    void SendRaw(void* data, size_t size)
    {
        nn::socket::Send(g_tcpSocket, data, size, 0);
    }

    void AddToQueue(char* data)
    {
        if(!g_msgQueue)
            g_msgQueue = new std::list<char*>();

        g_msgQueue->push_back(data);
    }

    void TcpLogger::ClearQueue()
    {
        if(!g_msgQueue)
            return;

        while (!g_msgQueue->empty())
        {
            auto data = g_msgQueue->front();
            SendRaw(data, strlen(data));
            delete[] data;
            g_msgQueue->pop_front();
        }
    }

    void TcpLogger::Initialize()
    {
        if (g_loggerInit)
            return;

        struct sockaddr_in serverAddr;
        g_tcpSocket = nn::socket::Socket(AF_INET, SOCK_STREAM, 0);
        if(g_tcpSocket & 0x80000000)
            return;

        serverAddr.sin_addr.s_addr = inet_addr(IP);
        serverAddr.sin_family      = AF_INET;
        serverAddr.sin_port        = nn::socket::InetHtons(PORT);

        int rval = nn::socket::Connect(g_tcpSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (rval < 0)
            return;

        g_loggerInit = true;
        
        TcpLogger::Log("socket initialized!\n");
        TcpLogger::ClearQueue();
    }



    void TcpLogger::Log(const char* data, size_t size)
    {
        //Initialize();

        if (size == UINT32_MAX)
            size = strlen(data);

        char* ptr = new char[size+1];
        memset(ptr, 0, size+1);
        memcpy(ptr, data, size);

        if (!g_loggerInit)
        {
            AddToQueue(ptr);
            return;
        }

        SendRaw(ptr, size);
        delete[] ptr;
    }

    void TcpLogger::Log(std::string str)
    {
        TcpLogger::Log(str.data(), str.size());
    }

    void TcpLogger::LogFormat(const char* format, ...)
    {
        va_list args;
        char buff[0x1000] = {0};
        va_start(args, format);

        int len = vsnprintf(buff, sizeof(buff), format, args);
        
        TcpLogger::Log(buff, len);
        
        va_end (args);
    }
};