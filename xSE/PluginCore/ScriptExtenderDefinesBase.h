#pragma once

//////////////////////////////////////////////////////////////////////////
// Main defines
//////////////////////////////////////////////////////////////////////////


#if xSE_PLATFORM_MWSE

#define xSE_NAME MWSE
#define xSE_PACKED_VERSION PACKED_MWSE_VERSION

#define xSE_LOADFUNCTION MWSEPlugin_Load
#define xSE_QUERYFUNCTION MWSEPlugin_Query

#elif xSE_PLATFORM_OBSE

#define xSE_NAME OBSE
#define xSE_PACKED_VERSION OBSE_VERSION_INTEGER

#define xSE_LOADFUNCTION OBSEPlugin_Load
#define xSE_QUERYFUNCTION OBSEPlugin_Query

#elif xSE_PLATFORM_FOSE

#define xSE_NAME FOSE
#define xSE_PACKED_VERSION FOSE_VERSION_INTEGER

#define xSE_LOADFUNCTION FOSEPlugin_Load
#define xSE_QUERYFUNCTION FOSEPlugin_Query

#elif xSE_PLATFORM_NVSE

#define xSE_NAME NVSE
#define xSE_PACKED_VERSION NVSE_VERSION_INTEGER

#define xSE_LOADFUNCTION NVSEPlugin_Load
#define xSE_QUERYFUNCTION NVSEPlugin_Query

#elif xSE_PLATFORM_SKSE

#define xSE_NAME SKSE
#define xSE_PACKED_VERSION PACKED_SKSE_VERSION

#define xSE_LOADFUNCTION SKSEPlugin_Load
#define xSE_QUERYFUNCTION SKSEPlugin_Query

#elif xSE_PLATFORM_SKSE64

#define xSE_NAME SKSE64
#define xSE_PACKED_VERSION PACKED_SKSE_VERSION

#define xSE_LOADFUNCTION SKSEPlugin_Load
#define xSE_QUERYFUNCTION SKSEPlugin_Query

#elif xSE_PLATFORM_SKSEVR

#define xSE_NAME SKSEVR
#define xSE_PACKED_VERSION PACKED_SKSE_VERSION

#define xSE_LOADFUNCTION SKSEPlugin_Load
#define xSE_QUERYFUNCTION SKSEPlugin_Query

#elif xSE_PLATFORM_F4SE

#define xSE_NAME F4SE
#define xSE_PACKED_VERSION PACKED_F4SE_VERSION

#define xSE_LOADFUNCTION F4SEPlugin_Load
#define xSE_QUERYFUNCTION F4SEPlugin_Query

#else

#error "Unsupported configuration"

#endif

#define xSE_NAME_A _CRT_STRINGIZE(xSE_NAME)
#define xSE_NAME_W _CRT_WIDE(xSE_NAME_A)

#ifdef xSE_LIBRARY
	#define xSE_BIND_API __declspec(dllexport)
#else
	#define xSE_BIND_API
#endif
