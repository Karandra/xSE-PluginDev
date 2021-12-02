#include "pch.hpp"
#include "PluginCore.h"
#include "PluginCore/CommonExtenderPlatform.h"
#include "PluginCore/ScriptExtenderDefinesBase.h"
#include "PluginCore/ScriptExtenderDefinesExtra.h"
#include "PluginCore/ScriptExtenderInterfaceIncludes.h"

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

	#elif xSE_PLATFORM_SKSEVR

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::SKSEVR;

	#elif xSE_PLATFORM_SKSE64

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::SKSE64;

	#elif xSE_PLATFORM_SKSE64AE

	xSE::CommonExtenderPlatform g_Platform = xSE::PlatformType::SKSE64AE;

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

extern "C"
{
	xSE_BIND_API xSE_QUERYFUNCTION_SIGNATURE(const xSE_Interface* xSE, PluginInfo* info)
	{
		return g_Platform.OnQuery(xSE, info);
	}
	xSE_BIND_API xSE_LOADFUNCTION_SIGNATURE(const xSE_Interface* xSE)
	{
		return g_Platform.OnLoad(xSE);
	}

	#if xSE_PLATFORM_SKSE64AE
	xSE_BIND_API constexpr auto SKSEPlugin_Version = []()
	{
		SKSEPluginVersionData versionData = {};
		versionData.dataVersion = SKSEPluginVersionData::kVersion;

		// Dummy version
		versionData.pluginVersion = 1;

		// Set this to be version independent and perform an actual check on load
		versionData.versionIndependence = SKSEPluginVersionData::kVersionIndependent_Signatures;
		
		// Static name for a plugin based on this framework
		std::ranges::copy("xSE PluginDev Plugin", versionData.name);

		return versionData;
	}();
	#endif
}
