//@	{"targets":[{"name":"targetpythonhook.o","type":"object"}]}

#include "targetpythonhook.hpp"

using namespace Maike;

TargetPythonHook::TargetPythonHook(const ParameterSet& params):
	m_intpret(params),m_factory(m_intpret)
	{
	}

TargetPythonHook* TargetPythonHook::create(const ParameterSet& params)
	{return new TargetPythonHook(params);}

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

void TargetPythonHook::configDump(ResourceObject& pythonoptions) const
	{
	m_intpret.configDump(pythonoptions);
	}