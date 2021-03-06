//@	{
//@	"targets":[{"name":"targetdirectoryloader.o","type":"object"}]
//@	}

#include "targetdirectoryloader.hpp"
#include "directorylister.hpp"
#include "fileinfo.hpp"
#include "spider.hpp"
#include "targetdirectory.hpp"
#include "dependencygraph.hpp"
#include "resourceobject.hpp"
#include "handle.hpp"
#include "target_factorydelegator.hpp"
#include "pathutils.hpp"
#include <string>
#include <cstring>


using namespace Maike;

TargetDirectoryLoader::TargetDirectoryLoader()
	{
	configClear();
	}

void TargetDirectoryLoader::targetsLoad(const char* name_src,const char* in_dir
	,Spider& spider,DependencyGraph& graph,Target_FactoryDelegator& factory) const
	{
	Handle<TargetDirectory> target(
		TargetDirectory::create(name_src,in_dir,factory.rootGet(),factory.idGet()));
	graph.targetRegister(target);
	DirectoryLister dirlister(name_src);
	const char* entry=dirlister.read();
	while(entry!=nullptr)
		{
		if(m_ignore.find(Stringkey(entry))==m_ignore.end())
			{
			auto path_tot=dircat(name_src,entry);
			if(m_ignore_fullname.find(Stringkey(path_tot.c_str()))
				==m_ignore_fullname.end())
				{
				auto entry_type=FileInfo(path_tot.c_str()).typeGet();
				if((entry_type==FileInfo::Type::DIRECTORY && m_recursive)
					|| entry_type==FileInfo::Type::FILE)
					{spider.scanFile(path_tot.c_str(),name_src);}
				}
			}

		entry=dirlister.read();
		}
	}

TargetDirectoryLoader& TargetDirectoryLoader::pathReject(const char* name)
	{
	m_ignore[Stringkey(name)]=std::string(name);
	return *this;
	}

TargetDirectoryLoader& TargetDirectoryLoader::pathAccept(const char* name)
	{
	m_ignore.erase(Stringkey(name));
	return *this;
	}

TargetDirectoryLoader& TargetDirectoryLoader::pathFullReject(const char* name)
	{
	m_ignore_fullname[Stringkey(name)]=std::string(name);
	return *this;
	}

TargetDirectoryLoader& TargetDirectoryLoader::pathFullAccept(const char* name)
	{
	m_ignore_fullname.erase(Stringkey(name));
	return *this;
	}


void TargetDirectoryLoader::configClear()
	{
	m_ignore.clear();
	m_ignore_fullname.clear();
	m_recursive=0;
//	IRP (Infinite Recursion Prevention)
	pathReject("."); //Remove references to this
	pathReject("..");//and parent directory
	}

TargetDirectoryLoader& TargetDirectoryLoader::configAppendDefault()
	{
	m_recursive=0;
	pathReject(".");
	pathReject("..");
	return *this;
	}

TargetDirectoryLoader& TargetDirectoryLoader::configAppend(const ResourceObject& directoryoptions)
	{
	if(directoryoptions.objectExists("recursive"))
		{
		m_recursive=static_cast<long long int>(directoryoptions.objectGet("recursive") );
		}


	if(directoryoptions.objectExists("paths_reject"))
		{
		auto paths_reject=directoryoptions.objectGet("paths_reject");
		auto N=paths_reject.objectCountGet();
		for(decltype(N) k=0;k<N;++k)
			{
			auto path=static_cast<const char*>(paths_reject.objectGet(k));
			pathReject(path);
			}
		}

	if(directoryoptions.objectExists("paths_full_reject"))
		{
		auto paths_reject=directoryoptions.objectGet("paths_full_reject");
		auto N=paths_reject.objectCountGet();
		for(decltype(N) k=0;k<N;++k)
			{
			auto path=static_cast<const char*>(paths_reject.objectGet(k));
			pathFullReject(path);
			}
		}

	return *this;
	}


void TargetDirectoryLoader::configDump(ResourceObject& directoryoptions) const
	{
	directoryoptions.objectSet("recursive",directoryoptions.create(static_cast<long long int>(m_recursive)));
		{
		auto paths_reject=directoryoptions.createArray();
		auto i=m_ignore.begin();
		auto i_end=m_ignore.end();
		while(i!=i_end)
			{
			paths_reject.objectAppend(paths_reject.create(i->second.c_str()));
			++i;
			}
		directoryoptions.objectSet("paths_reject",std::move(paths_reject));
		}

		{
		auto paths_reject=directoryoptions.createArray();
		auto i=m_ignore_fullname.begin();
		auto i_end=m_ignore_fullname.end();
		while(i!=i_end)
			{
			paths_reject.objectAppend(paths_reject.create(i->second.c_str()));
			++i;
			}
		directoryoptions.objectSet("paths_full_reject",std::move(paths_reject));
		}
	}
