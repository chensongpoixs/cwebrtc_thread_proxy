#include "clog.h"
#include "cdecoder_object.h"
#include <memory>
#include <iostream>

#include <mutex>



namespace chen {

	
	cdecoder_object::cdecoder_object()
	{

		
	}
	cdecoder_object::~cdecoder_object()
	{

	}



	int32_t cdecoder_object::start(const std::string &url, const std::string & uid )

	{
		NORMAL_EX_LOG("url = %s, uid = %s", url.c_str(), uid.c_str());
		
		return 0;
	}
	
	int32_t cdecoder_object::get_status()
	{
		NORMAL_EX_LOG("url = %s, uid = %s", m_url.c_str(), m_uid.c_str());
		return m_status.load();
	}
	int32_t cdecoder_object::stop()
	{
		NORMAL_EX_LOG("url = %s, uid = %s, m_status = %d", m_url.c_str(), m_uid.c_str(), m_status.load());
		
		return 0;
	}

	void cdecoder_object::Destory()
	{
		NORMAL_EX_LOG("url = %s, uid = %s, m_status = %d", m_url.c_str(), m_uid.c_str(), m_status.load());

		
	}

	void cdecoder_object::_work_thread()
	{
		
	}
}