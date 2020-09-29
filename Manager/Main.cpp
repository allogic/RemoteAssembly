#include <Socket.h>

#include <iostream>
#include <filesystem>
#include <string>
#include <thread>

using namespace std::chrono_literals;

bool FileChanged(std::string const& fileName)
{
  if (!std::filesystem::exists(fileName))
    return false;

  auto        currTime{ std::filesystem::last_write_time(fileName) };
  static auto prevTime{ currTime };

  if (prevTime < currTime)
  {
    prevTime = currTime;
    return true;
  }

  return false;
}

int main()
{
  Client client{ "127.0.0.1", "9999" };

  std::string       fileName{ "C:\\Users\\Michael\\source\\repos\\RemoteAssembly\\x64\\Debug\\Payload.dll" };

  std::vector<char> bytes{};
  bytes.resize(512);

  while (true)
  {
    if (FileChanged(fileName))
    {
      std::cout << "Connection established\n";

      client.BeginTransaction();

      TcpSocket::Send(client.mSocket, { 'f', 'o', 'o', '\n' });
      std::cout << "Sending bytes\n";

      client.EndTransaction();

      while (TcpSocket::Receive(client.mSocket, bytes))
      {
        std::cout << "Receiving bytes\n";

        for (char chr : bytes)
          std::cout << chr;
      }
    }

    std::this_thread::sleep_for(100ms);
  }

  return 0;
}