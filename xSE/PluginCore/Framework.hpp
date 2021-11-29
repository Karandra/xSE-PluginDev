#pragma once

// KxFramework
#include <kxf/Common.hpp>

#include <kxf/RTTI/Common.h>
#include <kxf/General/String.h>
#include <kxf/General/Format.h>
#include <kxf/General/Version.h>
#include <kxf/General/DateTime.h>

#ifdef xSE_LIBRARY
	#define xSE_API __declspec(dllexport)
#else
	#define xSE_API __declspec(dllimport)
#endif
