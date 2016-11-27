//@	{
//@	 "targets":[ {"name":"fileutilsdefault.hpp","type":"include"} ]
//@	,"dependencies_extra":[{"ref":"fileutilsdefault.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_FILEUTILSDEFAULT_HPP
#define MAIKE_FILEUTILSDEFAULT_HPP

#include "fileutils.hpp"
#include "stringkey.hpp"
#include "writebuffer.hpp"
#include <map>

namespace Maike
	{
	class PRIVATE FileUtilsDefault:public FileUtils
		{
		public:
			explicit FileUtilsDefault(DataSink& sink):m_wb(sink){}

			bool checksumCheck(const char* filename) const;
			bool exists(const char* file) const;
			void mkdir(const char* name);
			void copy(const char* source,const char* dest);
			void copyFilter(const char* source,const char* dest
				,const char* comment_line_regexp);
			void remove(const char* name);
			void removeTree(const char* name,Twins<const char* const*> keeplist);
			void echo(const char* str,const char* filename);
			void checksumCompute(const char* filename);
			void hashesLoad(const char* hashfile);

		private:
			std::map< Stringkey,std::array<uint8_t,32> > m_hashes;
			WriteBuffer m_wb;
		};
	}

#endif

