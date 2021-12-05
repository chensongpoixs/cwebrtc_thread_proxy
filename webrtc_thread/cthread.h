/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	Location

*********************************************************************/

#ifndef  _MESSAGE_QUEUE_H_
#define  _MESSAGE_QUEUE_H_

#include <string.h>

#include <algorithm>
#include <list>
#include <memory>
#include <queue>
#include <vector>
#include <thread>
#include "cconstructor_magic.h"
#include<condition_variable>
#include <atomic>
#include "clocation.h"
#include "cmessage_handler.h"


namespace chen {

struct Message;
class MessageQueue;

const uint32_t MQID_ANY = static_cast<uint32_t>(-1);
const uint32_t MQID_DISPOSE = static_cast<uint32_t>(-2);

// No destructor
class MessageData {
public:
	MessageData() {}
	virtual ~MessageData() {}
};
struct Message {
	Message()
		: phandler(nullptr), message_id(0), pdata(nullptr), cur_time() {}
	inline bool Match(MessageHandler* handler, uint32_t id) const {
		return (handler == nullptr || handler == phandler) &&
			(id == MQID_ANY || id == message_id);
	}
	Location posted_from;
	MessageHandler* phandler;
	uint32_t message_id;
	MessageData* pdata;
	std::chrono::steady_clock::time_point cur_time;
};

typedef std::list<Message> MessageList;

// DelayedMessage goes into a priority queue, sorted by trigger time.  Messages
// with the same trigger time are processed in num_ (FIFO) order.



class Thread {
	_DISALLOW_COPY_AND_ASSIGN(Thread)
private:
	typedef std::condition_variable					ccond;
	typedef std::atomic_bool						catomic_bool;
	typedef std::lock_guard<std::mutex>				clock_guard;
	typedef std::mutex								cmutex;
 public:
  static const int kForever = -1;

  Thread();

  virtual ~Thread();
public:

  bool init();
 
  void Destroy();
 // virtual void Quit();
   bool IsQuitting();



 
   void Post(const Location& posted_from,
                    MessageHandler* phandler,
                    uint32_t id = 0,
					 MessageData* pdata = nullptr,
                    bool time_sensitive = true);
  
   void Dispatch(Message* pmsg);
  bool	IsCurrent() const ;
private:
  void			_work_pthread();

  

 private:
	catomic_bool	m_stoped;

	cmutex			m_lock;
  MessageList		m_list;
  std::thread		m_thread;
  std::thread::id	m_thread_id;
  ccond				m_condition;
  //RTC_DISALLOW_COPY_AND_ASSIGN(MessageQueue);
};
//extern Thread g_thread;
}  // namespace chen

#endif  // RTC_BASE_MESSAGE_QUEUE_H_
