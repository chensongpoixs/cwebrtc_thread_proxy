/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	cthread_pool_mgr

*********************************************************************/
#include "cthread_pool_mgr.h"
#include "clog.h"
#include "cdecode_proxy.h"
#include "clog.h"
namespace chen {
	cthread_pool_mgr g_cthread_pool_mgr;
	cthread_pool_mgr::cthread_pool_mgr()
		:m_stoped(true)
	{}
	cthread_pool_mgr::~cthread_pool_mgr() {}


	bool cthread_pool_mgr::init(int32_t thread_nums)
	{
		NORMAL_EX_LOG("thread pool init ... thread_nums = %d", thread_nums);
		if (!m_stoped)
		{
			WARNING_EX_LOG("stoped = %d", m_stoped.load());
			return false;
		}
		if (thread_nums < 1)
		{
			WARNING_EX_LOG("thread_nums < 0  default 1 !!!" );
			thread_nums = 1;
		}

		for (int32_t i = 0; i < thread_nums; ++i)
		{
			  Thread* thread_ptr  = new Thread();
			 
			 if (!thread_ptr)
			 {
				 WARNING_EX_LOG("alloc thread failed !!!");
				 continue;
			 }
			 if (!thread_ptr->init())
			 {
				 WARNING_EX_LOG(" thread init failed !!!");
				 delete thread_ptr;
				 thread_ptr = NULL;
				 continue;
			 }
			 m_threads.push_back(thread_ptr);
			
		}
		if (m_threads.size() < 1)
		{
			ERROR_EX_LOG("thread_pool not find thread  !!! ");
			return false;
		}
		m_stoped.store(false);
		return true;
	}
	void cthread_pool_mgr::destory()
	{
		if (m_stoped)
		{
			// wraning 
			return;
		}
		
		m_stoped.store(false);
		clock_guard lock(m_all_object_lock);
		NORMAL_EX_LOG("thread pool  delete ...");

		for (std::vector<Thread*>::iterator iter = m_threads.begin(); iter != m_threads.end(); ++iter)
		{ 
			if ((*iter))
			{ 
				(*iter)->Destroy();
				delete (*iter); 
			}
		}
		NORMAL_EX_LOG("cdecoder_object  Destory ...");
		for (std::unordered_map<std::string, cdecoder_object*>::iterator iter = m_all_object.begin(); iter != m_all_object.end(); ++iter)
		{
			cdecoder_object* ptr = iter->second;
			if (ptr)
			{
				//ptr->stop();
				ptr->Destory();
				delete ptr;
			}
		}
		m_all_object.clear();
		m_threads.clear();
		m_all_object_threads.clear();
		NORMAL_EX_LOG("thread pool end !!!");
		// show exit signed;

	}

	 
	Object * cthread_pool_mgr::get_object_thread(const std::string & url)
	{
		NORMAL_EX_LOG("url = %s", url.c_str());
		if (m_stoped)
		{
			// warning --->
			WARNING_EX_LOG("url = %s", url.c_str());
			return nullptr;
		}
		//clock_guard lock(m_object_threads_lock);
		clock_guard lock(m_all_object_lock);
		iterater iter = m_all_object_threads.find(url);
		if (iter != m_all_object_threads.end())
		{
			return &iter->second;
		}
		return _add_object_thread(url);
	}
	Object*   cthread_pool_mgr::_add_object_thread(const std::string & url)
	{
		NORMAL_EX_LOG("url = %s", url.c_str());
		if (m_stoped)
		{
			// warning ---> //可能 释放的时候申请内存哈 ^_^
			WARNING_EX_LOG("url = %s", url.c_str());
			return nullptr;
		}
		
		
		std::unordered_map<std::string, cdecoder_object*>::iterator iter = m_all_object.find(url);
		
		Object object;
		if (iter == m_all_object.end() )
		{
			cdecoder_object* decode_ptr  = new cdecoder_object();
			if (!decode_ptr)
			{
				WARNING_EX_LOG(" alloc cdecoder object failed ", url.c_str());
				return nullptr;
			}
			m_all_object[url] = decode_ptr;
			object.m_object = decode_ptr;
		}
		else
		{
			object.m_object = iter->second;
		}
		object.m_thread = m_threads[m_all_object.size() % m_threads.size()];
		std::pair<std::unordered_map<std::string, Object>::iterator, bool> pi = m_all_object_threads.insert(std::make_pair(url, object));
		if (!pi.second)
		{
			WARNING_EX_LOG(" insert  cdecoder object failed ", url.c_str());
			return NULL;
		}
		return &pi.first->second;
	}
}