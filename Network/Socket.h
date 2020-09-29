#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <string>
#include <vector>

struct TcpSocket
{
  SOCKET    mSocket   { INVALID_SOCKET };
  addrinfo* mpAddrInfo{ nullptr };

  TcpSocket(
    std::string const& address,
    std::string const& port,
    int flags)
  {
    WSADATA wsaData{};

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    addrinfo addrHints
    {
      .ai_flags   { flags },
      .ai_family  { AF_INET },
      .ai_socktype{ SOCK_STREAM },
      .ai_protocol{ IPPROTO::IPPROTO_TCP },
    };

    getaddrinfo(address.c_str(), port.c_str(), &addrHints, &mpAddrInfo);

    SOCKET winSocket{ INVALID_SOCKET };

    winSocket = socket
    (
      mpAddrInfo->ai_family,
      mpAddrInfo->ai_socktype,
      mpAddrInfo->ai_protocol
    );
  }

  static void Send(SOCKET winSocket, std::vector<char> const& bytes)
  {
    send
    (
      winSocket,
      bytes.data(),
      static_cast<int>(bytes.size()),
      0
    );
  }
  static bool Receive(SOCKET winSocket, std::vector<char>& bytes)
  {
    return recv
    (
      winSocket,
      bytes.data(),
      static_cast<int>(bytes.size()),
      0
    );
  }
};

struct Server
  : TcpSocket
{
  Server(
    std::string const& address,
    std::string const& port)
    : TcpSocket(address, port, AI_PASSIVE)
  {
    Bind();
    Listen();
  }

  void   Bind()
  {
    bind
    (
      mSocket,
      mpAddrInfo->ai_addr,
      static_cast<int>(mpAddrInfo->ai_addrlen)
    );
  }
  void   Listen()
  {
    listen
    (
      mSocket,
      SOMAXCONN
    );
  }
  SOCKET Accept()
  {
    return accept
    (
      mSocket,
      nullptr,
      nullptr
    );
  }
};

struct Client
  : TcpSocket
{
  Client(
    std::string const& address,
    std::string const& port)
    : TcpSocket(address, port, 0) {}

  void BeginTransaction()
  {
    connect
    (
      mSocket,
      mpAddrInfo->ai_addr,
      static_cast<int>(mpAddrInfo->ai_addrlen)
    );
  }
  void EndTransaction()
  {
    shutdown
    (
      mSocket,
      SD_SEND
    );
  }
};