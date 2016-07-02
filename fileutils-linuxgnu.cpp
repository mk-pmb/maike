//@	[
//@		[
//@		"and(not(less_than(linux,version('2.6.33'))),gnu)"
//@			,{
//@		 	"targets":
//@				[{
//@			 	 "name":"fileutils.o","type":"object"
//@				,"dependencies":[{"ref":"pthread","rel":"external"}]
//@				}]
//@			}
//@		]
//@	]

#define _LARGEFILE64_SOURCE

#include "fileutils.hpp"
#include "errormessage.hpp"
#include "variant.hpp"
#include "exceptionhandler.hpp"
#include "strerror.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>

#include <vector>
#include <cstdint>
#include <exception>
#include <tuple>

using namespace Maike;

bool FileUtils::newer(const char* file_a,const char* file_b)
	{
	struct stat stat_a;
	struct stat stat_b;

	auto res_a=stat(file_a, &stat_a);
	auto errno_a=errno;
	auto res_b=stat(file_b, &stat_b);
	auto errno_b=errno;

	if(res_a==-1 && res_b==-1)
		{
		exceptionRaise(ErrorMessage("None of the files #0;, and #1; are accessible. #0;: #2;. #1;: #3;."
			,{
			 file_a
			,file_b
			,static_cast<const char*>(strerror(errno_a))
			,static_cast<const char*>(strerror(errno_b))
			}));
		}

	if(res_a==-1)
		{return 0;}

	if(res_b==-1)
		{return 1;}

	return stat_a.st_mtime > stat_b.st_mtime;
	}

void FileUtils::mkdir(const char* name)
	{
	if( ::mkdir(name, S_IRWXU )==-1 )
		{
		exceptionRaise(ErrorMessage("It was not possible to create a directory with name #0;. #1;"
			,{name,static_cast<const char*>(strerror(errno))}));
		}
	}


bool FileUtils::exists(const char* file)
	{
	if(access(file,F_OK)==-1)
		{return 0;}
	return 1;
	}


namespace
	{
	class FileDescriptor
		{
		public:
			FileDescriptor(const char* file,int flags,int mode)
				{
				m_fd=open(file,flags,mode);
				if(m_fd==-1)
					{
					exceptionRaise(ErrorMessage("It was not possible to open the file #0;. #1;"
						,{static_cast<const char*>(strerror(errno))}));
					}
				}

			FileDescriptor(const char* file,int flags)
				{
				m_fd=open(file,flags);
				if(m_fd==-1)
					{
					exceptionRaise(ErrorMessage("It was not possible to open the file #0;. #1;"
						,{static_cast<const char*>(strerror(errno))}));
					}
				}

			~FileDescriptor()
				{close(m_fd);}

			int get() noexcept
				{return m_fd;}

		private:
			int m_fd;
		};
	}

void FileUtils::copy(const char* source,const char* dest)
	{
	FileDescriptor source_fd(source,O_RDONLY);
	FileDescriptor dest_fd(dest,O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);

	struct stat source_stat;
	if(fstat(source_fd.get(),&source_stat)==-1)
		{
		unlink(dest);
		exceptionRaise(ErrorMessage("stat error: #0;",{static_cast<const char*>(strerror(errno))}));
		}

	auto size=source_stat.st_size;
	while(size!=0)
		{
		auto res=sendfile(dest_fd.get(),source_fd.get(),nullptr,size);
		if(res==-1)
			{
			unlink(dest);
			exceptionRaise(ErrorMessage("It was not possible to copy #0; to #1;"
				,{static_cast<const char*>(strerror(errno))}));
			}

		size-=res;
		}
	}
