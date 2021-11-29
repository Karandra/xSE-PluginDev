#pragma once
#include "Framework.hpp"

namespace kxf
{
	class IFileSystem;
}

namespace xSE
{
	enum class PlatformType
	{
		None = -1,

		MWSE,
		OBSE,
		FOSE,
		NVSE,
		SKSE,
		SKSE64,
		SKSEVR,
		F4SE,
		F4SEVR
	};
}

namespace xSE
{
	class xSE_API IExtenderPlugin: public kxf::RTTI::Interface<IExtenderPlugin>
	{
		KxRTTI_DeclareIID(IExtenderPlugin, {0xd9ad128e, 0x26a3, 0x428f, {0x85, 0xc1, 0x3e, 0x19, 0x25, 0xc0, 0x7e, 0x9d}});

		public:
			virtual kxf::String GetName() const = 0;
			virtual kxf::String GetAuthor() const = 0;
			virtual kxf::Version GetVersion() const = 0;
	};
}

namespace xSE
{
	class xSE_API IExtenderPlatform: public kxf::RTTI::Interface<IExtenderPlatform>
	{
		KxRTTI_DeclareIID(IExtenderPlatform, {0xaaf8c9dd, 0x7ec6, 0x40dd, {0xb0, 0x29, 0x6d, 0x77, 0xc, 0x5b, 0xaf, 0x63}});

		public:
			virtual PlatformType GetType() const = 0;
			virtual kxf::String GetName() const = 0;
			virtual kxf::String GetGameName() const = 0;
			virtual kxf::String GetFullName() const = 0;

			virtual std::shared_ptr<kxf::IFileSystem> GetGameRootDirectory() const = 0;
			virtual std::shared_ptr<kxf::IFileSystem> GetGameDataDirectory() const = 0;
			virtual std::shared_ptr<kxf::IFileSystem> GetPlatformDirectory() const = 0;
			virtual std::shared_ptr<kxf::IFileSystem> GetPlatformPluginsDirectory() const = 0;
			virtual std::shared_ptr<kxf::IFileSystem> GetPlatformLogsDirectory() const = 0;

			virtual bool Initialize(std::shared_ptr<IExtenderPlugin> plugin) = 0;
			virtual void Terminate() = 0;

			virtual void LogString(kxf::String logString, size_t indent = 0) = 0;

		public:
			template<size_t indent = 0>
			void Log(const kxf::String& logString)
			{
				LogString(logString, indent);
			}

			template<size_t indent = 0, class ...Args>
			void Log(const kxf::String& format, Args&&... args)
			{
				LogString(kxf::Format(format, std::forward<Args>(args)...), indent);
			}
	};
}

namespace xSE
{
	xSE_API std::shared_ptr<IExtenderPlatform> GetPlatform();
}
