//@	{"targets":[{"name":"targetcxxloader.o","type":"object"}]}

#include "targetcxxloader.hpp"
#include "filein.hpp"
#include "targetcxxpptokenizer.hpp"
#include "spider.hpp"
#include "resourceobject.hpp"
#include "targetcxx.hpp"
#include "dependencygraph.hpp"
#include <cstdio>

using namespace Maike;

namespace
	{
	class TagFilter:public DataSource
		{
		public:
			TagFilter(DataSource& source):m_reader(source)
				,m_state(State::NEWLINE)
				{}

			TagFilter(DataSource&& source):TagFilter(source)
				{}

			size_t read(void* buffer,size_t length);

			const char* nameGet() const noexcept
				{return m_reader.nameGet();}

		private:
			ReadBuffer m_reader;
			enum class State:int{NEWLINE,COMMENT_0,JUNK,COMMENT_1,DATA};
			State m_state;
		};
	}

size_t TagFilter::read(void* buffer,size_t length)
	{
	auto buffer_out=reinterpret_cast<uint8_t*>(buffer);
	size_t n_read=0;
	auto state=m_state;
	while(n_read!=length && !m_reader.eof())
		{
		auto ch_in=m_reader.byteRead();
		switch(state)
			{
			case State::NEWLINE:
				if(!(ch_in<=' '))
					{
					switch(ch_in)
						{
						case '/':
							state=State::COMMENT_0;
							break;
						default:
							state=State::JUNK;
						}
					}
				break;

			case State::COMMENT_0:
				switch(ch_in)
					{
					case '/':
						state=State::COMMENT_1;
						break;
					default:
						state=State::JUNK;
					}
				break;

			case State::COMMENT_1:
				switch(ch_in)
					{
					case '@':
						state=State::DATA;
						break;
					default:
						state=State::JUNK;
					}
				break;

			case State::DATA:
				switch(ch_in)
					{
					case '\n':
					case '\r':
						*buffer_out='\n';
						++buffer_out;
						++n_read;
						state=State::NEWLINE;
						break;
					default:
						*buffer_out=ch_in;
						++buffer_out;
						++n_read;
					}
				break;

			case State::JUNK:
				switch(ch_in)
					{
					case '\n':
					case '\r':
						state=State::NEWLINE;
						break;
					}
				break;
			}
		}
	m_state=state;
	return n_read;
	}



static void includesGet(const char* name_src,const char* in_dir
	,Spider& spider,DependencyGraph& graph,TargetCxx& target)
	{
	std::string name_full(in_dir);
	name_full+='/';
	name_full+=name_src;

	FileIn file_reader(name_src);
	TargetCxxPPTokenizer cpptok(file_reader);
	TargetCxxPPTokenizer::Token tok_in;
	enum class Mode:uint8_t{NORMAL,INCLUDE};
	auto mode=Mode::NORMAL;
	while(cpptok.read(tok_in))
		{
		switch(mode)
			{
			case Mode::NORMAL:
				switch(tok_in.type)
					{
					case TargetCxxPPTokenizer::Token::Type::DIRECTIVE:
						if(tok_in.value=="include")
							{mode=Mode::INCLUDE;}
						break;
					default:
						break;
					}
				break;
			case Mode::INCLUDE:
				switch(tok_in.type)
					{
					case TargetCxxPPTokenizer::Token::Type::SYSINCLUDE:
					//TODO Implement this
						break;
					case TargetCxxPPTokenizer::Token::Type::STRING:
						{
						std::string name_dep_full(in_dir);
						name_dep_full+='/';
						name_dep_full+=tok_in.value;
						target.dependencyAdd(Dependency(name_dep_full.c_str()
							,Dependency::Relation::INTERNAL));
						spider.scanFile(name_dep_full.c_str(),in_dir);
						FileIn file(name_dep_full.c_str());
						ResourceObject obj{TagFilter(file)};
						if(obj.objectExists("dependencies_extra"))
							{
							auto deps=obj.objectGet("dependencies_extra");
							auto N=deps.objectCountGet();
							for(decltype(N) k=0;k<N;++k)
								{
								auto dep=deps.objectGet(k);
								auto ref=static_cast<const char*>( dep.objectGet("ref") );
								std::string ref_full(in_dir);
								ref_full+='/';
								ref_full+=ref;
								if(ref_full!=target.nameGet())
									{target.dependencyAdd(Dependency(deps.objectGet(k),in_dir));}
								}
							}
						}
						break;
					default:
					//CHECK Is this a legal case?
						break;
					}
				mode=Mode::NORMAL;
				break;
			}
		}
	}

void TargetCxxLoader::targetsLoad(const char* name_src,const char* in_dir
	,Spider& spider,DependencyGraph& graph,const ExpressionEvaluator& evaluator) const
	{
	std::string name_full(in_dir);
	name_full+='/';
	name_full+=name_src;

	FileIn source(name_src);
	ResourceObject rc{TagFilter(source)};

	if(rc.objectExists("targets"))
		{
		auto targets=rc.objectGet("targets");
		auto N=targets.objectCountGet();
		for(decltype(N) k=0;k<N;++k)
			{
			std::unique_ptr<TargetCxx> target
				{
				new TargetCxx(targets.objectGet(k),name_src,in_dir,graph.targetCounterGet())
				};
			includesGet(name_src,in_dir,spider,graph,*target);
			graph.targetRegister(std::move(target));
			}
		}
/*	else
		{
		printf("%s: No unconditional targets\n",name_full.c_str());
		}*/
	}
