
//@	{"targets":[{"name":"targetdirectory.o","type":"object"}]}

#include "targetdirectory.hpp"
#include "fileutils.hpp"
#include <cstring>

using namespace Maike;

void TargetDirectory::compileImpl(Twins<const Dependency*> dependency_list
	,Twins<const Dependency*> dependency_list_full
	,const char* target_dir
	,FileUtils& fileutils)
	{
	std::string fullpath=target_dir;
	auto name=nameGet();
	if(strcmp(name,"."))
		{
		fullpath+='/';
		fullpath+=name;
		}
	fileutils.mkdir(fullpath.c_str());
	}

bool TargetDirectory::upToDate(Twins<const Dependency*> dependency_list
	,Twins<const Dependency*> dependency_list_full
	,const char* target_dir
	,FileUtils& fileutils) const
	{
	std::string fullpath=target_dir;
	auto name=nameGet();
	if(strcmp(name,"."))
		{
		fullpath+='/';
		fullpath+=name;
		}
	return fileutils.exists(fullpath.c_str());
	}

void TargetDirectory::destroy() noexcept
	{delete this;}

TargetDirectory* TargetDirectory::create(const char* name,const char* in_dir
	,const char* root,size_t id)
	{return new TargetDirectory(name,in_dir,root,id);}
