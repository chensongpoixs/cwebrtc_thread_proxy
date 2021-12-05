/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	cthread_pool_mgr

*********************************************************************/


#ifndef _THREAD_POOL_MGR_H_
#define _THREAD_POOL_MGR_H_
#include "cthread_pool_mgr.h"
#include <unordered_map>
#include <atomic>
#include <memory>
#include <mutex>
#include "cdecoder_object.h"
namespace chen {
	class cdecoder_object;
	class Thread;
	struct Object
	{
		cdecoder_object*  m_object ;
		Thread*  m_thread ;
		Object()
			:m_object(nullptr)
			, m_thread(nullptr) {}
	};
	class cthread_pool_mgr
	{
	private:
		

		typedef std::lock_guard<std::mutex>			clock_guard;
		typedef std::mutex							clock;
	public:
		cthread_pool_mgr();
		virtual ~cthread_pool_mgr();
	public:

		bool init(int32_t thread_nums = 1);
		bool get_status() const { return m_stoped.load(); }
		void destory();
	public:
		//const Object * get_object_thread(const std::string & url) const ;
		 Object * get_object_thread(const std::string & url);
	private:

		Object*   _add_object_thread(const std::string & url);
	private:
		cthread_pool_mgr& operator=(const cthread_pool_mgr&);
		cthread_pool_mgr(const cthread_pool_mgr&); 
	private:
		typedef std::unordered_map<std::string, Object>::iterator iterater;
		typedef std::unordered_map<std::string, Object>::const_iterator const_iterater;
	private:
		std::vector< Thread*>		m_threads;
		
		std::atomic_bool							m_stoped;
		clock										m_all_object_lock;
		std::unordered_map<std::string, cdecoder_object*>		m_all_object;

		clock										m_object_threads_lock;
		std::unordered_map<std::string, Object>		m_all_object_threads;
		
	};
	extern cthread_pool_mgr g_cthread_pool_mgr;
}

#endif // _THREAD_POOL_MGR_H_
