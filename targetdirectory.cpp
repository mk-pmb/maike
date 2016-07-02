//@	{"targets":[{"name":"targetdirectory.o","type":"object"}]}

#include "targetdirectory.hpp"
#include "fileutils.hpp"
#include <cstring>

using namespace Maike;

void TargetDirectory::compile(Twins<const Dependency*> dependency_list
	,const char* target_dir)
	{
	std::string fullpath=target_dir;
	auto name=nameGet();
	if(strcmp(name,"."))
		{
		fullpath+='/';
		fullpath+=name;
		}
	FileUtils::mkdir(fullpath.c_str());
	}

bool TargetDirectory::upToDate(Twins<const Dependency*> dependency_list
	,const char* target_dir) const
	{
	std::string fullpath=target_dir;
	auto name=nameGet();
	if(strcmp(name,"."))
		{
		fullpath+='/';
		fullpath+=name;
		}
	return FileUtils::exists(fullpath.c_str());
	}

void TargetDirectory::destroy() noexcept
	{delete this;}

TargetDirectory* TargetDirectory::create(const char* name,const char* in_dir
	,size_t id)
	{return new TargetDirectory(name,in_dir,id);}
