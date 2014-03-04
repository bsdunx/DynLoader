/**
 * Copyright (c) 2007-2008, Igor Semenov
 * Copyright (c) 2010-2014, Adam Gregoire
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Igor Semenov nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY IGOR SEMENOV ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL IGOR SEMENOV BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <platform.h>

#include <DynClass.hpp>
#include <DynLoader.hpp>
#include <LoaderException.hpp>

#include <memory>
#include <algorithm>
#include <functional>

#include <cassert>

#ifdef PLATFORM_POSIX
#include <dlfcn.h>
#endif

/**
 * @namespace DynLoader
 */
namespace DynLoader
{

DynLoader::DynLoader() : lastError(), libs()
{
}

DynLoader::~DynLoader()
{
	Reset();
}

/**
 * @brief Retrieves an instance of a loaded library
 * @param libName - [in] library file name
 * @return lib - dynamic library
 */
DynLib* DynLoader::GetLoadedLibrary(const dyn_string& libName)
{
	for (auto lib : libs)
	{
		if (lib->name == libName)
			return lib;
	}

	return nullptr;
}

/**
 * @brief Open library
 * @param libName - [in] library file name
 * @return pointer to dynamic library, nullptr if not found or unable to 
 * create
 *
 * @todo Add path alteration for windows.
 */
DynLib* DynLoader::OpenLib(const dyn_string& libName, bool resolveSymbols)
{
	DynLib* lib = GetLoadedLibrary(libName);
	if (lib != nullptr)
		return lib;

	lib = new DynLib(libName, *this, resolveSymbols);
	if (lib == nullptr)
		return nullptr;

	libs.push_back(lib);

	return lib;
}

/**
 * @brief Returns a class instance from an instanced library
 * @param lib - [in] dynamic library instance
 * @param className - [in] class name
 * @return pointer to class instance
 */
DynClass* DynLoader::GetClassInstance(DynLib& lib, const dyn_string& className)
{
	for(auto entry : lib.instances)
	{
		if(entry->name == className)
			return entry->instance;
	}

	dyn_string builderName("Create" + className);

	// POSIX guarantees that the size of a pointer to object is equal to 
	// the size of a pointer to a function. On Windows NT systems this is also a safe 
	// assumption.
	std::function<DynClass*()> builder(std::bind(
			reinterpret_cast<DynClass*(*)()>(GetSymbolByName(lib, builderName.c_str()))));
	if(builder == nullptr)
		throw LoaderException("Factory builder `" + builderName + 
				"` for Class `" + className +
				"` not found in " + lib.name);

	// Create an instance of the class
	auto instance = builder();
	if(instance == nullptr)
		throw LoaderException("Unable to create instance of class `" + className + "`");
	
	auto entry = new DynClassEntry;
	entry->name = className;
	entry->instance = instance;

	lib.instances.push_back(entry);

	return instance;
}

/**
 * @brief Get last error description
 * @return last error description
 */
const dyn_string& DynLoader::GetLastError()
{
	lastError.clear();

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
	LPSTR lpMsgBuf = nullptr;
	const DWORD res = ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|
			FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, ::GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			lpMsgBuf, 0, nullptr);

	if(res != 0)
	{
		lastError.assign(lpMsgBuf);
		(void) ::LocalFree(lpMsgBuf);
	}
#elif PLATFORM_POSIX
	const char* dlerr = ::dlerror();
	if(dlerr != nullptr)
		lastError.assign(dlerr);
#endif

	return lastError;
}

/**
 * @brief Close library
 * @param lib - [in] dynamic library instance
 * @return void
 */
void DynLoader::CloseLib(DynLib& lib)
{
	libs.remove(&lib);

	delete &lib;
}

/**
 * @brief Get symbol by name
 * @param symbolName - [in] symbol name
 * @return pointer to symbol, 0 if not found
 */
DYN_SYMBOL DynLoader::GetSymbolByName(DynLib& lib, const DYN_CHAR * symbolName)
{
	return
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
		reinterpret_cast<void *>(::GetProcAddress(lib.handle, symbolName));
#elif PLATFORM_POSIX
		::dlsym(lib.handle, symbolName);
#endif
}

/**
 * @brief Reset the dynamic loader
 * Free all libraries and set initial state
 */
void DynLoader::Reset()
{
	// Free all libraries
	for(auto lib : libs)
		delete lib;

	libs.clear();
}

/**
 * @brief Destroy the dynamic loader
 * Dynamic loader shutdown routine
 */
void DynLoader::Destroy()
{
	delete this;
}

} // namespace DynLoader

