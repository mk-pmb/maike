//@	{"targets":[{"name":"targetarchive.o","type":"object"}]}

#include "targetarchive.hpp"
#include "targetarchivecompiler.hpp"
#include "../resourceobject.hpp"
#include "../stringkey.hpp"
#include "../errormessage.hpp"
#include "../variant.hpp"
#include "../exceptionhandler.hpp"
#include "../fileutils.hpp"
#include <cstring>

using namespace Maike;

static TargetArchive::Type type(const char* name_src,const char* type)
	{
	auto key=Stringkey(type);
	if(key==Stringkey("tar"))
		{return TargetArchive::Type::TAR;}

	if(key==Stringkey("zip"))
		{return TargetArchive::Type::ZIP;}

	exceptionRaise(ErrorMessage("#0;: Unknown target type #1;.",{name_src,type}));
	}

static const char* type(TargetArchive::Type type)
	{
	switch(type)
		{
		case TargetArchive::Type::TAR:
			return "tar";
		case TargetArchive::Type::ZIP:
			return "zip";
		}
	return nullptr;
	}

TargetArchive::TargetArchive(const ResourceObject& obj
	,const TargetArchiveCompiler& compiler,const char* name_src
	,const char* in_dir,const char* root,size_t id,size_t line_count):
	TargetBase(obj,name_src,in_dir,root,id,line_count)
	,r_compiler(compiler)
	{
	m_type=type(name_src,static_cast<const char*>( obj.objectGet("type") ));
	if(obj.objectExists("compressor"))
		{m_compression=std::string(static_cast<const char*>( obj.objectGet("compressor") ));}
	if(obj.objectExists("root"))
		{m_root=std::string(static_cast<const char*>(obj.objectGet("root")));}

	if(obj.objectExists("contents"))
		{	
		auto contents=obj.objectGet("contents");
		auto N=contents.objectCountGet();
		for(decltype(N) n=0;n<N;++n)
			{
			auto obj=contents.objectGet(n);
			auto file=static_cast<const char*>(obj.objectGet("file"));
			auto filename_full=dircat(in_dir,file);
			dependencyAdd(Dependency(filename_full.c_str(),root
				,Dependency::Relation::MISC));
			}
		}
	}

void TargetArchive::dumpDetails(ResourceObject& target) const
	{
	target.objectSet("type",target.create(type(typeGet())))
		.objectSet("compressor",target.create(compressionGet()))
		.objectSet("root",target.create(rootGet()));
	}


bool TargetArchive::upToDate(Twins<const Dependency*>
	,Twins<const Dependency*>
	,const char* target_dir) const
	{
	auto name_full=dircat( target_dir,nameGet() );
	if(FileUtils::newer(sourceNameGet(),name_full.c_str()))
		{return 0;}


	auto up_to_date=[&name_full](const char* name,Dependency::Relation)
		{return !FileUtils::newer(name,name_full.c_str());};

	return dependenciesProcess(target_dir,dependencies(),USE_ALL,up_to_date);


#if EMBED_RELATED_COMPLETED
	while(dependency_list.first!=dependency_list.second)
		{
		auto dep=dependency_list.first;
		switch(dep->relationGet())
			{
			case Dependency::Relation::GENERATED:
				if( FileUtils::newer(dircat(target_dir,dep->nameGet()).c_str()	
					,name_full.c_str()))
					{return 0;}
				break;
			case Dependency::Relation::FILE:
				if(FileUtils::newer(dep->nameGet(),name_full.c_str()))
					{return 0;}
				break;
			default:
				break;
			}
		++dependency_list.first;
		}
#endif
	return 1;
	}

static std::vector<std::string> filesCollect(Twins<const Dependency*> dependency_list
	,const char* target_dir)
	{
	std::vector<std::string> ret;
	auto collect=[&ret](const char* name,Dependency::Relation)
		{
		ret.push_back(name);
		return 1;
		};
	dependenciesProcess(target_dir,dependency_list,USE_ALL&(~USE_INTERNAL),collect);

	return std::move(ret);
	}

static std::vector<const char*> filesCollect(std::vector<std::string>&& files)=delete;

static std::vector<const char*> filesCollect(const std::vector<std::string>& files)
	{
	std::vector<const char*> ret;
	auto ptr=files.data();
	auto ptr_end=ptr + files.size();
	while(ptr!=ptr_end)
		{
		ret.push_back(ptr->c_str());
		++ptr;
		}
	return std::move(ret);
	}

void TargetArchive::compileImpl(Twins<const Dependency*>
	,Twins<const Dependency*>
	,const char* target_dir)
	{
	auto files_input=filesCollect(dependencies(),target_dir);
	auto r_files_input=filesCollect(files_input);
	auto range=Twins<const char* const*>(r_files_input.data(),r_files_input.data() + r_files_input.size()); 
	switch(typeGet())
		{
		case Type::TAR:
			r_compiler.tar(range,nameGet(),target_dir,rootGet(),compressionGet());
			break;
		case Type::ZIP:
			r_compiler.zip(range,nameGet(),target_dir,rootGet());
			break;
		}
	}

Maike::TargetArchive::~TargetArchive() noexcept
	{
	}


TargetArchive* TargetArchive::create(const ResourceObject& obj
	,const TargetArchiveCompiler& intpret,const char* name_src
	,const char* in_dir,const char* root,size_t id,size_t line_count)
	{return new TargetArchive(obj,intpret,name_src,in_dir,root,id,line_count);}

void TargetArchive::destroy() noexcept
	{delete this;}
