#include "Gpuinfo.h"

#if defined(_WIN32)

#include <windows.h>
 
#include <DXGI.h>  


#pragma comment(lib,"DXGI.lib") 
#pragma warning(disable: 4996)  



#endif // defined(_WIN32)


#include <unordered_set>



namespace dsp
{
	// 列表https://bluesky-soft.com/dxvac/deviceInfo/decoder/intel.html
	static std::unordered_set<std::string>  G_HardWare_GPU_ALL =
	{
		"Intel(R) HD Graphics(Cherry Trail)",
		"Intel(R) HD Graphics(Braswell)",
		"Intel(R) HD Graphics(Apollo Lake)",
		"Intel(R) HD Graphics 500",
		"Intel(R) HD Graphics 510",
		"Intel(R) HD Graphics 515",
		"Intel(R) HD Graphics 520",
		"Intel(R) HD Graphics 530",
		"Intel(R) HD Graphics 610",
		"Intel(R) HD Graphics 615",
		"Intel(R) HD Graphics 620",
		"Intel(R) HD Graphics 630",
		"Intel(R) Iris Graphics 540",
		"Intel(R) Iris Graphics 6100",
		"Intel(R) Iris Plus Graphics",
		"Intel(R) Iris Plus Graphics 640",
		"Intel(R) Iris Plus Graphics 655",
		"Intel(R) Iris Pro Graphics 580",
		"Intel(R) Iris Pro Graphics P6300",
		"Intel(R) Iris Xe Graphics",
		"Intel(R) UHD Graphics(Comet Lake)",
		"Intel(R) UHD Graphics(Tiger Lake)",
		"Intel(R) UHD Graphics 600",
		"Intel(R) UHD Graphics 605",
		"Intel(R) UHD Graphics 610",
		"Intel(R) UHD Graphics 617",
		"Intel(R) UHD Graphics 620",
		"Intel(R) UHD Graphics 630",
		"Intel(R) UHD Graphics 650"

	}; // 支持英特尔 psv类型 

	static const char* Intel = "Intel";
	static const char* Nvidia = "NVIDIA";

	
#if defined(_WIN32)
	//WString转String
	std::string WStringToString(const std::wstring& wstr)
	{
		std::string str(wstr.length(), ' ');
		std::copy(wstr.begin(), wstr.end(), str.begin());
		return str;
	}
	//获取显卡信息
	static int GetGpuInfo(char* pDeviceDescription1, char* pDeviceDescription2, char* pDeviceDescription3, int* pDeviceDescriptionsize)
	{
		// 参数定义  
		IDXGIFactory* pFactory;
		IDXGIAdapter* pAdapter;
		std::vector <IDXGIAdapter*> vAdapters;            // 显卡vector         
		int iAdapterNum = 0; // 显卡的数量  

		//创建一个DXGI工厂 (必须用CreateDXGIFactory1，如果用CreateDXGIFactory只能取出一块显卡信息) 
		HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&pFactory));
		if (FAILED(hr))
		{
			return 0;
		}

		// 枚举适配器  
		while (pFactory->EnumAdapters(iAdapterNum, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			vAdapters.push_back(pAdapter);
			++iAdapterNum;
		}
		*pDeviceDescriptionsize = iAdapterNum;

		// 信息输出  
		//printf("===============获取到%d块显卡===============\n", iAdapterNum);
		for (size_t i = 0; i < vAdapters.size(); i++)
		{
			// 获取信息  
			DXGI_ADAPTER_DESC adapterDesc;
			vAdapters[i]->GetDesc(&adapterDesc);
			std::wstring WDeviceDescription(adapterDesc.Description);
			std::string DeviceDescription = WStringToString(WDeviceDescription);

			if (i == 0)
			{
				memcpy(pDeviceDescription1, DeviceDescription.c_str(), DeviceDescription.size());
				pDeviceDescription1[DeviceDescription.size()] = 0;
			}
			if (i == 1)
			{
				memcpy(pDeviceDescription2, DeviceDescription.c_str(), DeviceDescription.size());
				pDeviceDescription2[DeviceDescription.size()] = 0;
			}
			if (i == 2)
			{
				memcpy(pDeviceDescription3, DeviceDescription.c_str(), DeviceDescription.size());
				pDeviceDescription3[DeviceDescription.size()] = 0;
			}

			// 输出显卡信息  
			//printf("显卡型号: %s\n", DeviceDescription.c_str());
		}
		vAdapters.clear();

		return 1;
	}


#else
	static int GetGpuInfo(char* pDeviceDescription1, char* pDeviceDescription2, char* pDeviceDescription3, int* pDeviceDescriptionsize)
	{
		return 1;
	}
#endif // defined(_WIN32)
	


	void GetHardWareGpuInfo(bool& HardWare_Intel, bool & HardWare_Nvidia)
	{
		char nDeviceDescription1[256] = { 0 };
		char nDeviceDescription2[256] = { 0 };
		char nDeviceDescription3[256] = { 0 };
		int nDeviceDescription1size = 0;
		int ret = GetGpuInfo(nDeviceDescription1, nDeviceDescription2, nDeviceDescription3, &nDeviceDescription1size);
		
		if (ret == 1 && nDeviceDescription1size> 0)
		{
			printf("[info]nDeviceDescription1size :%d\n", nDeviceDescription1size);
			
			auto funcdevice = [&](const char * device_name)
			{
				printf("[info]DeviceDescription : %s\n", device_name);
				if (memcmp(Intel, device_name, 5) == 0)
				{
					std::unordered_set<std::string>::const_iterator iter = G_HardWare_GPU_ALL.find(device_name);
					if (iter != G_HardWare_GPU_ALL.end())
					{
						HardWare_Intel = true;
					}
				}
				else if (memcmp(Nvidia, device_name, 6) == 0)
				{
					HardWare_Nvidia = true;
				}
			};
			if (strlen(nDeviceDescription1) > 5 )
			{
				funcdevice(nDeviceDescription1);
			}
			if (strlen(nDeviceDescription2) != 0)
			{
				funcdevice(nDeviceDescription2);
				
			}
			if (strlen(nDeviceDescription3) != 0)
			{
				funcdevice(nDeviceDescription3);
			}
		}
		else
		{
			printf("GetGpuInfo error\n");
		}
	}
}