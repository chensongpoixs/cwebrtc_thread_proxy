/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	cdecoderInterface

*********************************************************************/

#ifndef _DSP_CDECODE_INTERFACE_H_
#define _DSP_CDECODE_INTERFACE_H_
#include "Export.h"
#include <iostream>
#include <thread>
 
#include "cdecoder_enum.h"
namespace chen
{
	

	class cdecoderInterface
	{
	public:

		virtual int32_t start(const std::string &url, const std::string & uid ) = 0;
		virtual int32_t get_status() = 0;
		virtual int32_t stop() = 0;
	
		virtual ~cdecoderInterface() {}
	};
	
}

#endif // _DSP_CDECODE_INTERFACE_H_