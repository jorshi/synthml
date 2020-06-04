//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/module_win32.cpp
// Created by  : Steinberg, 08/2016
// Description : hosting module classes (win32 implementation)
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2019, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "module.h"
#include "optional.h"
#include "stringconvert.h"
#include <ShlObj.h>
#include <Windows.h>
#include <algorithm>
#include <experimental/filesystem>

#pragma comment(lib, "Shell32")

//------------------------------------------------------------------------
extern "C" {
typedef bool (PLUGIN_API* InitModuleFunc) ();
typedef bool (PLUGIN_API* ExitModuleFunc) ();
}

using namespace std::experimental;

//------------------------------------------------------------------------
namespace VST3 {
namespace Hosting {

//------------------------------------------------------------------------
namespace {

#if SMTG_OS_WINDOWS_ARM
#define USE_OLE 0
#if SMTG_PLATFORM_64
constexpr auto architectureString = "arm_64-win";
#else
constexpr auto architectureString = "arm-win";
#endif
#else
#define USE_OLE 1
#if SMTG_PLATFORM_64
constexpr auto architectureString = "x86_64-win";
#else
constexpr auto architectureString = "x86-win";
#endif
#endif

#if USE_OLE
//------------------------------------------------------------------------
struct Ole
{
	static Ole& instance ()
	{
		static Ole gInstance;
		return gInstance;
	}

private:
	Ole () { OleInitialize (nullptr); }
	~Ole () { OleUninitialize (); }
};
#endif // USE_OLE

//------------------------------------------------------------------------
class Win32Module : public Module
{
public:
	template <typename T>
	T getFunctionPointer (const char* name)
	{
		return reinterpret_cast<T> (GetProcAddress (module, name));
	}

	~Win32Module () override
	{
		factory = PluginFactory (nullptr);

		if (module)
		{
			if (auto dllExit = getFunctionPointer<ExitModuleFunc> ("ExitDll"))
				dllExit ();

			FreeLibrary ((HMODULE)module);
		}
	}

	bool load (const std::string& inPath, std::string& errorDescription) override
	{
		auto wideStr = StringConvert::convert (inPath);
		module = LoadLibraryW (reinterpret_cast<LPCWSTR> (wideStr.data ()));
		if (!module)
		{
			filesystem::path p (inPath);
			auto filename = p.filename ();
			p /= "Contents";
			p /= architectureString;
			p /= filename;
			wideStr = StringConvert::convert (p.string ());
			module = LoadLibraryW (reinterpret_cast<LPCWSTR> (wideStr.data ()));
			if (!module)
			{
				// TODO: is there an API to get more information about the failure ?
				errorDescription = "LoadLibray failed.";
				return false;
			}
		}

		auto dllEntry = getFunctionPointer<InitModuleFunc> ("InitDll");
		auto factoryProc = getFunctionPointer<GetFactoryProc> ("GetPluginFactory");
		if (!factoryProc)
		{
			errorDescription = "dll does not export the required 'GetPluginFactory' function";
			return false;
		}
		if (dllEntry && !dllEntry ())
		{
			errorDescription = "Calling 'InitDll' failed";
			return false;
		}
		auto f = Steinberg::FUnknownPtr<Steinberg::IPluginFactory> (owned(factoryProc ()));
		if (!f)
		{
			errorDescription = "Calling 'GetPluginFactory' returned nullptr";
			return false;
		}
		factory = PluginFactory (f);
		return true;
	}

	HINSTANCE module {nullptr};
};

//------------------------------------------------------------------------
bool checkVST3Package (filesystem::path& p)
{
	auto filename = p.filename ();
	p /= "Contents";
	p /= architectureString;
	p /= filename;
	auto hFile = CreateFileW (reinterpret_cast<LPCWSTR> (p.c_str ()), GENERIC_READ, FILE_SHARE_READ,
	                          NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle (hFile);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
bool isFolderSymbolicLink (const filesystem::path& p)
{
	std::wstring wString = p.generic_wstring ();
	auto attrib = GetFileAttributesW (reinterpret_cast<LPCWSTR> (wString.c_str ()));
	if (attrib & FILE_ATTRIBUTE_REPARSE_POINT)
	{
		auto hFile = CreateFileW (reinterpret_cast<LPCWSTR> (wString.c_str ()), GENERIC_READ,
		                          FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return true;
		else
			CloseHandle (hFile);
	}
	return false;
}

//------------------------------------------------------------------------
Optional<std::string> getKnownFolder (REFKNOWNFOLDERID folderID)
{
	PWSTR wideStr {};
	if (FAILED (SHGetKnownFolderPath (folderID, 0, nullptr, &wideStr)))
		return {};
	return StringConvert::convert (wideStr);
}

//------------------------------------------------------------------------
VST3::Optional<filesystem::path> resolveShellLink (const filesystem::path& p)
{
#if USE_OLE
	Ole::instance ();

	IShellLink* shellLink = nullptr;
	if (!SUCCEEDED (CoCreateInstance (CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
	                                  IID_IShellLink, reinterpret_cast<LPVOID*> (&shellLink))))
		return {};

	IPersistFile* persistFile = nullptr;
	if (!SUCCEEDED (
	        shellLink->QueryInterface (IID_IPersistFile, reinterpret_cast<void**> (&persistFile))))
		return {};

	if (!SUCCEEDED (persistFile->Load (p.native ().data (), STGM_READ)))
		return {};

	if (!SUCCEEDED (shellLink->Resolve (nullptr, MAKELONG (SLR_NO_UI, 500))))
		return {};

	WCHAR resolvedPath[MAX_PATH];
	if (!SUCCEEDED (shellLink->GetPath (resolvedPath, MAX_PATH, nullptr, SLGP_SHORTPATH)))
		return {};

	std::wstring longPath;
	longPath.resize (MAX_PATH);
	auto numChars =
	    GetLongPathNameW (resolvedPath, const_cast<wchar_t*> (longPath.data ()), MAX_PATH);
	if (!numChars)
		return {};
	longPath.resize (numChars);

	persistFile->Release ();
	shellLink->Release ();

	return {filesystem::path (longPath)};
#else
	// TODO for ARM
	return {};
#endif
}

//------------------------------------------------------------------------
void findFilesWithExt (const filesystem::path& path, const std::string& ext,
                       Module::PathList& pathList, bool recursive = true)
{
	for (auto& p : filesystem::directory_iterator (path))
	{
		const auto& cp = p.path ();
		const auto& cpExt = cp.extension ();
		if (cpExt == ext)
		{
			if ((p.status ().type () == filesystem::file_type::directory) ||
			    isFolderSymbolicLink (p))
			{
				filesystem::path finalPath (p);
				if (checkVST3Package (finalPath))
				{
					pathList.push_back (finalPath.generic_u8string ());
					continue;
				}
				findFilesWithExt (cp, ext, pathList, recursive);
			}
			else
				pathList.push_back (cp.generic_u8string ());
		}
		else if (recursive)
		{
			if (p.status ().type () == filesystem::file_type::directory)
			{
				findFilesWithExt (cp, ext, pathList, recursive);
			}
			else if (cpExt == ".lnk")
			{
				if (auto resolvedLink = resolveShellLink (cp))
				{
					if (resolvedLink->extension () == ext)
					{
						if (filesystem::is_directory (*resolvedLink) || isFolderSymbolicLink (*resolvedLink))
						{
							filesystem::path finalPath (*resolvedLink);
							if (checkVST3Package (finalPath))
							{
								pathList.push_back (finalPath.generic_u8string ());
								continue;
							}
							findFilesWithExt (*resolvedLink, ext, pathList, recursive);
						}
						else
							pathList.push_back (resolvedLink->generic_u8string ());
					}
					else if (filesystem::is_directory (*resolvedLink))
					{
						const auto& str = resolvedLink->generic_u8string ();
						if (cp.generic_u8string ().compare (0, str.size (), str.data (),
						                                    str.size ()) != 0)
							findFilesWithExt (*resolvedLink, ext, pathList, recursive);
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void findModules (const filesystem::path& path, Module::PathList& pathList)
{
	findFilesWithExt (path, ".vst3", pathList);
}

//------------------------------------------------------------------------
Optional<filesystem::path> getContentsDirectoryFromModuleExecutablePath (const std::string& modulePath)
{
	filesystem::path path (modulePath);
	path = path.parent_path ();
	if (path.filename () != architectureString)
		return {};
	path = path.parent_path ();
	if (path.filename () != "Contents")
		return {};

	return Optional<filesystem::path> {std::move (path)};
}

//------------------------------------------------------------------------
} // anonymous

//------------------------------------------------------------------------
Module::Ptr Module::create (const std::string& path, std::string& errorDescription)
{
	auto module = std::make_shared<Win32Module> ();
	if (module->load (path, errorDescription))
	{
		module->path = path;
		auto it = std::find_if (path.rbegin (), path.rend (),
		                        [] (const std::string::value_type& c) { return c == '/'; });
		if (it != path.rend ())
			module->name = {it.base (), path.end ()};
		return module;
	}
	return nullptr;
}

//------------------------------------------------------------------------
Module::PathList Module::getModulePaths ()
{
	// find plug-ins located in common/VST3
	PathList list;
	if (auto knownFolder = getKnownFolder (FOLDERID_ProgramFilesCommon))
	{
		filesystem::path p (*knownFolder);
		p.append ("VST3");
		findModules (p, list);
	}

	// find plug-ins located in VST3 (application folder)
	WCHAR modulePath[MAX_PATH + 1];
	GetModuleFileNameW (nullptr, modulePath, MAX_PATH);
	auto appPath = StringConvert::convert (modulePath);
	filesystem::path path (appPath);
	path = path.parent_path ();
	path = path.append ("VST3");
	findModules (path, list);

	return list;
}

//------------------------------------------------------------------------
Module::SnapshotList Module::getSnapshots (const std::string& modulePath)
{
	SnapshotList result;
	auto path = getContentsDirectoryFromModuleExecutablePath (modulePath);
	if (!path)
		return result;

	*path /= "Resources";
	*path /= "Snapshots";

	PathList pngList;
	findFilesWithExt (*path, ".png", pngList, false);
	for (auto& png : pngList)
	{
		filesystem::path p (png);
		auto filename = p.filename ().generic_u8string ();
		auto uid = Snapshot::decodeUID (filename);
		if (!uid)
			continue;
		auto scaleFactor = 1.;
		if (auto decodedScaleFactor = Snapshot::decodeScaleFactor (filename))
			scaleFactor = *decodedScaleFactor;

		Module::Snapshot::ImageDesc desc;
		desc.scaleFactor = scaleFactor;
		desc.path = std::move (png);
		bool found = false;
		for (auto& entry : result)
		{
			if (entry.uid != *uid)
				continue;
			found = true;
			entry.images.emplace_back (std::move (desc));
			break;
		}
		if (found)
			continue;
		Module::Snapshot snapshot;
		snapshot.uid = *uid;
		snapshot.images.emplace_back (std::move (desc));
		result.emplace_back (std::move (snapshot));
	}
	return result;
}

//------------------------------------------------------------------------
} // Hosting
} // VST3
