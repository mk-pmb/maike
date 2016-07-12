//@	{"targets":[{"name":"targetpythonhook.o","type":"object"}]}

#include "targetpythonhook.hpp"

using namespace Maike;

TargetPythonHook::TargetPythonHook(const ResourceObject& pythonoptions
	,const ParameterSet& params):
	m_intpret(pythonoptions,params),m_factory(m_intpret)
	{
	}

TargetPythonHook* TargetPythonHook::create(const ResourceObject& pythonoptions
	,const ParameterSet& params)
	{return new TargetPythonHook(pythonoptions,params);}

void TargetPythonHook::destroy() noexcept
	{delete this;}

void TargetPythonHook::configClear()
	{
	m_intpret.configClear();
	}


TargetPythonHook& TargetPythonHook::configAppend(const ResourceObject& pythonoptions)
	{
	m_intpret.configAppend(pythonoptions);
	return *this;
	}
