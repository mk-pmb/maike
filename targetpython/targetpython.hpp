//@	{
//@	 "targets":[{"name":"targetpython.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"targetpython.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_TARGETPYTHON_HPP
#define MAIKE_TARGETPYTHON_HPP

#include "../targetbase.hpp"

namespace Maike
	{
	class TargetPythonInterpreter;
	class PRIVATE TargetPython final : public Maike::TargetBase
		{
		public:
			static TargetPython* create(const ResourceObject& obj
				,const TargetPythonInterpreter& compiler,const char* name_src
				,const char* in_dir,const char* root,size_t id,size_t line_count);

			static TargetPython* create(const ResourceObject& obj
				,TargetPythonInterpreter&& compiler,const char* name_src
				,const char* in_dir,const char* root,size_t id
				,size_t line_count)=delete;

			bool upToDate(Twins<const Dependency*> dependency_list
				,Twins<const Dependency*> dependency_list_full
				,const char* target_dir) const;

			void compileImpl(Twins<const Dependency*> dependency_list
				,Twins<const Dependency*> dependency_list_full
				,const char* target_dir);

			void destroy() noexcept;

			void dumpDetails(ResourceObject& target) const;

			bool generated() const noexcept
				{return 1;}

		private:
			const TargetPythonInterpreter& r_intpret;

			TargetPython(const ResourceObject& obj
				,const TargetPythonInterpreter& intpret,const char* name_src
				,const char* in_dir,const char* root,size_t id,size_t line_count);

			TargetPython(const ResourceObject& obj
				,TargetPythonInterpreter&& intpret,const char* name_src
				,const char* in_dir,const char* root,size_t id
				,size_t line_count)=delete;

			~TargetPython() noexcept;

			std::vector<std::string> m_args;
			std::string m_root;
			int m_status;
			bool m_static;
		};
	}

#endif
