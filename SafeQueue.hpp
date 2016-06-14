#ifndef _SAFE_QUEUE
#define _SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

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
    m.unlock();
    c.notify_one();
  }

  T dequeue(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

  size_t size(void){
    	std::lock_guard<std::mutex> lock(m);
	int sz =  q.size();
	m.unlock();
	c.notify_one();
	return sz;
  }

  bool empty(void) {
	std::lock_guard<std::mutex> lock(m);
	bool isEmpty = q.empty();
	m.unlock();
	c.notify_one();
	return isEmpty;
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};
#endif
