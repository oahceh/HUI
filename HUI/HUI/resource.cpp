#include "resource.h"
#include "CxImage\ximage.h"

namespace HUI {

UIResourceManager* UIResourceManager::cache_ = NULL;

UIResourceManager* UIResourceManager::GetInstance() {
	if (NULL == cache_) { cache_ = new UIResourceManager(); }
	return cache_;
}

void UIResourceManager::FreeInstance() { delete cache_; }

UIResourceManager::~UIResourceManager() {
	for (auto var : image_caches_) { delete var.second; }
}

BOOL UIResourceManager::GetImageByFilePath(const wstring &path, CxImage **image) {
	if (image_caches_.find(path) == image_caches_.end()) {
		// image not found
		BOOL bFound = FALSE;
		CxImage *cximage = new CxImage();
		if (cximage->Load(path.c_str())) {
			image_caches_[path] = cximage;
			*image = cximage;
			bFound = TRUE;
		} else { // Load failed
			delete image; 
		}
		return bFound;
	} else {
		*image = image_caches_[path];
		return TRUE;
	}
}

} // namespace HUI