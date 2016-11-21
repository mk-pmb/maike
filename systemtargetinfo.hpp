//@	{
//@	 "targets":[{"name":"systemtargetinfo.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"systemtargetinfo.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_SYSTEMTARGETINFO_HPP
#define MAIKE_SYSTEMTARGETINFO_HPP

#include "parametersetdumpable.hpp"
#include "visibility.hpp"
#include "mapreplace.hpp"
#include "variant.hpp"
#include "stringkey.hpp"
#include <string>
#include <map>
#include <vector>

namespace Maike
	{
	class PRIVATE SystemTargetInfo:public ParameterSetDumpable
		{
		public:
			SystemTargetInfo();
			~SystemTargetInfo() noexcept;

			void parameterGet(const Stringkey& key
				,ParameterProcessor&& proc) const;

			Variant variableGet(const Stringkey& key) const noexcept;

			void variableSet(const Stringkey& key,const char* str)
				{
				auto i=replace(m_strings,{key,std::string(str)});
				replace(m_sysvars,{key,Variant(i->second.c_str())});
				}

			template<class T>
			void variableSet(const Stringkey& key,const T& value)
				{replace(m_sysvars,{key,Variant(value)});}

			void clear();

			SystemTargetInfo& configAppend(const ResourceObject& targetinfo);
			void configDump(ResourceObject& targetinfo) const;
			ResourceObject configDump() const;

			SystemTargetInfo& sysvarsLoad();
			SystemTargetInfo& configAppendDefault();

		private:
			std::map<Stringkey,Variant> m_sysvars;
			std::map<Stringkey,std::string> m_strings;
			std::map<Stringkey,std::string> m_varnames;
		};

	}

#endif // MAIKE_SYSTEMINFO_HPP
