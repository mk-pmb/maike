//@	{
//@	"targets":[{"name":"targetdirectoryloader.o","type":"object"}]
//@	}

#include "targetdirectoryloader.hpp"
#include "directorylister.hpp"
#include "fileinfo.hpp"
#include "spider.hpp"
#include "targetdirectory.hpp"
#include "dependencygraph.hpp"
#include <string>

#include <cstdio>

using namespace Maike;

TargetDirectoryLoader::TargetDirectoryLoader():m_recursive(1)
	{
	m_ignore.insert(Stringkey("."));
	m_ignore.insert(Stringkey(".."));
	}

void TargetDirectoryLoader::targetsLoad(const char* name_src
	,const char* in_dir,Spider& spider
	,DependencyGraph& graph) const
	{
	auto target=new TargetDirectory(name_src,in_dir,graph.targetCounterGet());
	graph.targetRegister(std::unique_ptr<Target>(target));
	DirectoryLister dirlister(name_src);
	const char* entry=dirlister.read();
	while(entry!=nullptr)
		{
		if(m_ignore.find(Stringkey(entry))==m_ignore.end())
			{
			std::string path_tot(name_src);
			path_tot+='/';
			path_tot+=entry;
			auto entry_type=FileInfo(path_tot.c_str()).typeGet();
			if((entry_type==FileInfo::Type::DIRECTORY && m_recursive)
				|| entry_type==FileInfo::Type::FILE)
				{spider.scanFile(path_tot.c_str(),name_src);}
			}

		entry=dirlister.read();
		}
	}