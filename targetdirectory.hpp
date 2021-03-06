//@	{
//@	 "targets":[{"name":"targetdirectory.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"targetdirectory.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_TARGETDIRECTORY_HPP
#define MAIKE_TARGETDIRECTORY_HPP

#include "targetbase.hpp"

namespace Maike
	{
	class TargetDirectoryCompiler;

	class PRIVATE TargetDirectory final:public TargetBase
		{
		public:
			static TargetDirectory* create(const char* name,const char* in_dir
				,const char* root,size_t id);

			void compileImpl(Twins<const Dependency*> dependency_list
				,Twins<const Dependency*> dependency_list_full
				,const char* target_dir);

			bool upToDate(Twins<const Dependency*> dependency_list
				,Twins<const Dependency*> dependency_list_full
				,const char* target_dir) const;

			void destroy() noexcept;

			bool generated() const noexcept
				{return 1;}

			void dumpDetails(ResourceObject&) const
				{}

		private:
			explicit TargetDirectory(const char* name,const char* in_dir
				,const char* root,size_t id):TargetBase(name,name,in_dir,root,id)
				{}
		};
	}

#endif
