#include <Socket.h>

#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main()
{
  Server server{ "127.0.0.1", "9999" };

  std::vector<char> bytes{};
  bytes.resize(512);

  while (true)
  {
    if (SOCKET socket{ server.Accept() })
    {
      std::cout << "Connection accepted\n";

      while (TcpSocket::Receive(socket, bytes))
      {
        std::cout << "Receiving bytes\n";

        for (char chr : bytes)
          std::cout << chr;

        TcpSocket::Send(socket, { 'b', 'a', 'r', '\n' });
        std::cout << "Sending bytes\n";
      }
    }

    std::this_thread::sleep_for(100ms);
  }

  return 0;
}