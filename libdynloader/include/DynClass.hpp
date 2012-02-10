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

#ifndef __DYNCLASS_HPP__
#define __DYNCLASS_HPP__

#include <platform.h>

/**
 * @namespace PDL
 * @brief Portable Dynamic Loader
 */
namespace DynLoader
{

/**
 * @class DynClass DynClass.hpp <DynClass.hpp>
 * @brief Common interface for all dynamically loaded classes
 */
class DynClass
{

public:
	/**
	 * @brief Get class name
	 * return class name
	 */
	virtual const PDL_CHAR * GetClassName() const throw() = 0;

	/**
	 * @brief Destroy class instance
	 */
	void Destroy() throw() { delete this; }

protected:
	/**
	 * @brief Destructor
	 */
	virtual ~DynClass() throw() { ;; }

}; // class DynClass

} // namespace PDL

/**
 * @def DECLARE_DYN_CLASS DynClass.hpp <DynClass.hpp>
 * @brief Declare dynamically loaded class
 * @param className - [in] name of the class
 * Class should be derived from DynClass
 */
#define DECLARE_DYN_CLASS(className) \
public: \
	virtual const PDL_CHAR * GetClassName() const throw() { return #className; }

/**
 * @def EXPORT_DYN_CLASS DynClass.hpp <DynClass.hpp>
 * @brief Export constructor for dynamically loaded class
 * @param className - [in] name of the class
 */
#define EXPORT_DYN_CLASS(className) \
extern "C" PDL_DECL DynLoader::DynClass * Create##className() \
{ \
	try { \
		return new className(); \
	} \
	catch(...) \
	{ \
		;; \
	} \
	return nullptr; \
}

#endif // __DYNCLASS_HPP__

