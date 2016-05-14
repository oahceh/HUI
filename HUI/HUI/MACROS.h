#pragma once

#include <cassert>

#define PTR_VOID(p)		{ assert(p); if (NULL == p) { return; } }