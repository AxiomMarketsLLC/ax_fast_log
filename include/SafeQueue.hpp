#ifndef _SAFE_QUEUE
#define _SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
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
    boost::lock_guard<boost::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  T dequeue()
  {
    boost::unique_lock<boost::mutex> lock(m);
    
    while(q.empty()){
          c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

  size_t size(void){
    	boost::lock_guard<boost::mutex> lock(m);
	int sz =  q.size();
	return sz;
  }

private:
  std::queue<T> q;
  mutable boost::mutex m;
  boost::condition_variable c;
};
#endif
