/**
 * @mainpage DynLoader
 *
 * @section Introduction
 *
 * DynLoader is a simple and lightweight C++11 library forked from libpdl, 
 * which provides a common cross-platform interface for creating and using 
 * dynamically loaded interface classes.
 *
 * Project home:@n
 * http://code.google.com/p/libdynloader @n
 *
 * Project downloads:@n
 * http://code.google.com/p/libdynloader/downloads @n
 *
 * Project repository:@n
 * http://libdynloader.googlecode.com/svn/trunk/ @n
 *
 *
 * @section Example
 *
 * Dynamically loaded class interface:
 * @code
 * class ITest : public DynLoader::DynClass
 * {
 * public:
 *	// Class functionality
 *	virtual void DoSomething() throw() = 0;
 *	// Declare this class as dynamically loaded
 *	DECLARE_DYN_CLASS(ITest)
 * };
 * @endcode
 *
 * Dynamically loaded class implementation:
 * @code
 * class TestClass1 : public ITest
 * {
 * public:
 *	// Class functionality
 * 	void DoSomething() throw()
 * 	{
 * 		// Do something
 * 	}
 * };
 * @endcode
 *
 * Dynamically loaded class usage:
 * @code
 * try
 * {
 * 	DynLoader::DynLoader & dynLoader = DynLoader::DynLoader::Instance();
 * 	ITest * instance =
 * 		dynLoader.GetClassInstance<ITest>("./TestClass.so", "TestClass");
 * 	instance->DoSomething();
 * }
 * catch(DynLoader::LoaderException & ex)
 * {
 * 	fprintf(stderr, "Loader exception: %s\n", ex.what());
 * }
 * @endcode
 *
 *
 * @section Status
 *
 * The library is under development and testing now.
 *
 *
 * @author Igor Semenov <igor@progz.ru>
 * @author Adam Gregoire <bsdunx@gmail.com>
 *
 * @version 0.5.0
 *
 * @par License:
 * Copyright (c) 2007-2008, Igor Semenov @n
 * Copyright (c) 2010-2014, Adam Gregoire @n
 *
 * All rights reserved. @n
 * @n
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: @n
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer. @n
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution. @n
 *     * Neither the name of Igor Semenov nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission. @n
 * @n
 * THIS SOFTWARE IS PROVIDED BY IGOR SEMENOV ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL IGOR SEMENOV BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. @n
 * @n
 */

#ifndef __DYNLOADER_HPP__
#define __DYNLOADER_HPP__

#include <platform.h>

#include "DynClass.hpp"
#include "LoaderException.hpp"

#include <memory>
#include <list>

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
#include <windows.h>
#elif PLATFORM_POSIX
#include <dlfcn.h>
#endif

/**
 * @namespace DynLoader
 */
namespace DynLoader
{

/* @brief DynLib forward declaration */
struct DynLib;

/**
 * @class DynLoader DynLoader.hpp <DynLoader.hpp>
 * @brief Dynamic module and interface loader
 */
class API_EXPORT DynLoader
{
private:
	dyn_string lastError;

	std::list<DynLib*> libs;

	/**
	 * @brief Open library
	 * @param libName - [in] library file name
	 * @return true - loaded successfully, false otherwise
	 */
	DynLib* OpenLib(const dyn_string& libName, bool resolveSymbols = true);

	/**
	 * @brief Close library
	 * @param lib - [in] reference to dynamic library instance
	 */
	void CloseLib(DynLib& lib);

	/**
	 * @brief Get symbol by name
	 * @param symbolName - [in] symbol name
	 * @return pointer to symbol, nullptr if not found
	 */
	DYN_SYMBOL GetSymbolByName(DynLib& lib, const DYN_CHAR* symbolName);

	/**
	 * @brief Get class instance
	 * @param lib - [in] reference a DynLib instance
	 * @param className - [in] class name
	 * @return pointer to DynClass instance
	 */
	DynClass* GetClassInstance(DynLib& lib, const dyn_string& className);

public:

	DynLib* GetLoadedLibrary(const dyn_string& libName);

	/* @brief Disable copy and default constructors */
	DynLoader(const DynLoader&) = delete;
	DynLoader& operator=(const DynLoader&) = delete;

	/**
	 * @brief Default constructor and destructor
	 */
	DynLoader();
	~DynLoader();

	/**
	 * @brief Create class instance
	 * @param libName - [in] library file name
	 * @param className - [in] class name
	 * @return class instance, 0 if failed
	 * Class must be derived from DynClass
	 */
	template<typename Class>
	Class* GetClassInstance(const dyn_string& libName, const dyn_string& className)
	{
		DynLib* lib = OpenLib(libName);

		return lib ? static_cast<Class*>(GetClassInstance(*lib, className)) : nullptr;
	}

	/**
	 * @brief Reset the dynamic loader
	 * Frees all class instances and unloads all libraries
	 */
	void Reset();

	/**
	 * @brief Destroy the dynamic loader
	 * Resets the loader to default state and initiates object destruction
	 */
	void Destroy();

	/**
	 * @brief Get last error description
	 * @return last error description
	 */
	const dyn_string& GetLastError();

}; // class DynLoader

/**
 * @brief DynClassEntry structure 
 * We store the name here to keep it out of the client implementation
 */
struct DynClassEntry
{
	dyn_string name;
	DynClass* instance;

	DynClassEntry(const dyn_string& name, DynClass* instance) :
			name(name), instance(instance)
	{
	}
};

/* @brief DynLib structure */
struct DynLib
{
	dyn_string name;
	DYN_HANDLE handle;
	std::list<DynClassEntry*> instances;
	DynLoader& loader;

	DynLib(const dyn_string& libName, DynLoader& loader, bool resolveSymbols) :
			name(libName), handle(nullptr), instances(), loader(loader)
	{
	
		handle =
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
				::LoadLibraryExA(name.c_str(), nullptr, resolveSymbols ? (DWORD)0 : DONT_RESOLVE_DLL_REFERENCES);
#elif PLATFORM_POSIX
				::dlopen(name.c_str(), RTLD_GLOBAL | (resolveSymbols ? RTLD_NOW : RTLD_LAZY));
#endif

		if(handle == nullptr)
			throw LoaderException("Could not open `" + libName + "`");
	}

	~DynLib()
	{
	
		for (auto entry : instances)
		{
			entry->instance->Destroy();
			delete entry;
		}
		instances.clear();

		bool closeSuccess = true;
		if(handle)
		{
			closeSuccess =
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
					(::FreeLibrary(handle) != FALSE);
#elif PLATFORM_POSIX
					(::dlclose(handle) == 0);
#endif

			if(!closeSuccess)
				throw LoaderException("Unable to close library: Error `" + loader.GetLastError() + "`");

			handle = nullptr;
		}
	}
	
	DynLib(const DynLib& lib);
	const DynLib& operator=(const DynLib& lib);
};

} // namespace DynLoader

#endif // __DYNLOADER_HPP__

