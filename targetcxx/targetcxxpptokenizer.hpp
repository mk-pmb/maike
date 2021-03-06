//@	{
//@	 "targets":[{"name":"targetcxxpptokenizer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"targetcxxpptokenizer.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_TARGETCXXPPTOKENIZER_H
#define MAIKE_TARGETCXXPPTOKENIZER_H

#include "../readbuffer.hpp"
#include "../visibility.hpp"
#include <string>
#include <vector>

namespace Maike
	{
	class PRIVATE TargetCxxPPTokenizer
		{
		public:
			struct PRIVATE Token
				{
				Token():type(Type::OTHER){}
				std::string value;

				enum class Type:int{OTHER,DIRECTIVE,STRING,SYSINCLUDE};

				Type type;
				};

			TargetCxxPPTokenizer(DataSource& source):
				m_reader(source),m_state(State::NEWLINE)
				{}

			bool read(Token& token);

			std::vector<std::string> macroDecode(const char* str);

		private:
			ReadBuffer m_reader;
			enum class State:int
				{
				 NEWLINE
				,COMMENT_0
				,COMMENT_1
				,COMMENT_2
				,COMMENT_3
				,PREPROCESSOR_BEGIN
				,PREPROCESSOR
				,PREPROCESSOR_NEXT
				,STRING
				,SYSINCLUDE
				,JUNK
				};
			State m_state;
		};
	}

#endif
