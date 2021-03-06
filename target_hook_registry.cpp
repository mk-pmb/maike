//@	{"targets":[{"name":"target_hook_registry.o","type":"object"}]}

#include "target_hook_registry.hpp"
#include "resourceobject.hpp"
#include "target_hook_plugin.hpp"
#include "stringkey.hpp"
#include "errormessage.hpp"
#include "variant.hpp"
#include "exceptionhandler.hpp"

using namespace Maike;

std::map<Stringkey,Target_Hook_Plugin> Target_Hook_Registry::s_plugins;

Target_Hook_Plugin& Target_Hook_Registry::pluginLoad(const char* name)
	{
	auto key=Stringkey(name);
	auto i=s_plugins.find(key);
	if(i==s_plugins.end())
		{
		return s_plugins.emplace(std::move(key),Target_Hook_Plugin(name)).first
			->second;
		}
	return i->second;
	}

Target_Hook_Registry::Target_Hook_Registry(const ParameterSetDumpable& sysvars):
	r_sysvars(sysvars)
	{}

Target_Hook_Registry::~Target_Hook_Registry()
	{}

Target_Hook_Registry::HookInfo& Target_Hook_Registry::hookCreate(const char* name,const char* plugname)
	{
	auto key=Stringkey(name);
	auto i=m_hooks.find(key);
	if(i==m_hooks.end())
		{
		auto& plug=pluginLoad(plugname);
		HookInfo hi{std::string(name),std::string(plugname),plug.create(r_sysvars)};
		return m_hooks.emplace(std::move(key),std::move(hi)).first->second;
		}
	if(i->second.plugin!=plugname)
		{
		exceptionRaise(ErrorMessage("Incompatible configuration for hook #0;. "
			"Hook is configured for the plugin #1; but, new configuration wants #2;."
				,{name,i->second.plugin.c_str(),plugname}));
		}
	return i->second;
	}

Target_Hook_Registry& Target_Hook_Registry::hookRegister(const char* name_plugin
	,Twins<const char* const*> filename_exts)
	{
	auto hook_name=std::string(name_plugin);
	hook_name+="_default";
	size_t counter=0;
	while(m_hooks.find(Stringkey(hook_name.c_str()))!=m_hooks.end())
		{
		hook_name=std::string(name_plugin);
		hook_name+='_'+std::to_string(counter);
		++counter;
		}
	auto& hook_info=hookCreate(hook_name.c_str(),name_plugin);
	while(filename_exts.first!=filename_exts.second)
		{
		auto fext=*filename_exts.first;
		r_filenameext_hook[Stringkey(fext)]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(fext));
		++filename_exts.first;
		}
	hook_info.hook->configAppendDefault();
	return *this;
	}

Target_Hook_Registry& Target_Hook_Registry::hookConfigAppend(const char* name,const ResourceObject& config)
	{
	auto i=m_hooks.find(Stringkey(name));
	if(i==m_hooks.end())
		{
		exceptionRaise(ErrorMessage("No hook with name #0; has been loaded"
			,{name}));
		}
	i->second.hook->configAppend(config);
	return *this;
	}


Target_Hook_Registry& Target_Hook_Registry::configAppend(const ResourceObject& targethooks)
	{
	auto N=targethooks.objectCountGet();
	for(decltype(N) k=0;k<N;++k)
		{
		auto target_hook=targethooks.objectGet(k);
		auto& hook_info=hookCreate(
			 static_cast<const char*>(target_hook.objectGet("name"))
			,static_cast<const char*>(target_hook.objectGet("plugin")));

		auto filename_exts=target_hook.objectGet("filename_exts");
		auto M=filename_exts.objectCountGet();
		for(decltype(M) l=0;l<M;++l)
			{
			auto fext=static_cast<const char*>(filename_exts.objectGet(l));
			r_filenameext_hook[Stringkey(fext)]=hook_info.hook.get();
			hook_info.filename_exts.insert(std::string(fext));
			}

		if(target_hook.objectExists("config"))
			{hook_info.hook->configAppend(target_hook.objectGet("config"));}
		else
			{hook_info.hook->configAppendDefault();}
		}

	return *this;
	}

Target_Hook_Registry& Target_Hook_Registry::configAppendDefault()
	{
		{
		auto& hook_info=hookCreate("targetcxx_default","targetcxx");
		r_filenameext_hook[Stringkey(".cpp")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".cpp"));
		r_filenameext_hook[Stringkey(".cxx")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".cxx"));
		r_filenameext_hook[Stringkey(".c++")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".c++"));
		r_filenameext_hook[Stringkey(".cc")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".cc"));
		r_filenameext_hook[Stringkey(".hpp")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".hpp"));
		r_filenameext_hook[Stringkey(".hxx")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".hxx"));
		r_filenameext_hook[Stringkey(".h++")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".h++"));
		r_filenameext_hook[Stringkey(".hh")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".hh"));
		r_filenameext_hook[Stringkey(".h")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".h"));
		hook_info.hook->configAppendDefault();
		}

		{
		auto& hook_info=hookCreate("targetpython_default","targetpython");
		r_filenameext_hook[Stringkey(".py")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".py"));
		hook_info.hook->configAppendDefault();
		}

		{
		auto& hook_info=hookCreate("targetbash_default","targetbash");
		r_filenameext_hook[Stringkey(".sh")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".sh"));
		hook_info.hook->configAppendDefault();
		}

		{
		auto& hook_info=hookCreate("targetoctave_default","targetoctave");
		r_filenameext_hook[Stringkey(".m")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".m"));
		hook_info.hook->configAppendDefault();
		}

		{
		auto& hook_info=hookCreate("targetarchive_default","targetarchive");
		r_filenameext_hook[Stringkey(".archive")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".archive"));
		hook_info.hook->configAppendDefault();
		}

		{
		auto& hook_info=hookCreate("targetxml_default","targetxml");
		r_filenameext_hook[Stringkey(".xml")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".xml"));
		hook_info.hook->configAppendDefault();
		}

		{
		auto& hook_info=hookCreate("targetcp_default","targetcp");
		r_filenameext_hook[Stringkey(".cp")]=hook_info.hook.get();
		hook_info.filename_exts.insert(std::string(".cp"));
		hook_info.hook->configAppendDefault();
		}
	return *this;
	}

void Target_Hook_Registry::configClear()
	{
	r_filenameext_hook.clear();
	m_hooks.clear();
	}

void Target_Hook_Registry::enumerate(EnumCallbackFilenameExt&& callback) const
	{
	auto i=r_filenameext_hook.begin();
	auto i_end=r_filenameext_hook.end();
	while(i!=i_end)
		{
		callback(i->first,*(i->second));
		++i;
		}
	}

void Target_Hook_Registry::configDump(ResourceObject& targethooks) const
	{
	auto i=m_hooks.begin();
	auto i_end=m_hooks.end();
	while(i!=i_end)
		{
		auto hook=targethooks.createObject();

		hook.objectSet("name",hook.create(i->second.name.c_str()))
			.objectSet("plugin",hook.create(i->second.plugin.c_str()));

			{
			auto filename_exts=hook.createArray();
			auto j=i->second.filename_exts.begin();
			auto j_end=i->second.filename_exts.end();
			while(j!=j_end)
				{
				filename_exts.objectAppend(filename_exts.create(j->c_str()));
				++j;
				}
			hook.objectSet("filename_exts",std::move(filename_exts));
			}

			{
			auto config=hook.createObject();
			i->second.hook->configDump(config);
			hook.objectSet("config",std::move(config));
			}

		targethooks.objectAppend(std::move(hook));
		++i;
		}
	}
