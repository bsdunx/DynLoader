/**
 * Copyright (c) 2007-2008, Igor Semenov
 * Copyright (c) 2010-2011, Adam Gregoire
 *
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
 * @namespace DynLoader
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
	for(auto i(libs_.cbegin()), cend(libs_.cend()); i != cend; ++i)
	{
		if(i->second->Opened())
		{
			if(!i->second->Close())
				throw LoaderException("Unable to close `" + i->first);
		}
		delete i->second;
	}

	// Clear library map
	libs_.clear();
}

/**
 * @brief Get dynamic library instance
 * @param libName - [in] library file name
 * @return dynamic library instance
 */
DynLib & DynLibManager::GetLibInstance(const PDL_CHAR * libName)
{
	if(!libName)
		throw LoaderException("You must supply a library name");

	DynLib * lib = nullptr;
	auto const libInstance(libs_.find(libName));

	if(libInstance != libs_.cend())
		lib = libInstance->second;
	else
	{
		lib = new DynLib();
		if(lib == nullptr)
			throw LoaderException("Unable to create new DynLib");

		if(!lib->Open(libName, true))
		{
			delete lib;
			throw LoaderException("Cannot load library `" + pdl_string(libName) + 
					"`: " + lib->GetLastError());
		}
		libs_[libName] = lib;
	}
	
	if(!lib)
	{
		// Actually, this should never happen.
		// If it has happened, this means serious memory damage
		throw LoaderException("Internal error");
	}

	return *lib;
}

} // namespace DynLoader

