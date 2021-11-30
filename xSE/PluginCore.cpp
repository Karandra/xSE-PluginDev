#include "pch.hpp"
#include "PluginCore.h"
#include "PluginCore/CommonExtenderPlatform.h"
#include "PluginCore/ScriptExtenderDefinesBase.h"

namespace
{
	#if xSE_PLATFORM_MWSE

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::MWSE;

	#elif xSE_PLATFORM_OBSE

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::OBSE;

	#elif xSE_PLATFORM_FOSE

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::FOSE;

	#elif xSE_PLATFORM_NVSE

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::NVSE;

	#elif xSE_PLATFORM_SKSE

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::SKSE;

	#elif xSE_PLATFORM_SKSE64

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::SKSE64;

	#elif xSE_PLATFORM_SKSEVR

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::SKSEVR;

	#elif xSE_PLATFORM_F4SE

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::F4SE;

	#elif xSE_PLATFORM_F4SEVR

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::F4SEVR;

	#endif
}

namespace xSE
{
	std::shared_ptr<IExtenderPlatform> GetPlatform()
	{
		return kxf::RTTI::assume_non_owned(g_Platform);
	}
}
