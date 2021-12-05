/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	cdecoder_proxy

*********************************************************************/

#ifndef _DSP_CDECODE_PROXY_H_
#define _DSP_CDECODE_PROXY_H_
#include "cdecode_interface.h"
#include "cproxy.h"
#include "cthread_pool_mgr.h"
#include "cdecoder_object.h"
 
namespace chen
{

	BEGIN_PROXY_MAP(cdecoder)
	PROXY_WORKER_METHOD5(int32_t, start, const std::string &, const std::string &, const std::string & )
	PROXY_WORKER_METHOD1(int32_t, get_status, const std::string &)
	PROXY_WORKER_METHOD1(int32_t, stop, const std::string &)
	END_PROXY_MAP() 

	void    test();
	

	 
}

#endif // _DSP_CDECODE_PROXY_H_