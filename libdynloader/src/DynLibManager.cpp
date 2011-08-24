/**
 * Copyright (c) 2007-2008, Igor Semenov
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
#include <DynLib.hpp>
#include <DynLibManager.hpp>
#include <LoaderException.hpp>

#include <cassert>

/**
 * @namespace PDL
 * @brief Portable Dynamic Loader
 */
namespace DynLoader
{

/**
 * @brief Constructor
 */
DynLibManager::DynLibManager() : libs_()
{
	;;
}

/**
 * @brief Destructor
 */
DynLibManager::~DynLibManager() throw()
{
	try
	{
		Reset();
	}
	catch(...)
	{
		;;
	}
}

/**
 * @brief Reset library manager
 * Free all libraries and set initial state
 */
void DynLibManager::Reset()
{
	// Free all libraries
	const DynLibManager::LibMap::iterator cend(libs_.end());
	for(auto i(libs_.begin()); i != cend; ++i)
	{
		DynLib * lib = i->second;

		assert(lib && lib->Opened());

		lib->Close();
		delete lib;
	}

	// Clear library map
	libs_.clear();
}

/**
 * @brief Get dynamic library
 * @param libName - [in] library file name
 * @return dynamic library
 * @throw LoaderException - cannot load library
 */
DynLib & DynLibManager::GetLib(const PDL_CHAR * libName)
{
	auto &lib = GetLibInstance(libName);

	if(!lib.Opened())
		throw LoaderException("`" + pdl_string(libName) + pdl_string("` is not opened."));

	return lib;
}

/**
 * @brief Get dynamic library instance
 * @param libName - [in] library file name
 * @return dynamic library instance
 */
DynLib & DynLibManager::GetLibInstance(const PDL_CHAR * libName)
{
	if(!libName)
		throw LoaderException(pdl_string("Library name is 0"));

	DynLib * lib = nullptr;
	auto const libInstance(libs_.find(libName));

	if(libInstance != libs_.cend())
		lib = libInstance->second;
	else
	{
		lib = new DynLib();
		if(lib && !lib->Open(libName, true))
		{
			delete lib;
			throw LoaderException(pdl_string("Cannot load library `") +
			                       pdl_string(libName) + pdl_string("`: ") +
			                       lib->GetLastError());
		}
		libs_[libName] = lib;
	}
	
	if(!lib)
	{
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
		VC_DBG_BREAK
#endif
		// Actually, this should never happen.
		// If it has happened, this means serious memory damage
		throw LoaderException(pdl_string("Internal error"));
	}

	return *lib;
}

} // namespace PDL

