#include <iostream>
#include "BufferedChannel.h"

/*long sum(const std::vector<int>& v) {
  long result = 0;
  for (auto i : v) {
    result += i;
  }
  return result;
}

void Check() {
  const int CHANNEL_SIZE = 100;
  const int TO = 1000000;
  BufferedChannel<int> channel(CHANNEL_SIZE);
  std::thread reader([&](){
    std::vector<int> values;
    for (int i = 0; i < TO; ++i) {
      values.push_back(channel.Recv().first);
      printf("%d", i);
    }
    if (TO * (TO - 1) / 2 != sum(values))
      std::cerr << "diff";
    else
      std::cerr << "same";
  });
  std::thread writer([&](){
    for (int i = 0; i < TO; ++i)
      channel.Send(i);
    channel.Close();
    std::cout << "closed" << std::endl;
  });
  reader.join();
  writer.join();
}*/

int main() {
  const int CHANNEL_SIZE = 10;
  BufferedChannel<int> channel(CHANNEL_SIZE);
  for (int i = 0; i < CHANNEL_SIZE; ++i) {
    channel.Send(i);
  }
  std::thread thread([&channel]() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    channel.Send(100);
    std::cout << "Value is sent" << std::endl;
    channel.Close();
  });
  for (int i = 0; i < 15; ++i) {
    std::pair<int, bool> value = channel.Recv();
    std::cout << value.first << " " << (int) value.second << std::endl;
  }
  thread.join();
  
  //Check();
  return 0;
}