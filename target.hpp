//@ {
//@ "targets":[ {"name":"target.hpp","type":"include"} ]
//@ }

#ifndef MAIKE_TARGET_HPP
#define MAIKE_TARGET_HPP

#include "twins.hpp"
#include <cstddef>

namespace Maike
	{
	class Dependency;
	class Invoker;

	class Target
		{
		public:
			virtual ~Target()=default;
			virtual Target& childCountIncrement() noexcept=0;
			virtual size_t childCountGet() const noexcept=0;
			virtual bool compile(Twins<const Target* const*> targets_rel
				,Invoker& invoker) const noexcept=0;
			virtual void dependencyAdd(Dependency&& dep)=0;
			virtual Twins<const Dependency*> dependenciesGet() const noexcept=0;
			virtual size_t idGet() const noexcept=0;
			virtual const char* nameGet() const noexcept=0;
			virtual const char* nameFullGet() const noexcept=0;
			virtual const char* sourceNameGet() const noexcept=0;
			virtual const char* sourceNameFullGet() const noexcept=0;
			virtual bool upToDate(Twins<const Target* const*> targets_rel
				,Invoker& invoker) const noexcept=0;
		};
	}

#endif
