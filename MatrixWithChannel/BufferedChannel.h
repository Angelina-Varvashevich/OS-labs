#ifndef MATRIXWITHCHANNEL__NEWCHANNEL_H_
#define MATRIXWITHCHANNEL__NEWCHANNEL_H_
#include <queue>
#include <mutex>
#include <condition_variable>
template<typename T>
class BufferedChannel {
 public:
  BufferedChannel(int size_) : bufferSize(size_) {
    checkStateOfQueue();
  }

  void Send(T value) {
    std::unique_lock<std::mutex> sendLock(sendMutex);
    checkStateOfQueue();
    if (isClose) {
      throw std::runtime_error("Channel is closed.");
    }
    while (full && !isClose) {
      sendVariable.wait(sendLock);
    }
    if (isClose) {
      throw std::runtime_error("Channel is closed.");
    }
    changeMutex.lock();
    queue.push(std::move(value));
    changeMutex.unlock();
    checkStateOfQueue();
    recvVariable.notify_one();
  }

  std::pair<T, bool> Recv() {
    std::unique_lock<std::mutex> recvLock(recvMutex);
    checkStateOfQueue();
    while (empty && !isClose) {
      recvVariable.wait(recvLock);
    }
    if (isClose && empty) {
      recvVariable.notify_all();
      return std::pair<T, bool>(T(), false);
    }
    changeMutex.lock();
    T value = std::move(queue.front());
    queue.pop();
    changeMutex.unlock();
    checkStateOfQueue();
    sendVariable.notify_one();
    return std::pair<T, bool>(value, true);
  }

  void Close() {
    isClose = true;
    recvVariable.notify_all();
    sendVariable.notify_all();
  }

 private:
  void checkStateOfQueue() {
    full = queue.size() == bufferSize;
    empty = queue.empty();
  }
  const int bufferSize;
  std::queue<T> queue;
  std::mutex sendMutex;
  std::mutex recvMutex;
  std::mutex changeMutex;
  std::condition_variable sendVariable;
  std::condition_variable recvVariable;
  bool full;
  bool empty;
  bool isClose = false;
};

#endif //MATRIXWITHCHANNEL__NEWCHANNEL_H_
