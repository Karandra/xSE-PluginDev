#include "pch.hpp"
#include "CommonExtenderPlatform.h"
#include "ScriptExtenderDefinesBase.h"
#include "ScriptExtenderDefinesExtra.h"
#include "ScriptExtenderInterfaceIncludes.h"

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
			case PlatformType::SKSE64:
			case PlatformType::SKSEVR:
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
						m_Platform.Log("<Framework:{}> {}", TranslateLevel(level), message);
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
			case PlatformType::SKSE64:
			{
				return "SKSE64";
			}
			case PlatformType::SKSEVR:
			{
				return "SKSEVR";
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
			case PlatformType::SKSE64:
			{
				return "SkyrimSE";
			}
			case PlatformType::SKSEVR:
			{
				return "SkyrimVR";
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
			InitializeLogger();
					
			// Register modules
			Log("Initializing framework");
			return InitializeModules();
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

	void CommonExtenderPlatform::LogString(kxf::String logString, size_t indent)
	{
		// Log to xSE target if supported
		#if xSE_HAS_LOG
		{
			auto logStringSE = kxf::Format("<{}>", GetName(), logString);
			xSE_LOG("%s", logStringSE.nc_str());
		}
		#endif

		// Log to our own target
		if (m_LogStream && !logString.IsEmptyOrWhitespace())
		{
			kxf::IO::OutputStreamWriter writer(*m_LogStream);

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
}
