/**
 * Copyright (c) 2007-2008, Igor Semenov
 * Copyright (c) 2010-2011, Adam Gregoire
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
#include <DynLib.hpp>
#include <LoaderException.hpp>

#include <string>
#include <cassert>

#if PLATFORM_POSIX
#  include <dlfcn.h>
#endif

/**
 * @namespace DynLoader
 * @brief DynLoader namespace
 */
namespace DynLoader
{

/**
 * @brief Dynamic instance builder
 */
typedef DynClass * (*DynBuilder)();

/**
 * @brief Constructor
 */
DynLib::DynLib() : lib_(nullptr), libName_(), instances_()
{
	;;
}

/**
 * @brief Destructor
 */
DynLib::~DynLib() throw()
{
	(void) Close();
}
	
/**
 * @brief Open library
 * @param libName - [in] library file name
 * @return true - loaded successfully, false otherwise
 *
 * @todo Add path alteration for windows. eBoLA
 */
bool DynLib::Open(const PDL_CHAR * libName, bool resolveSymbols)
{
	if(!libName)
		return false;

	lib_ =
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
		::LoadLibraryExA(libName, NULL, resolveSymbols ? (DWORD)0 : DONT_RESOLVE_DLL_REFERENCES);
#elif PLATFORM_POSIX
		::dlopen(libName, RTLD_GLOBAL | (resolveSymbols ? RTLD_NOW : RTLD_LAZY));
#endif

	if(lib_)
		libName_ = libName;
	else
		throw LoaderException(pdl_string("Could not open `") + pdl_string(libName) + "`");

	return !!lib_;
}

/**
 * @brief Get default library filename extension (platform-specific)
 * @return default library filename extension
 */
const PDL_CHAR * DynLib::GetDefaultExt()
{
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
	return ".dll";
#elif PLATFORM_POSIX
	return ".so";
#endif
}

/**
 * @brief Get class instance
 * @param className - [in] class name
 * @return pointer to class instance
 */
DynClass & DynLib::GetInstance(const PDL_CHAR * className)
{
	auto const loadedInstance(instances_.find(className));
	if(loadedInstance != instances_.cend())
		return *(loadedInstance->second);
	
	auto const builderName(pdl_string("Create") + pdl_string(className));
	
	auto symbol = GetSymbolByName(builderName.c_str());
	if(!symbol)
	{
		instances_[className] = nullptr;
		throw LoaderException(pdl_string("Class `") + pdl_string(className) +
		                      pdl_string("` not found in ") + libName_);
	}

	// POSIX guarantees that the size of a pointer to object is equal to 
	// the size of a pointer to a function.
	//DynBuilder builder = nullptr;
	//builder = static_cast<DynBuilder>(symbol);
	DynClass *instance(static_cast<DynClass *>(symbol));

	instances_[className] = instance;

	return *instance;
}

/**
 * @brief Get symbol by name
 * @param symbolName - [in] symbol name
 * @return pointer to symbol, 0 if not found
 */
PDL_SYMBOL DynLib::GetSymbolByName(const PDL_CHAR * symbolName)
{
	if(!lib_)
		throw LoaderException(pdl_string("Library is not opened"));

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
	return static_cast<void *>(::GetProcAddress(lib_, symbolName));
#elif PLATFORM_POSIX
	return ::dlsym(lib_, symbolName);
#endif
}

/**
 * @brief Get last error description
 * @return last error description
 */
pdl_string DynLib::GetLastError() const
{
	try
	{
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
		LPSTR lpMsgBuf = NULL;
		const DWORD res =
			::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			                  NULL, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                          lpMsgBuf, 0, nullptr);
		pdl_string errorText;
		if(res != 0)
		{
			errorText.assign(lpMsgBuf);
			(void) ::LocalFree(lpMsgBuf);
		}
#elif PLATFORM_POSIX
		const pdl_string errorText(dlerror());
#endif
		return errorText;
	}
	catch (...)
	{
		return pdl_string("Exception attempting to get last error.");
	}
}

/**
 * @brief Close library
 * @return true if closed successfully, false otherwise
 */
bool DynLib::Close()
{
	for(auto it(instances_.cbegin()), cend(instances_.cend()); it != cend; ++it)
	{
		it->second->Destroy();
	}
	instances_.clear();

	bool closeSuccess = true;
	if(lib_)
	{
		closeSuccess = 
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
				(::FreeLibrary(lib_) != FALSE);
#elif PLATFORM_POSIX
				(::dlclose(lib_) == 0);
#endif
		lib_ = nullptr;
	}

	libName_.clear();

	if(!closeSuccess)
		GetLastError();

	return closeSuccess;
}

} // namespace DynLoader

