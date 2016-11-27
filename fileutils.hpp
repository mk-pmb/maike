//@	{
//@	 "targets":[ {"name":"fileutils.hpp","type":"include"} ]
//@	}

#ifndef MAIKE_FILEUTILS_HPP
#define MAIKE_FILEUTILS_HPP

#include "visibility.hpp"
#include "twins.hpp"

namespace Maike
	{
	class PRIVATE FileUtils
		{
		public:
			virtual bool checksumCheck(const char* filename) const=0;
			virtual bool exists(const char* file) const=0;
			virtual void remove(const char* name)=0;
			virtual void mkdir(const char* name)=0;
			virtual void copy(const char* source,const char* dest)=0;
			virtual void copyFilter(const char* source,const char* dest
				,const char* comment_line_regexp)=0;
			virtual void removeTree(const char* name,Twins<const char* const*> keeplist)=0;
			virtual void echo(const char* str,const char* filename)=0;
		};
	}

#endif

