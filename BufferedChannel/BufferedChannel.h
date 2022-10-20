#ifndef BUFFEREDCHANNEL__BUFFEREDCHANNEL_H_
#define BUFFEREDCHANNEL__BUFFEREDCHANNEL_H_

#include <utility>
#include <queue>
#include <condition_variable>
#include <mutex>

template<class T>
class BufferedChannel {
 public:
  explicit BufferedChannel(int bufferSize_) : bufferSize(bufferSize_) {}

  void Send(T value) {
    checkStateOfQueue();
    if (!channelIsOpen) {
      throw std::runtime_error("Channel is closed.");
    }
    if (!full && channelIsOpen) {
      std::lock_guard<std::mutex> locker(lockSave);
      valueQueue.push(value);
      if (valueQueue.size() < bufferSize && channelIsOpen) {
        full = false;
      }
    } else {
      std::unique_lock<std::mutex> locker(lockSave);
      queueSave.wait(locker);
      if (!channelIsOpen) {
        throw std::runtime_error("Channel is closed.");
      }
      valueQueue.push(value);
    }
    queueRecv.notify_one();
  }

  std::pair<T, bool> Recv() {
    checkStateOfQueue();
    if (empty && !channelIsOpen) {
      return std::pair(T(), false);
    }
    if (!empty) {
      std::lock_guard<std::mutex> locker(lockRecv);
      T value = valueQueue.front();
      valueQueue.pop();
      if (valueQueue.size() == 0) empty = true;
      queueSave.notify_one();
      return std::pair(value, true);
    } else {
      std::unique_lock<std::mutex> locker(lockRecv);
      queueRecv.wait(locker);
      if (!channelIsOpen) {
        return {T(), false};
      }
      T value = valueQueue.front();
      valueQueue.pop();
      return std::pair(value, true);
    }

  }

  void Close() {
    channelIsOpen = false;
    queueRecv.notify_all();
    queueSave.notify_all();
  }

 private:
  void checkStateOfQueue() {
    full = valueQueue.size() == bufferSize;
    empty = valueQueue.empty();
  }
  const int bufferSize;
  std::queue<T> valueQueue;
  std::condition_variable queueSave;
  std::condition_variable queueRecv;
  std::mutex lockSave;
  std::mutex lockRecv;
  bool emptyQueue;
  bool full;
  bool empty;
  bool channelIsOpen = true;
};

#endif //BUFFEREDCHANNEL__BUFFEREDCHANNEL_H_
