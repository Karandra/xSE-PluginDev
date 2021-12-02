#pragma once
#include "Framework.hpp"
#include "PluginCore.h"
#include "ScriptExtenderDefinesBase.h"

#include <kxf/IO/IStream.h>
#include <kxf/EventSystem/IEvtHandler.h>
#include <kxf/FileSystem/IFileSystem.h>

namespace xSE
{
	class CommonExtenderPlatform: public kxf::RTTI::Implementation<CommonExtenderPlatform, IExtenderPlatform>
	{
		private:
			PlatformType m_PlatformType = PlatformType::None;
			
			std::shared_ptr<IExtenderPlugin> m_Plugin;
			std::shared_ptr<kxf::IEvtHandler> m_EvtHandler;
			std::unique_ptr<kxf::IOutputStream> m_LogStream;

			// xSE info
			kxf::String m_PluginName;
			const void* m_SEInterface = nullptr;
			uint32_t m_PluginHandle = std::numeric_limits<uint32_t>::max();
			uint32_t m_SEVersion = 0;
			uint32_t m_RuntimeVersion = 0;
			uint32_t m_EditorVersion = 0;
			bool m_QueryCalled = false;
			bool m_LoadCalled = false;

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

			void LogString(const kxf::String& category, kxf::String logString, size_t indent) override;

		public:
			// CommonExtenderPlatform
			bool OnQuery(const void* seInterface, void* pluginInfo);
			bool OnLoad(const void* seInterface);
	};
}
