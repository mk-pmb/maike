//@	{
//@	 "targets":[{"name":"targetpythonhook.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"targetpythonhook.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_TARGETPYTHONHOOK_HPP
#define MAIKE_TARGETPYTHONHOOK_HPP

#include "../target_hook.hpp"

#include "targetpythoninterpreter.hpp"
#include "targetpythonloader.hpp"

namespace Maike
	{
	class PRIVATE TargetPythonHook final:public Target_Hook
		{
		public:
			static TargetPythonHook* create(const ParameterSetDumpable& params);

			static TargetPythonHook* create(ParameterSet&& params)=delete;

			const TargetPythonLoader& loaderGet() const noexcept
				{return m_loader;}

			void configClear();

			TargetPythonHook& configAppendDefault();

			TargetPythonHook& configAppend(const ResourceObject& pythonoptions);

			void configDump(ResourceObject& pythonoptions) const;

		private:
			TargetPythonInterpreter m_intpret;
			TargetPythonLoader m_loader;

			explicit TargetPythonHook(const ParameterSetDumpable& params);

			void destroy() noexcept;
		};
	}

#endif
