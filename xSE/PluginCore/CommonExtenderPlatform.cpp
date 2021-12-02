#include "pch.hpp"
#include "CommonExtenderPlatform.h"
#include "ScriptExtenderDefinesExtra.h"
#include "ScriptExtenderInterfaceIncludes.h"
#include "InitializationEvent.h"

#include <kxf/IO/IStream.h>
#include <kxf/IO/StreamReaderWriter.h>
#include <kxf/Log/Common.h>
#include <kxf/System/NativeAPI.h>
#include <kxf/System/DynamicLibrary.h>
#include <kxf/System/ShellOperations.h>
#include <kxf/FileSystem/NativeFileSystem.h>

namespace xSE
{
	kxf::String CommonExtenderPlatform::GetPlatformFolderName() const
	{
		switch (m_PlatformType)
		{
			case PlatformType::MWSE:
			{
				return "MWSE";
			}
			case PlatformType::OBSE:
			{
				return "OBSE";
			}
			case PlatformType::FOSE:
			{
				return "FOSE";
			}
			case PlatformType::NVSE:
			{
				return "NVSE";
			}
			case PlatformType::SKSE:
			case PlatformType::SKSEVR:
			case PlatformType::SKSE64:
			case PlatformType::SKSE64AE:
			{
				return "SKSE";
			}
			case PlatformType::F4SE:
			case PlatformType::F4SEVR:
			{
				return "F4SE";
			}
		};
		return {};
	}
	kxf::FSPath CommonExtenderPlatform::GetGameConfigPath() const
	{
		auto path = kxf::Shell::GetKnownDirectory(kxf::KnownDirectoryID::Documents) / "My Games";
		switch (m_PlatformType)
		{
			case PlatformType::MWSE:
			{
				return path / "Morrowind";
			}
			case PlatformType::OBSE:
			{
				return path / "Oblivion";
			}
			case PlatformType::FOSE:
			{
				return path / "Fallout3";
			}
			case PlatformType::NVSE:
			{
				return path / "FalloutNV";
			}
			case PlatformType::SKSE:
			{
				return path / "Skyrim";
			}
			case PlatformType::SKSE64:
			case PlatformType::SKSE64AE:
			{
				return path / "Skyrim Special Edition";
			}
			case PlatformType::SKSEVR:
			{
				return path / "Skyrim VR";
			}
			case PlatformType::F4SE:
			{
				return path / "Fallout4";
			}
			case PlatformType::F4SEVR:
			{
				return path / "Fallout4VR";
			}
		};
		return {};
	}
	kxf::FSPath CommonExtenderPlatform::GetPlatformDirectoryPath() const
	{
		return kxf::NativeFileSystem::GetExecutingModuleRootDirectory() / "Data" / GetPlatformFolderName();
	}

	void CommonExtenderPlatform::InitializeLogger()
	{
		// Disable asserts as they're not useful here
		wxLog::DontCreateOnDemand();
		kxf::Log::EnableAsserts(false);

		if (auto fs = GetPlatformLogsDirectory())
		{
			m_LogStream = fs->OpenToWrite(m_Plugin->GetName() + ".log");
		}

		// Redirect the framework log to our own log file
		if (m_LogStream)
		{
			class LogTarget final: public wxLog
			{
				private:
					CommonExtenderPlatform& m_Platform;

				protected:
					void DoLogRecord(wxLogLevel level, const wxString& message, const wxLogRecordInfo& info) override
					{
						auto TranslateLevel = [](wxLogLevel level)
						{
							switch (level)
							{
								case wxLOG_Info:
								{
									return "Info";
								}
								case wxLOG_Error:
								{
									return "Error";
								}
								case wxLOG_Trace:
								{
									return "Trace";
								}
								case wxLOG_Debug:
								{
									return "Debug";
								}
								case wxLOG_Status:
								{
									return "Status";
								}
								case wxLOG_Message:
								{
									return "Message";
								}
								case wxLOG_Warning:
								{
									return "Warning";
								}
								case wxLOG_Progress:
								{
									return "Progress";
								}
								case wxLOG_FatalError:
								{
									return "FatalError";
								}
							};
							return "";
						};
						m_Platform.LogCategory(kxf::Format("Framework:{}", TranslateLevel(level)), message);
					}

				public:
					LogTarget(CommonExtenderPlatform& instance)
						:m_Platform(instance)
					{
					}
			};
			kxf::Log::SetActiveTarget(std::make_unique<LogTarget>(*this));
		}
		else
		{
			// Or disable it altogether
			kxf::Log::Enable(false);
		}
	}
	bool CommonExtenderPlatform::InitializeModules()
	{
		using kxf::NativeAPISet;

		auto& loader = kxf::NativeAPILoader::GetInstance();
		loader.LoadLibraries
		({
			NativeAPISet::NtDLL,
			NativeAPISet::Kernel32,
			NativeAPISet::KernelBase,
			NativeAPISet::User32,
			NativeAPISet::ShlWAPI,
			NativeAPISet::DbgHelp
		});

		wxModule::RegisterModules();
		if (!wxModule::InitializeModules())
		{
			Log<1>("Initializing framework: failed");
			return false;
		}
		return true;
	}

	// IExtenderPlatform
	xSE::PlatformType CommonExtenderPlatform::GetType() const
	{
		return m_PlatformType;
	}
	kxf::String CommonExtenderPlatform::GetName() const
	{
		switch (m_PlatformType)
		{
			case PlatformType::MWSE:
			{
				return "MWSE";
			}
			case PlatformType::OBSE:
			{
				return "OBSE";
			}
			case PlatformType::FOSE:
			{
				return "FOSE";
			}
			case PlatformType::NVSE:
			{
				return "NVSE";
			}
			case PlatformType::SKSE:
			{
				return "SKSE";
			}
			case PlatformType::SKSEVR:
			{
				return "SKSEVR";
			}
			case PlatformType::SKSE64:
			{
				return "SKSE64";
			}
			case PlatformType::SKSE64AE:
			{
				return "SKSE64AE";
			}
			case PlatformType::F4SE:
			{
				return "F4SE";
			}
			case PlatformType::F4SEVR:
			{
				return "F4SEVR";
			}
		};
		return {};
	}
	kxf::String CommonExtenderPlatform::GetFullName() const
	{
		auto result = GetGameName();
		if (!result.IsEmpty())
		{
			result += " Script Extender";
			return result;
		}
		return {};
	}
	kxf::String CommonExtenderPlatform::GetGameName() const
	{
		switch (m_PlatformType)
		{
			case PlatformType::MWSE:
			{
				return "Morrowind";
			}
			case PlatformType::OBSE:
			{
				return "Oblivion";
			}
			case PlatformType::FOSE:
			{
				return "Fallout 3";
			}
			case PlatformType::NVSE:
			{
				return "Fallout: New Vegas";
			}
			case PlatformType::SKSE:
			{
				return "Skyrim";
			}
			case PlatformType::SKSEVR:
			{
				return "Skyrim VR";
			}
			case PlatformType::SKSE64:
			{
				return "Skyrim Special Edition";
			}
			case PlatformType::SKSE64AE:
			{
				return "Skyrim Anniversary Edition";
			}
			case PlatformType::F4SE:
			{
				return "Fallout 4";
			}
			case PlatformType::F4SEVR:
			{
				return "Fallout 4 VR";
			}
		};
		return {};
	}
	kxf::Version CommonExtenderPlatform::GetVersion() const
	{
		auto FromInteger = [](uint32_t value)
		{
			return kxf::Version(kxf::DateTime().SetValue(value));
		};

		return FromInteger(xSE_PACKED_VERSION);
	}

	std::shared_ptr<kxf::IFileSystem> CommonExtenderPlatform::GetGameRootDirectory() const
	{
		if (!IsNull())
		{
			return std::make_shared<kxf::ScopedNativeFileSystem>(kxf::NativeFileSystem::GetExecutingModuleRootDirectory());
		}
		return nullptr;
	}
	std::shared_ptr<kxf::IFileSystem> CommonExtenderPlatform::GetGameDataDirectory() const
	{
		if (!IsNull())
		{
			return std::make_shared<kxf::ScopedNativeFileSystem>(kxf::NativeFileSystem::GetExecutingModuleRootDirectory() / "Data");
		}
		return nullptr;
	}
	std::shared_ptr<kxf::IFileSystem> CommonExtenderPlatform::GetPlatformDirectory() const
	{
		if (!IsNull())
		{
			return std::make_shared<kxf::ScopedNativeFileSystem>(GetPlatformDirectoryPath());
		}
		return nullptr;
	}
	std::shared_ptr<kxf::IFileSystem> CommonExtenderPlatform::GetPlatformPluginsDirectory() const
	{
		if (!IsNull())
		{
			return std::make_shared<kxf::ScopedNativeFileSystem>(GetPlatformDirectoryPath() / "Plugins");
		}
		return nullptr;
	}
	std::shared_ptr<kxf::IFileSystem> CommonExtenderPlatform::GetPlatformLogsDirectory() const
	{
		if (!IsNull())
		{
			return std::make_shared<kxf::ScopedNativeFileSystem>(GetGameConfigPath() / GetPlatformFolderName());
		}
		return nullptr;
	}

	bool CommonExtenderPlatform::Initialize(std::shared_ptr<IExtenderPlugin> plugin)
	{
		if (!m_Plugin)
		{
			m_Plugin = std::move(plugin);
			if (m_Plugin->QueryInterface(m_EvtHandler))
			{
				InitializeLogger();

				// Register modules
				Log("Initializing framework");
				return InitializeModules();
			}
			return false;
		}
		return true;
	}
	void CommonExtenderPlatform::Terminate()
	{
		if (m_Plugin)
		{
			m_Plugin = nullptr;
		}
	}

	void CommonExtenderPlatform::LogString(const kxf::String& category, kxf::String logString, size_t indent)
	{
		// Log to xSE target if supported and compatible
		#if xSE_HAS_LOG
		if (m_SEVersion == xSE_PACKED_VERSION)
		{
			auto logStringSE = kxf::Format("<{}>", m_Plugin ? m_Plugin->GetName() : "xSE PluginCore", logString);
			xSE_LOG("%s", logStringSE.nc_str());
		}
		#endif

		// Log to our own target
		if (m_LogStream && !logString.IsEmptyOrWhitespace())
		{
			kxf::IO::OutputStreamWriter writer(*m_LogStream);

			// Add category
			if (!category.IsEmpty())
			{
				logString.Prepend("> ");
				logString.Prepend(category);
				logString.Prepend('<');
			}

			const size_t indentChars = indent * 4;
			if (indentChars != 0)
			{
				logString.Prepend(' ', indentChars);
			}

			// Add timestamp
			auto timeStamp = kxf::DateTime::Now();
			kxf::String timeStampFormatted = kxf::Format("[{}:{:0>3}] ", timeStamp.FormatISOCombined(' '), timeStamp.GetMillisecond());
			if (indentChars != 0)
			{
				logString.Insert(indentChars, timeStampFormatted);
			}
			else
			{
				logString.Prepend(timeStampFormatted);
			}

			// Write and flush
			writer.WriteStringUTF8(logString.Append('\n'));
			m_LogStream->Flush();
		}
	}

	// CommonExtenderPlatform
	bool CommonExtenderPlatform::OnQuery(const void* seInterface, void* pluginInfo)
	{
		LogPlatform("[" _CRT_STRINGIZE(xSE_QUERYFUNCTION) "] On query plugin");
		if (m_QueryCalled)
		{
			LogPlatform<1>("OnQuery has already been called");
			return false;
		}
		m_QueryCalled = true;

		if (!m_Plugin)
		{
			LogPlatform<1>("Plugin is not initialized");
			return false;
		}

		if (seInterface)
		{
			LogPlatform<1>("Processing xSE interface");

			auto se = static_cast<const xSE_Interface*>(seInterface);
			m_SEVersion = xSE_INTERFACE_VERSION(se);
			m_SEInterface = seInterface;
			LogPlatform<1>("xSE runtime version: {}; compiled version: {}", m_SEVersion, static_cast<decltype(m_SEVersion)>(xSE_PACKED_VERSION));

			if (auto info = static_cast<PluginInfo*>(pluginInfo))
			{
				LogPlatform<1>("Processing plugin info");

				m_PluginName = m_Plugin->GetName();
				info->infoVersion = PluginInfo::kInfoVersion;
				info->name = m_PluginName.nc_str();
				info->version = static_cast<uint32_t>(m_Plugin->GetVersion().ToInteger());
			}

			auto flags = m_Plugin->GetFlags();
			LogPlatform<1>("Preparing to load plugin");

			if (m_SEVersion == xSE_PACKED_VERSION || flags.Contains(ExtenderPluginFlag::VersionIndependent))
			{
				if (flags.Contains(ExtenderPluginFlag::VersionIndependent))
				{
					LogPlatform<2>("The plugin reported itself as a version independent");
				}
				if (se->isEditor && !flags.Contains(ExtenderPluginFlag::AllowEditor))
				{
					LogPlatform<2>("Loading in editor mode disabled");
					return false;
				}

				m_PluginHandle = se->GetPluginHandle();
				if (!m_EvtHandler->ProcessEvent(InitializationEvent::EvtQuery))
				{
					LogPlatform<2>("The plugin didn't process the query event");
				}
				return true;
			}
			else
			{
				LogPlatform<1>("Runtime xSE version doesn't match the compiled version");
			}
		}
		return false;
	}
	bool CommonExtenderPlatform::OnLoad(const void* seInterface)
	{
		#if xSE_PLATFORM_SKSE64AE
		if (!m_QueryCalled && !OnQuery(seInterface, nullptr))
		{
			return false;
		}
		#endif

		LogPlatform("[" _CRT_STRINGIZE(xSE_LOADFUNCTION) "] On load plugin");
		if (m_LoadCalled)
		{
			LogPlatform<1>("OnLoad has already been called");
			return false;
		}
		m_LoadCalled = true;
		
		if (m_EvtHandler->ProcessEvent(InitializationEvent::EvtLoad))
		{
			return true;
		}
		else
		{
			LogPlatform<1>("The plugin didn't process the load event");
			return false;
		}
	}
}
