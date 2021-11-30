#pragma once
#include "Framework.hpp"
#include "PluginCore.h"

#include <kxf/IO/IStream.h>
#include <kxf/FileSystem/IFileSystem.h>

namespace xSE
{
	class CommonExtenderPlatform: public kxf::RTTI::Implementation<CommonExtenderPlatform, IExtenderPlatform>
	{
		private:
			PlatformType m_PlatformType = PlatformType::None;
			
			std::shared_ptr<IExtenderPlugin> m_Plugin;
			std::unique_ptr<kxf::IOutputStream> m_LogStream;

		private:
			bool IsNull() const
			{
				return m_PlatformType == PlatformType::None;
			}

			kxf::String GetPlatformFolderName() const;
			kxf::FSPath GetGameConfigPath() const;
			kxf::FSPath GetPlatformDirectoryPath() const;

			void InitializeLogger();
			bool InitializeModules();

		public:
			CommonExtenderPlatform(PlatformType type) noexcept
				:m_PlatformType(type)
			{
			}

		public:
			// IExtenderPlatform
			xSE::PlatformType GetType() const override;
			kxf::String GetName() const override;
			kxf::String GetFullName() const override;
			kxf::String GetGameName() const override;
			kxf::Version GetVersion() const override;

			std::shared_ptr<kxf::IFileSystem> GetGameRootDirectory() const override;
			std::shared_ptr<kxf::IFileSystem> GetGameDataDirectory() const override;
			std::shared_ptr<kxf::IFileSystem> GetPlatformDirectory() const override;
			std::shared_ptr<kxf::IFileSystem> GetPlatformPluginsDirectory() const override;
			std::shared_ptr<kxf::IFileSystem> GetPlatformLogsDirectory() const override;

			bool Initialize(std::shared_ptr<IExtenderPlugin> plugin) override;
			void Terminate() override;

			void LogString(kxf::String logString, size_t indent) override;
	};
}
