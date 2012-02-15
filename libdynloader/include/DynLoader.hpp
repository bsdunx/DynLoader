/**
 * @mainpage libDynLoader
 *
 * @section Introduction
 *
 * libdynloader is a simple and lightweight C++ library forked from libpdl, 
 * which provides common cross-platform interface for creating and using 
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
 *	/// Class functionality
 *	virtual void DoSomething() throw() = 0;
 *	/// Declare this class as dynamically loaded
 *	DECLARE_DYN_CLASS(ITest)
 * };
 * @endcode
 *
 * Dynamically loaded class implementation:
 * @code
 * class TestClass1 : public ITest
 * {
 * public:
 *	/// Class functionality
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
 * @version 0.0.1
 *
 * @par License:
 * Copyright (c) 2007-2008, Igor Semenov @n
 * Copyright (c) 2010-2011, Adam Gregoire @n
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
#include <memory>
#include <vector>

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
#include <windows.h>
#endif

/**
 * @namespace DynLoader
 */
namespace DynLoader
{

/* @brief Forward declarations */
class DynClass;

struct DynLibData
{
	pdl_string libName;
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
	HMODULE libHandle;
#elif PLATFORM_POSIX
	void * libHandle;
#endif
	std::vector<DynClass *> instances;

	DynLibData(const pdl_string& libName) : libName(libName), libHandle(nullptr)
	{
	}
};

/**
 * @class DynLoader DynLoader.hpp <DynLoader.hpp>
 * @brief Dynamic loader
 * @todo Replace class export with a factory function which has C linkage.
 */
class API_EXPORT DynLoader
{

private:
	// Forbid copy constructor and assignment operator
	DynLoader(const DynLoader &);
	DynLoader & operator= (const DynLoader &);

	pdl_string lastError;

	std::vector<DynLibData *> libs;

public:

	DynLoader();
	~DynLoader();

	/**
	 * @brief Create class instance
	 * @param libName - [in] library file name
	 * @param className - [in] class name
	 * @return class instance, 0 if failed
	 * Class should be derived from DynamicClass
	 */
	template<typename Class>
	Class * GetClassInstance(const PDL_CHAR * libName, const PDL_CHAR * className)
	{
		DynLibData* lib = nullptr;

		lib = GetLibInstance(libName);

		return static_cast<Class *>(GetClassInstance(*lib, className));
	}

	/**
	 * @brief Reset dynamic loader
	 * Unload all loaded libraries and free instances
	 */
	void Reset();

		/**
	 * @brief Open library
	 * @param libName - [in] library file name
	 * @return true - loaded successfully, false otherwise
	 */
	DynLibData * OpenLib(const PDL_CHAR * libName, bool resolveSymbols = true);

	/**
	 * @brief Close library
	 * @return true if closed successfully, false otherwise
	 */
	bool CloseLib(DynLibData& lib);

	/**
	 * @brief Get class instance
	 * @param className - [in] class name
	 * @return pointer to class instance
	 */
	DynClass * GetClassInstance(DynLibData& lib, const pdl_string& className);

	/**
	 * @brief Get symbol by name
	 * @param symbolName - [in] symbol name
	 * @return pointer to symbol, 0 if not found
	 */
	PDL_SYMBOL GetSymbolByName(DynLibData& lib, const PDL_CHAR * symbolName);

	DynLibData * GetLibInstance(const PDL_CHAR * libName);

	/**
	 * @brief Get last error description
	 * @return last error description
	 */
	const pdl_string & GetLastError();

	/**
	 * @brief Clear last retrieved error description
	 * 
	 */
	void ClearLastError();

}; // class DynLoader

} // namespace DynLoader

#endif // __DYNLOADER_HPP__
