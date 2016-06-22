#ifndef _SAFE_QUEUE
#define _SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <boost/thread.hpp>

template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  T dequeue(const int timeout_ms)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty()){
	boost::this_thread::interruption_point(); 
     	c.wait_for(lock,std::chrono::milliseconds(timeout_ms));
    }
    T val = q.front();
    q.pop();
    return val;
  }

  size_t size(void){
    	std::lock_guard<std::mutex> lock(m);
	int sz =  q.size();
	return sz;
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};
#endif
