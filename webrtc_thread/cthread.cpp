/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	thread

*********************************************************************/

#include <string>
#include <utility>

 
#include "cthread.h"
#include "clog.h"

namespace chen {
	//Thread g_thread;

const int kMaxMsgLatency = 150;                // 150 ms
const int kSlowDispatchLoggingThreshold = 50;  // 50 ms


static std::chrono::steady_clock::time_point get_timemillis()
{
	return std::chrono::steady_clock::now() ;
}

//------------------------------------------------------------------
// MessageQueue
Thread::Thread()
	: m_stoped(true)
{}



Thread::~Thread() 
{
	//Destroy();
}

bool Thread::init() {
  if (!m_stoped.load()) 
  {
    return false;
  }
  m_stoped.store(false);
  m_thread = std::thread(std::bind(&Thread::_work_pthread, this));
  //printf("thread work");
  return true;
}

void Thread::Destroy() 
{
	if (!m_stoped.load())
	{
		m_stoped.store(true);
	}
	m_condition.notify_all();
	// all thread stop
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}



bool Thread::IsQuitting() 
{
  return m_stoped.load();
}




void Thread::Post(const Location& posted_from,
                        MessageHandler* phandler,
                        uint32_t id,
						MessageData* pdata,
                        bool time_sensitive) {
  if (IsQuitting()) {
    delete pdata;
    return;
  }

   
  {
	  clock_guard lock(m_lock);
    Message msg;
    msg.posted_from = posted_from;
    msg.phandler = phandler;
    msg.message_id = id;
    msg.pdata = pdata;
    msg.cur_time = get_timemillis() ;
    m_list.push_back(msg);
	 
  }
  {
	  m_condition.notify_one();
  }
}


void			Thread::_work_pthread()
{
	m_thread_id = std::this_thread::get_id();
	MessageList temp_message;
	while (!m_stoped)
	{
		// 
		if (m_list.size()<= 0)
		{
			std::unique_lock<std::mutex> lock(m_lock);

			m_condition.wait(lock, [this]() {return m_stoped.load() || m_list.size() >0; });
		}
		
		{
			clock_guard lock(m_lock);
			temp_message = m_list;
			m_list.clear();
		}
		for (MessageList::iterator iter = temp_message.begin(); iter != temp_message.end(); ++iter)
		{
			Dispatch(&*iter);
		}
		temp_message.clear();
	}

}
//void Thread::Quit()
//{
//}
void Thread::Dispatch(Message* pmsg) {
 
	std::chrono::steady_clock::time_point start_time = get_timemillis();
	std::chrono::steady_clock::duration diff =   start_time - pmsg->cur_time;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
  if (ms.count() > (kMaxMsgLatency - kSlowDispatchLoggingThreshold))
  {
	  WARNING_EX_LOG("work thread Message took  diff = %lu ms to dispatch. Posted from: = %s", ms.count(), pmsg->posted_from.ToString().c_str());
  }
  pmsg->phandler->OnMessage(pmsg);
  std::chrono::steady_clock::time_point end_time = get_timemillis();
   diff = end_time - start_time;
   ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
  if (ms.count() > kSlowDispatchLoggingThreshold)
  {
	  WARNING_EX_LOG("Message took  diff = %lu ms to dispatch. Posted from: = %s", ms.count(), pmsg->posted_from.ToString().c_str());
  }

  
}
bool	Thread::IsCurrent() const
{
	return std::this_thread::get_id() == m_thread_id;
}
}  // namespace rtc
