#pragma once

/**
 * @file    resource.h
 * @brief   UI资源缓存，把图片缓存进内存中，以map<名称，数据>的方式缓存在内存中，当
			程序结束时，需要释放
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-25
 */

#include <xstring>
#include <string>
#include <map>
#include <Windows.h>
using namespace std;
class CxImage;

namespace HUI {

class UIResourceManager
{
public:
	~UIResourceManager();
	static UIResourceManager* GetInstance();
	static void FreeInstance();
	static UIResourceManager *cache_;

	BOOL GetImageByFilePath(const wstring &path, CxImage **image);
private:
	UIResourceManager() {}
private:
	std::map<wstring, CxImage*> image_caches_;
}; // class UIResourceManager

} // namespace HUI