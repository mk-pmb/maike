//@	{
//@	 "targets":[{"name":"targetcxxoptions.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"targetcxxoptions.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_TARGETCXXOPTIONS_HPP
#define MAIKE_TARGETCXXOPTIONS_HPP


#include "command.hpp"
#include <vector>
#include <string>

namespace Maike
	{
	class ResourceObject;

	class PRIVATE TargetCxxOptions
		{
		public:
			TargetCxxOptions();

			TargetCxxOptions(const ResourceObject& options);

			const Command& versionqueryGet() const noexcept
				{return m_versionquery;}

			const Command& objcompileGet() const noexcept
				{return m_objcompile;}

			const Command& appcompileGet() const noexcept
				{return m_appcompile;}

			const Command& dllcompileGet() const noexcept
				{return m_dllcompile;}

			const Command& pkgconfigGet() const noexcept
				{return m_pkgconfig;}


			unsigned long long int cxxversionMinGet() const noexcept
				{return m_cxxversion_min;}

			unsigned long long int cxxversionMaxGet() const noexcept
				{return m_cxxversion_max;}

			const char* stdprefixGet() const noexcept
				{return m_stdprefix.c_str();}

			const char* libextFormatGet() const noexcept
				{return m_libext_format.c_str();}

			const char* libintFormatGet() const noexcept
				{return m_libext_format.c_str();}

			const char* cflagsFormatGet() const noexcept
				{return m_cflags_format.c_str();}


			void configClear();
			TargetCxxOptions& configAppendDefault();
			TargetCxxOptions& configAppend(const ResourceObject& cxxoptions);
			TargetCxxOptions& configAppend(const TargetCxxOptions& options);

			void configDump(ResourceObject& cxxoptions) const;

			Twins<const std::string*> includedirGet() const noexcept
				{
				return
					{
					 m_includedir.data()
					,m_includedir.data() + m_includedir.size()
					};
				}

			Twins<const std::string*> includedirNoscanGet() const noexcept
				{
				return
					{
					 m_includedir_noscan.data()
					,m_includedir_noscan.data() + m_includedir_noscan.size()
					};
				}

			TargetCxxOptions& includedirNoscanAppend(const char* dir);


			const char* includedirFormatGet() const noexcept
				{return m_includedir_format.c_str();}

			Twins<const std::string*> libdirGet() const noexcept
				{
				return
					{
					 m_libdir.data()
					,m_libdir.data() + m_libdir.size()
					};
				}

			const char* libdirFormatGet() const noexcept
				{return m_libdir_format.c_str();}

			Twins<const std::string*> cflagsExtraGet() const noexcept
				{
				return
					{
					 m_cflags_extra.data()
					,m_cflags_extra.data() + m_cflags_extra.size()
					};
				}

		private:
			std::vector< std::string > m_includedir;
			std::vector< std::string > m_includedir_noscan;
			std::vector< std::string > m_libdir;

			std::string m_platform_suffix;

			std::string m_includedir_format;
			std::string m_libdir_format;
			std::string m_libext_format;
			std::string m_libint_format;

			std::string m_cflags_format;
			std::vector< std::string > m_cflags_extra;

			unsigned long long int m_cxxversion_min;
			unsigned long long int m_cxxversion_max;

			std::string m_stdprefix;

			Command m_appcompile;
			Command m_dllcompile;
			Command m_libcompile;
			Command m_objcompile;
			Command m_versionquery;
			Command m_pkgconfig;
		};

	inline TargetCxxOptions operator|(TargetCxxOptions a,const TargetCxxOptions& b)
		{return a.configAppend(b);}
	}

#endif
