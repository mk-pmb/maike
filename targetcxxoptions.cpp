//@	 {"targets":[{"name":"targetcxxoptions.o","type":"object"}]}

#include "targetcxxoptions.hpp"
#include "resourceobject.hpp"

using namespace Maike;

static std::vector< std::string > stringArrayGet(const ResourceObject& array)
	{
	std::vector<std::string> ret;
	auto N=array.objectCountGet();
	for(decltype(N) k=0;k<N;++k)
		{ret.push_back(std::string(static_cast<const char*>(array.objectGet(k))));}
	return std::move(ret);
	}

TargetCxxOptions::TargetCxxOptions():m_fields_valid(0){}

TargetCxxOptions::TargetCxxOptions(const ResourceObject& cxxoptions):
	m_includedir_format("-I\"^\""),m_libdir_format("-L\"^\""),m_cxxversion_min(0)
	,m_stdprefix("-std=")
	{
	if(cxxoptions.objectExists("includedir"))
		{m_includedir=stringArrayGet( cxxoptions.objectGet("includedir") );}

	if(cxxoptions.objectExists("includedir_format"))
		{m_includedir_format=std::string(static_cast<const char*>(cxxoptions.objectGet("includedir_format")));}

	if(cxxoptions.objectExists("libdir"))
		{m_libdir=stringArrayGet( cxxoptions.objectGet("libdir") );}

	if(cxxoptions.objectExists("libdir_format"))
		{m_includedir_format=std::string(static_cast<const char*>(cxxoptions.objectGet("libdir_format")));}

	if(cxxoptions.objectExists("cxxversion_min"))
		{m_cxxversion_min=static_cast<long long int>( cxxoptions.objectGet("cxxversion_min") );}

	if(cxxoptions.objectExists("stdprefix"))
		{m_stdprefix=std::string( static_cast<const char*>(cxxoptions.objectGet("stdprefix")) );}

	if(cxxoptions.objectExists("platform_suffix"))
		{m_platform_suffix=std::string( static_cast<const char*>(cxxoptions.objectGet("platform_suffix")) );}

	if(cxxoptions.objectExists("libext_format"))
		{m_libext_format=std::string(static_cast<const char*>(cxxoptions.objectGet("libext_format")));}

	if(cxxoptions.objectExists("libint_format"))
		{m_libint_format=std::string(static_cast<const char*>(cxxoptions.objectGet("libint_format")));}

	if(cxxoptions.objectExists("objcompile"))
		{m_objcompile=Command(cxxoptions.objectGet("objcompile"));}

	if(cxxoptions.objectExists("appcompile"))
		{m_appcompile=Command(cxxoptions.objectGet("appcompile"));}

	if(cxxoptions.objectExists("dllcompile"))
		{m_dllcompile=Command(cxxoptions.objectGet("dllcompile"));}

	if(cxxoptions.objectExists("libcompile"))
		{m_dllcompile=Command(cxxoptions.objectGet("libcompile"));}

	if(cxxoptions.objectExists("versionquery"))
		{m_versionquery=Command(cxxoptions.objectGet("versionquery"));}
	}