/**
 * Copyright (c) 2007-2008, Igor Semenov
 * Copyright (c) 2010-2012, Adam Gregoire
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

#include <cstdio>

#include <platform.h>

#include "UnitTest.hpp"

#include <DynLoader.hpp>
#include <LoaderException.hpp>
#include "TestInterface.hpp"

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage %s <libName> <className1> [<className2>...]\n", argv[0]);
		return 1;
	}

	try
	{
		DynLoader::DynLoader* dynLoader = new DynLoader::DynLoader;
		UNIT_TEST(true);
		
		dynLoader->Reset();
		UNIT_TEST(true);
	
		for(int i = 2; i < argc; ++i)
		{
			auto instance = dynLoader->GetClassInstance<DynLoader::ITest>(DynLoader::dyn_string(argv[1]), DynLoader::dyn_string(argv[i]));
			instance->DoSomething();
			UNIT_TEST(true);
		}
		
		dynLoader->Reset();
		UNIT_TEST(true);
		
		for(int i = 2; i < argc; ++i)
		{
			auto instance = dynLoader->GetClassInstance<DynLoader::ITest>(DynLoader::dyn_string(argv[1]), DynLoader::dyn_string(argv[i]));
			instance->DoSomething();
			UNIT_TEST(true);
		}

		dynLoader->Reset();
		UNIT_TEST(true);

		dynLoader->Reset();
		UNIT_TEST(true);

		dynLoader->Destroy();
		UNIT_TEST(true);
	}
	catch(DynLoader::LoaderException& ex)
	{
		fprintf(stderr, "Loader exception: %s\n", ex.what());
		UNIT_TEST(false);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}

	return 0;
}

