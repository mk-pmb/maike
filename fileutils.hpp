//@	{
//@	 "targets":[ {"name":"fileutils.hpp","type":"include"} ]
//@	,"dependencies_extra":[{"ref":"fileutils.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_FILEUTILS_HPP
#define MAIKE_FILEUTILS_HPP

#include "visibility.hpp"

namespace Maike
	{
	class DataSink;

	namespace FileUtils
		{
		PRIVATE bool newer(const char* file_a,const char* file_b);
		PRIVATE void mkdir(const char* name);
		PRIVATE void copy(const char* source,const char* dest);
		PRIVATE void copyFilter(const char* source,const char* dest
			,const char* comment_line_regexp);
		PRIVATE bool exists(const char* file);
		};
	}

#endif
