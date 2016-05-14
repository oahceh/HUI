#include "hui.h"
#include "resource.h"

void InitHUI() {
	HUI::UIResourceManager::GetInstance();
}

void DisposeHUI() {
	HUI::UIResourceManager::FreeInstance();
}