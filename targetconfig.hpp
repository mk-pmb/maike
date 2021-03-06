//@	{
//@	 "targets":[{"name":"targetconfig.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"targetconfig.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_TARGETCONFIG_HPP
#define MAIKE_TARGETCONFIG_HPP

#include "target.hpp"
#include "dependency.hpp"
#include <string>
#include <vector>
#include <cmath>

namespace Maike
	{
	class PRIVATE TargetConfig final:public Target
		{
		public:
			static TargetConfig* create(const ResourceObject& config,size_t id)
				{return new TargetConfig(config,id);}

			void compile(Twins<const Dependency*> dependency_list
				,Twins<const Dependency*> dependency_list_full
				,const char* target_dir);

			Target& dependencyAdd(Dependency&&)
				{return *this;}

			Twins<const Dependency*> dependencies() const noexcept
				{return {&m_dep,&m_dep + 1};}

			Twins<Dependency*> dependencies() noexcept
				{return {&m_dep,&m_dep + 1};}

			TargetConfig& dependencyInverseAdd(Dependency&& dep)
				{
				m_deps_inverse.push_back(std::move(dep));
				return *this;
				}

			Twins<const Dependency*> dependenciesInverseGet() const noexcept
				{
				return {m_deps_inverse.data(),m_deps_inverse.data() + m_deps_inverse.size()};
				}

			size_t idGet() const noexcept
				{return m_id;}

			const char* nameGet() const noexcept
				{return "maikeconfig.json";}

			const char* sourceNameGet() const noexcept
				{return "";}

			const char* inDirGet() const noexcept
				{return ".";}

			bool upToDate(Twins<const Dependency*> dependency_list
				,Twins<const Dependency*> dependency_list_full
				,const char* target_dir) const;

			void dump(ResourceObject&) const
				{}

			size_t lineCountGet() const noexcept
				{return 0;}

			double compilationTimeGet() const noexcept
				{return m_compilation_time;}

			const char* descriptionGet() const noexcept
				{return "File containing effective configuration";}

			bool generated() const noexcept
				{return 1;}

		private:
			explicit TargetConfig(const ResourceObject& config,size_t id);
			virtual ~TargetConfig()
				{}
			void destroy() noexcept
				{delete this;}
			size_t m_id;
			Dependency m_dep;
			std::vector<Dependency> m_deps_inverse;
			std::string m_content;
			mutable double m_compilation_time;

		};
	}

#endif
