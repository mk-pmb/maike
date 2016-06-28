//@	{
//@	 "targets":[ {"name":"invokerreal.hpp","type":"include"} ]
//@	,"dependencies_extra":[{"ref":"invokerreal.o","rel":"implementation"}]
//@	}

#ifndef MAIKE_INVOKERREAL_HPP
#define MAIKE_INVOKERREAL_HPP

#include "invoker.hpp"

namespace Maike
	{
	class DataSink;

	class InvokerReal:public Invoker
		{
		public:
			InvokerReal();

			InvokerReal& echoStreamSet(DataSink& sink) noexcept
				{
				r_echo_stream=&sink;
				return *this;
				}

			int run(const char* command,Twins<const char* const*> args
				,DataSink& standard_output,DataSink& standard_error);
			bool newer(const char* file_a,const char* file_b) const;
			void mkdir(const char* name);
			void copy(const char* source,const char* dest);
			bool exists(const char* file) const;

		private:
			DataSink* r_echo_stream;
		};
	}

#endif

