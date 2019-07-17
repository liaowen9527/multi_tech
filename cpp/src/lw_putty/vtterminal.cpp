#include "vtterminal.h"
#include <mutex>
#include <map>

static std::mutex s_mutex;
static std::map<void*, CVtTerminal*> s_mapTerminal;

CVtTerminal* CVtTerminal::Find(void* terminal)
{
	std::lock_guard<std::mutex> lck(s_mutex);
	auto itr = s_mapTerminal.find(terminal);
	if (s_mapTerminal.end() != itr)
	{
		return itr->second;
	}
	return nullptr;
}

void CVtTerminal::Register(CVtTerminal* terminal)
{
	std::lock_guard<std::mutex> lck(s_mutex);
	s_mapTerminal[terminal->m_term] = terminal;
}

void CVtTerminal::UnRegister(CVtTerminal* terminal)
{
	std::lock_guard<std::mutex> lck(s_mutex);
	s_mapTerminal.erase(terminal->m_term);
}

CVtCallback* CVtTerminal::GetCallback()
{
	return m_pCallback;
}

void CVtTerminal::SetCallback(CVtCallback* pCallback)
{
	m_pCallback = pCallback;
}