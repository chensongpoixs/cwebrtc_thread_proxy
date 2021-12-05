#ifndef FFMPEGUTIL_H
#define FFMPEGUTIL_H

#include <string>
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>
 
#include "cdecode_interface.h"
#include <memory>

 




namespace chen
{
	 

	class cdecoder_object : public cdecoderInterface
	{
	private:
		typedef std::lock_guard<std::mutex>			clock_guard;
		
	public:
		cdecoder_object();
		~cdecoder_object();


	public:
		 
		 int32_t start(const std::string &url, const std::string & uid );
		 
		 int32_t get_status()  ;
		 int32_t stop()  ;

		void Destory();

	private:
		void _work_thread();
	private:
		std::string			m_url;
		std::string			m_uid;
	 
		 
		 
		std::atomic<EDECODE_STATUS_TYPE> m_status;
	 

	};
}

#endif // !FFMPEGUTIL_H
