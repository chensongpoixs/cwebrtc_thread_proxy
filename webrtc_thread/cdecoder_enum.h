#ifndef C_DECODER_ENUM_H
#define C_DECODER_ENUM_H
namespace chen
{
	enum EDECODE_STATUS_TYPE
	{
		EDECODE_NODE = 0,
		EDECODE_RUNING,
		EDECODE_THREAD_RUNING // 
	};


	enum EDECODE_ERROR_CODE
	{

		EDecode_Error_Code_Init = 10000, //初始化状态不对
		EDecode_Error_Code_NotInit, 
		EDecode_Error_Code_formatContext, // nullptr 
		EDecode_Error_Code_OpenInput, // url 路径错误或者网络有问题
		EDecode_Error_Code_MediaStreamInfo, //获取媒体的信息错误 
		EDecode_Error_Code_VideoIndex, // 获取视频索引值  一般默认1哈 ^_^
		EDecode_Error_Code_NotFindDecoder, // 没有发现解码器
		EDecode_Error_Code_AlllocCodecContexFailed, // 
		EDecode_Error_Code_ParameterToContext, 
		EDecode_Error_Code_CodecOpenFailed, 
		EDecode_Error_Code_NotThreadRuning, //没有线程运行中
		EDecode_Error_Code_Mem, 
	};

}

#endif // C_DECODER_ENUM_H