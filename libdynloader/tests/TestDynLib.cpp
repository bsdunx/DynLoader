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

#include <cstdio>
#include <cstring>

#include <DynClass.hpp>
#include <DynLib.hpp>
#include <LoaderException.hpp>
#include "UnitTest.hpp"

int main(int argc, char ** argv)
{
	if( argc != 2 )
	{
		fprintf(stderr, "ERROR: Usage %s <libName>\n", argv[0]);
		return -1;
	}

	DynLoader::DynLib lib1;
	DynLoader::DynLib lib2;
	DynLoader::DynLib lib3;
	
	// Test DynLib::Open()
	try
	{
		lib1.Open("SomeDefinitelyNotExistentFile");
		UNIT_TEST(false);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "Exception: %s, LastError: (%s)\n", 
		        ex.what(), lib1.GetLastError().c_str());
		UNIT_TEST(true);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}

	try
	{
		lib2.Open(argv[1]);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "Exception: %s, LastError: (%s)\n", 
		        ex.what(), lib2.GetLastError().c_str());
	}
	catch(...)
	{
		UNIT_TEST(false);
	}

	try
	{
		lib3.Open(0);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "Exception: %s, LastError: (%s)\n", 
		        ex.what(), lib3.GetLastError().c_str());
		UNIT_TEST(true);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}
	
	// Test DynLib::Opened()
	UNIT_TEST(!lib1.Opened());
	UNIT_TEST(lib2.Opened());
	
	// Test DynLib::GetInstance()
	try
	{
		lib1.GetInstance("Test1");
		UNIT_TEST(false);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "Exception: %s, LastError: (%s)\n", 
		        ex.what(), lib1.GetLastError().c_str());
		UNIT_TEST(true);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}
	
	try
	{
		lib2.GetInstance("NonExistentClass");
		UNIT_TEST(false);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "Exception: %s, LastError: (%s)\n", ex.what(), lib2.GetLastError().c_str());
		UNIT_TEST(true);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}
	
	try
	{
		lib2.GetInstance("Test1");
		lib2.GetInstance("Test2");
		UNIT_TEST(true);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "Exception: %s, LastError: (%s)\n", ex.what(), lib2.GetLastError().c_str());
		UNIT_TEST(false);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}
	
	try
	{
		auto &class1 = lib2.GetInstance("Test1");
		auto &class2 = lib2.GetInstance("Test1");
		UNIT_TEST(&class1 && &class2 && &class1 == &class2);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "Exception: %s, LastError: (%s)\n", ex.what(), lib2.GetLastError().c_str());
		UNIT_TEST(false);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}

	// Test DynLib::Close()
	UNIT_TEST(lib1.Close());
	UNIT_TEST(lib2.Close());

	return 0;
}

