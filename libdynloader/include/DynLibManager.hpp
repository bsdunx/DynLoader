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

#ifndef __DYNLIBMANAGER_HPP__
#define __DYNLIBMANAGER_HPP__

#include <platform.h>
#include <string>
#include <unordered_map>

/**
 * @namespace PDL
 * @brief Portable Dynamic Loader
 */
namespace DynLoader
{

// FWD
class DynLib;

/**
 * @class DynLibManager DynLibManager.hpp <DynLibManager.hpp>
 * @brief Dynamic library instance manager
 */
class API_LOCAL DynLibManager
{

public:
	/**
	 * @brief Constructor
	 */
	DynLibManager();

	/**
	 * @brief Destructor
	 */
	~DynLibManager() throw();

	/**
	 * @brief Reset library manager
	 * Free all libraries and set initial state
	 */
	void Reset();

	/**
	 * @brief Get dynamic library instance
	 * @param libName - [in] library file name
	 * @return dynamic library
	 * @throw LoaderException - cannot load library
	 */
	DynLib & GetLibInstance(const PDL_CHAR * libName);

private:
	// FIXME back to vector since this container should not have more
	// than say 128 entries.
	std::unordered_map<pdl_string, DynLib *> libs_;

}; // class DynLibManager

} // namespace DynLoader

#endif // __DYNLIBMANAGER_HPP__

