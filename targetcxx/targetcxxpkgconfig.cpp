//@	{"targets":[{"name":"targetcxxpkgconfig.o","type":"object"}]}

#include "targetcxxpkgconfig.hpp"
#include "targetcxxoptions.hpp"
#include "../target.hpp"
#include "../stdstream.hpp"
#include "../writebuffer.hpp"
#include "../parametersetmapfixed.hpp"
#include "../readbuffer.hpp"
#include "../thread.hpp"
#include "../errormessage.hpp"
#include "../variant.hpp"
#include "../exceptionhandler.hpp"
#include "../dependency.hpp"

using namespace Maike;

namespace
	{
	class ReadCallback
		{
		public:
			explicit ReadCallback(DataSource* src):r_src(src)
				{}

			void operator()()
				{
				WriteBuffer wb(StdStream::error());
				try
					{
					ReadBuffer rb(*r_src);
					while(!rb.eof())
						{
						wb.write(rb.byteRead());
						}
					}
				catch(const ErrorMessage& message)
					{wb.write("Error: ").write(message.messageGet()).write("\n");}
				}

		private:
			DataSource* r_src;
		};
	}

namespace
	{
	class PkgConfigTokenizer
		{
		public:
			PkgConfigTokenizer(DataSource& src,const char* delim_seq):m_rb(src)
				,r_delim_seq(delim_seq),m_ch_next('\0')
				{}

			bool itemRead(std::string& item);

		private:
			ReadBuffer m_rb;
			const char* r_delim_seq;
			char m_ch_next;
		};
	}

bool PkgConfigTokenizer::itemRead(std::string& item)
	{
	auto ptr=r_delim_seq + ((m_ch_next=='\0')? 1 : 0);
	std::string reminder;
	item.clear();
	if(m_ch_next!='\0')
		{
		item+=m_ch_next;
		m_ch_next='\0';
		}
	while(!m_rb.eof())
		{
		auto ch_in=m_rb.byteRead();
		if(ch_in=='\n')
			{return 1;}
		if(ch_in==*ptr && ch_in!='\0')
			{
			reminder+=ch_in;
			++ptr;
			}
		else
			{
			if(*ptr=='\0')
				{
				m_ch_next=ch_in;
				return 1;
				}
			ptr=r_delim_seq;
			item+=reminder;
			item+=ch_in;
			}
		}
	return 0;
	}

template<class ItemSink>
PRIVATE void outputRead(Pipe& pipe,const char* delim_seq,const ItemSink& sink)
	{
	auto standard_error=pipe.stderrCapture();
	Thread<ReadCallback> stderr_reader(ReadCallback{standard_error.get()});
	auto standard_output=pipe.stdoutCapture();
	PkgConfigTokenizer tokens(*standard_output.get(),delim_seq);
	std::string str;
	while(tokens.itemRead(str))
		{
		if(str.size()!=0)
			{sink(str.c_str());}
		}
	}

typedef ParameterSetMapFixed<
	 Stringkey("action")
	,Stringkey("libname")> PkgConfigParams;

template<class ItemSink>
PRIVATE int pipeExecute(const Command& cmd,const PkgConfigParams& params
	,const char* delim_seq,const ItemSink& sink)
	{
	const ParameterSet* paramsets[]={&params};
	auto pkgconfig=cmd.execute(Pipe::REDIRECT_STDERR|Pipe::REDIRECT_STDOUT|Pipe::ECHO_OFF
		,{paramsets,paramsets + 1});
	outputRead(pkgconfig,delim_seq,sink);
	return pkgconfig.exitStatusGet();
	}

PkgConfigRequest::PkgConfigRequest(const Command& cmd,const char* libname
	,const char* context)
	{
	PkgConfigParams params;
	params.get<Stringkey("libname")>().push_back(std::string(libname));

	params.get<Stringkey("action")>().push_back(std::string("--cflags-only-I"));
	if(pipeExecute(cmd,params," -I",[this](const char* str)
		{m_incdir.push_back(std::string(str));})!=0)
		{
		exceptionRaise(ErrorMessage("#0;: It was not possible to find information about "
			"the library #1;",{context,libname}));
		}

	params.get<Stringkey("action")>()[0]=std::string("--cflags-only-other");
	if(pipeExecute(cmd,params," -",[this](const char* str)
		{m_cflags.push_back(std::string(str));})!=0)
		{
		exceptionRaise(ErrorMessage("#0;: It was not possible to find information about "
			"the library #1;",{context,libname}));
		}

	params.get<Stringkey("action")>()[0]=std::string("--libs-only-L");
	if(pipeExecute(cmd,params," -L",[this](const char* str)
		{m_libdir.push_back(std::string(str));})!=0)
		{
		exceptionRaise(ErrorMessage("#0;: It was not possible to find information about "
			"the library #1;",{context,libname}));
		}

	params.get<Stringkey("action")>()[0]=std::string("--libs-only-l");
	std::vector<Dependency> depnames;
	if(pipeExecute(cmd,params," -l",[this](const char* str)
		{m_deps.push_back(Dependency(str,"",Dependency::Relation::EXTERNAL));})!=0)
		{
		exceptionRaise(ErrorMessage("#0;: It was not possible to find information about "
			"the library #1;",{context,libname}));
		}
	std::reverse(m_deps.data(),m_deps.data() + m_deps.size());
	}


const PkgConfigRequest& PkgConfigRequest::optionsPush(TargetCxxOptions& options
	,TargetCxxOptions& options_local) const
	{
	std::for_each(m_incdir.begin(),m_incdir.end(),[&options_local](const std::string& dir)
		{options_local.includedirNoscanAppend(dir.c_str());});
	std::for_each(m_cflags.begin(),m_cflags.end(),[&options](const std::string& flag)
		{options.cflagsExtraAppend(flag.c_str());});
	std::for_each(m_libdir.begin(),m_libdir.end(),[&options](const std::string& dir)
		{options.libdirAppend(dir.c_str());});
	return *this;
	}

const PkgConfigRequest& PkgConfigRequest::dependenciesPush(Target& t) const
	{
	std::for_each(m_deps.begin(),m_deps.end(),[&t](const Dependency& dep)
		{
		Dependency dep_copy(dep);
		t.dependencyAdd(std::move(dep_copy));
		});
	return *this;
	}

