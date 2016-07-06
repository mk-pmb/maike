//@	{
//@	 "targets":[{"name":"options.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"options.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_OPTIONS_HPP
#define MAIKE_OPTIONS_HPP

#include "twins.hpp"
#include "mapfixed.hpp"
#include "stringkey.hpp"
#include <string>
#include <vector>

namespace Maike
	{
	class Options
		{
		public:
			struct Option
				{
				const char* key;
				const char* description;
				unsigned int argcount;
				unsigned int group;
				const std::vector<std::string>* values;
				};

			typedef MapFixed<Stringkey::HashValue,Option
				,Stringkey("help")
				,Stringkey("version")
				,Stringkey("configfiles")
				,Stringkey("no-stdconfig")
				,Stringkey("no-sysvars")
				,Stringkey("targets")
				,Stringkey("list-leaf-targets")
				,Stringkey("list-external-targets")
				,Stringkey("list-all-targets")
				,Stringkey("dump-graph")
				,Stringkey("dump-target-usecount")
				,Stringkey("dump-timestat")> OptionMap;

			Options(Twins<const char* const*> args);

			void optionsPrint() const;

		private:
			OptionMap m_options;
			std::vector<std::string> m_data[OptionMap::size()];
		};
	}

#endif // MAIKE_OPTIONS_HPP
