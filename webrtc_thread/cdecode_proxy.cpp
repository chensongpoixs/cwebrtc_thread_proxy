/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	cdecoderProxy

*********************************************************************/

#include "cdecode_proxy.h"
#include <atomic>
#include "clog.h"
namespace chen {
	chen::cdecoderProxy test1;
	std::atomic<int32_t> nums = 1;


	std::vector<std::string> urls ={
	"http://192.168.2.235/cosmos/520844025075605504.flv",
	"http://192.168.2.235/cosmos/520845229717463040.flv",
	"http://192.168.2.235/cosmos/542232770748096512.flv",
	"http://192.168.2.235/cosmos/542234532422885376.flv",
	"http://192.168.2.235/cosmos/542235539097788416.flv",
	"http://192.168.2.235/cosmos/542234784072736768.flv",
	"http://192.168.2.235/cosmos/542234784085319680.flv"
	};

	void yuvCallback(datautil::avRawDataPtr pYuvData, std::string uid)
	{
		NORMAL_EX_LOG("uid = %s", uid.c_str());
	}
	void exitcallback(bool bIsOccurErr, std::string uid)
	{
		NORMAL_EX_LOG("uid = %s", uid.c_str());
	}
	


	uint32 thread_nums = 50;
	std::atomic<int32_t> nums1 = 1;
	static FILE * out_log_ptr = fopen("out_file.log", "wb+");
	std::mutex g_mutex;
	void test_init()
	{
		std::string url = "123456789" + std::to_string(nums++);
		int32_t index = nums;
		printf("work thread_id = %p\n", std::this_thread::get_id());
		test1.start(urls[index %urls.size()], urls[index %urls.size()], urls[index %urls.size()], nullptr, nullptr );

	}

	void test_init2()
	{
		std::string url = "123456789" + std::to_string(nums1++);
		int32_t index = nums;
		//printf("work thread_id = %p\n", std::this_thread::get_id());
		//test1.init(urls[index %urls.size()], urls[index %urls.size()], urls[index %urls.size()]);
		printf("work thread_id = %p\n", std::this_thread::get_id());
		test1.start(urls[index %urls.size()], urls[index %urls.size()], urls[index %urls.size()], nullptr, nullptr );

	}

	void stop_start()
	{
		int32_t index = ++nums;
		int32_t status = test1.get_status(urls[index %urls.size()]);
		NORMAL_EX_LOG("work thread_id = %p stop = %s\n", std::this_thread::get_id(), urls[index %urls.size()].c_str());
		if (status == EDECODE_RUNING)
		{
			test1.stop(urls[index %urls.size()]);
		}
		status = test1.get_status(urls[index %urls.size()]);
		while (EDECODE_NODE != status  )
		{
			test1.stop(urls[index %urls.size()]);
			NORMAL_EX_LOG("work sleep 1000 ... thread_id = %p\n", std::this_thread::get_id());

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			status = test1.get_status(urls[index %urls.size()]);
		}
		
		NORMAL_EX_LOG("work thread_id = %p\n", std::this_thread::get_id());
		test1.start(urls[index %urls.size()], urls[index %urls.size()], urls[index %urls.size()], nullptr, nullptr );

		NORMAL_EX_LOG("work thread_id = %p start = %s\n", std::this_thread::get_id(), urls[index %urls.size()].c_str());

	}

	void start_thread_pool()
	{
		{
			std::lock_guard<std::mutex> lock(g_mutex);
			fprintf(out_log_ptr, "start_thread_pool start\n");
			fflush(out_log_ptr);
		}
		nums = 0;

		std::vector<std::thread*> m_threads;
		std::vector<std::thread*> m_thread2s;
		for (int i = 0; i < thread_nums; ++i)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, "m_threads = %d\n", i);
				fflush(out_log_ptr);
			}
			std::thread*  thread_ptr = new std::thread(&stop_start);
			m_threads.push_back(thread_ptr);

		}

		nums = 0;
		for (int i = 0; i < thread_nums; ++i)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, " m_thread2s = %d\n", i);
				fflush(out_log_ptr);
			}
			std::thread*  thread_ptr = new std::thread(&stop_start);
			m_thread2s.push_back(thread_ptr);

		}
		NORMAL_EX_LOG("start stop !!!");
		int i = 0;
		for (std::thread *& thread: m_thread2s)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, "exit m_thread2s = %d\n", ++i);
				fflush(out_log_ptr);
			}
			if (thread->joinable())
			{
				thread->join();
			}
			delete thread;
		}
		i = 0;
		for (std::thread *& thread: m_threads)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, "exit m_threads = %d\n", ++i);
				fflush(out_log_ptr);
			}
			if (thread->joinable())
			{
				thread->join();
			}
			delete thread;
		}
		m_thread2s.clear();
		m_threads.clear();
		{
			std::lock_guard<std::mutex> lock(g_mutex);
			fprintf(out_log_ptr, "start_thread_pool end\n");
			fflush(out_log_ptr);
		}
	}
	void test()
	{

		 

		if (!LOG::init(ELogStorageFilePost))
		{
			printf("log init failed !!!\n");
			return;
		}
		datautil::DeviceDecoderType_t type = datautil::Device_Decoder_Cpu;
		initDeviceDecoderEnv(type);
		if (!g_cthread_pool_mgr.init(8))
		{
			printf("thread init failed !!!\n");
			return;
		}

		//chen::cedecode* ptr = new chen::cedecode();
		printf("work thread_id = %p\n", std::this_thread::get_id());
		
		test1.start("http://192.168.2.235/cosmos/520844025075605504.flv", "http://192.168.2.235/cosmos/520844025075605504.flv", "testlibyuv",  &yuvCallback , &exitcallback );

		/*std::vector<std::thread*> m_threads;
		std::vector<std::thread*> m_thread2s;
		for (int i = 0; i < thread_nums; ++i)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, "m_threads = %d\n", i);
				fflush(out_log_ptr);
			}
			std::thread*  thread_ptr = new std::thread(&test_init);
			m_threads.push_back(thread_ptr);

		}


		for (int i = 0; i < thread_nums; ++i)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, " m_thread2s = %d\n", i);
				fflush(out_log_ptr);
			}
			std::thread*  thread_ptr = new std::thread(&test_init2);
			m_thread2s.push_back(thread_ptr);

		}
		int i = 0;
		for (std::thread *& thread: m_thread2s)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, "exit m_thread2s = %d\n", ++i);
				fflush(out_log_ptr);
			}
			if (thread->joinable())
			{
				thread->join();
			}
			delete thread;
		}
		i = 0;
		for (std::thread *& thread: m_threads)
		{
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				fprintf(out_log_ptr, "exit m_threads = %d\n", ++i);
				fflush(out_log_ptr);
			}
			if (thread->joinable())
			{
				thread->join();
			}
			delete thread;
		}
		m_thread2s.clear();
		m_threads.clear();
		
		int count = 0;
		while (1)
		{

			std::this_thread::sleep_for(std::chrono::seconds(1));
			start_thread_pool();
			++count;
			if (count > 30)
			{
				break;
			}
		}*/

		while (1)
		{

			printf("second 1 ...\n");
			std::this_thread::sleep_for(std::chrono::seconds(1));
			
		}

		g_cthread_pool_mgr.destory();
		LOG::destroy();
		printf("thread pool destory !!!\n");
	}
}