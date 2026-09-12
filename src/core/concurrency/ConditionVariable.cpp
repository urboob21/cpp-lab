// -----------------------------------------------------------------------------
// std::condition_variable
//
// Lets a thread sleep until another thread signals that some condition has
// become true, without busy-waiting.
//
//   std::unique_lock lock(mutex);
//   cv.wait(lock, [] { return ready; });   // sleep until ready == true
//
// Rules:
//   - Always wait with a predicate: wake-ups can be spurious, and a notify
//     sent before the wait started would otherwise be lost.
//   - Change the shared state while holding the mutex, then notify.
//   - wait() needs std::unique_lock because it unlocks/relocks the mutex.
//
// Reference: https://en.cppreference.com/w/cpp/thread/condition_variable
// -----------------------------------------------------------------------------

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace handshake {

/// Everything both threads share, protected by `mutex`.
struct Channel {
  std::mutex mutex;
  std::condition_variable cv;
  std::string data;
  bool ready{false};
  bool processed{false};
};

void worker(Channel& channel) {
  std::unique_lock<std::mutex> lock(channel.mutex);
  LOG("worker: waiting for data");

  // wait() atomically unlocks the mutex and sleeps. When woken up it locks the
  // mutex again and re-checks the predicate before returning.
  channel.cv.wait(lock, [&channel] { return channel.ready; });

  LOG("worker: processing data");
  channel.data += " -> processed";
  channel.processed = true;

  lock.unlock();  // unlock first so the woken thread can take the mutex at once
  channel.cv.notify_one();
}

void run() {
  LOG_SECTION("Handshake between two threads");
  Channel channel;
  std::thread worker_thread(worker, std::ref(channel));

  {
    const std::lock_guard<std::mutex> lock(channel.mutex);
    channel.data = "request";
    channel.ready = true;
  }
  LOG("main: data is ready, notify_one()");
  channel.cv.notify_one();

  {
    std::unique_lock<std::mutex> lock(channel.mutex);
    channel.cv.wait(lock, [&channel] { return channel.processed; });
    LOG_S("main: received '" << channel.data << "'");
  }
  worker_thread.join();
}

}  // namespace handshake

namespace producer_consumer {

/// A minimal thread-safe queue: pop() blocks until an item arrives or the
/// queue is closed.
class BlockingQueue {
 public:
  void push(int value) {
    {
      const std::lock_guard<std::mutex> lock(mutex_);
      items_.push_back(value);
    }
    cv_.notify_one();  // wake one waiting consumer
  }

  void close() {
    {
      const std::lock_guard<std::mutex> lock(mutex_);
      closed_ = true;
    }
    cv_.notify_all();  // wake every consumer so they can finish
  }

  /// Returns std::nullopt once the queue is closed and empty.
  std::optional<int> pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return closed_ || !items_.empty(); });
    if (items_.empty()) {
      return std::nullopt;
    }
    const int value = items_.front();
    items_.pop_front();
    return value;
  }

 private:
  std::mutex mutex_;
  std::condition_variable cv_;
  std::deque<int> items_;
  bool closed_{false};
};

void run() {
  LOG_SECTION("Producer / consumer with a blocking queue");
  BlockingQueue queue;

  auto consume = [&queue](int id) {
    while (const std::optional<int> item = queue.pop()) {
      LOG_S("consumer " << id << " got item " << *item);
    }
    LOG_S("consumer " << id << " sees a closed queue and stops");
  };

  std::vector<std::thread> consumers;
  consumers.emplace_back(consume, 1);
  consumers.emplace_back(consume, 2);

  for (int item = 1; item <= 6; ++item) {
    queue.push(item);
  }
  queue.close();

  for (std::thread& consumer : consumers) {
    consumer.join();
  }
}

}  // namespace producer_consumer

}  // namespace

LAB_EXAMPLE("ConditionVariable",
            "wait/notify handshake and a blocking producer/consumer queue") {
  handshake::run();
  producer_consumer::run();
}
