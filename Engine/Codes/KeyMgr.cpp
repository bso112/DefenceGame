#include "..\Headers\KeyMgr.h"


USING(Engine)

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));

}


bool CKeyMgr::Key_Pressing(int _Key, _uint _iSceneID)
{
	if ((GetAsyncKeyState(_Key) & 0x8000))
	{
		if (_iSceneID >= m_iMaxScene)
			MSG_BOX("유효하지 않은 씬입니다.");

		if (m_listObservers[_iSceneID].empty())
			return false;

		for (auto& observer : m_listObservers[_iSceneID])
		{
			observer->OnKeyPressing(_Key);
		}
		return true;
	}
	return false;

}

bool CKeyMgr::Key_Down(int _Key, _uint _iSceneID)
{
	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];

		if (_iSceneID >= m_iMaxScene)
			MSG_BOX("유효하지 않은 씬입니다.");

		if (m_listObservers[_iSceneID].empty())
			return false;

		for (auto& observer : m_listObservers[_iSceneID])
		{
			observer->OnKeyDown(_Key);
		}
		return true;
	}
	return false;
}

bool CKeyMgr::Key_Up(int _Key, _uint _iSceneID)
{
	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		
		if (_iSceneID >= m_iMaxScene)
			MSG_BOX("유효하지 않은 씬입니다.");

		if (m_listObservers[_iSceneID].empty())
			return false;

		for (auto& observer : m_listObservers[_iSceneID])
		{
			observer->OnKeyUp(_Key);
		}
		return true;
	}
	return false;
}

void CKeyMgr::Key_Update()
{
	// Key 상태 복원 함수
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];

		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}

HRESULT CKeyMgr::Reserve_Key_Manager(_uint _iMaxSceneID)
{
	m_listObservers = new list<CBase*>[_iMaxSceneID];

	m_iMaxScene = _iMaxSceneID;
	return S_OK;
}

HRESULT CKeyMgr::RegisterObserver(_uint _eSceneID, CBase * _pObserver)
{
	if (_eSceneID >= m_iMaxScene)
		return E_FAIL;

	if (nullptr == _pObserver)
		return E_FAIL;
	
	m_listObservers[_eSceneID].push_back(_pObserver);
	
	return S_OK;
}

HRESULT CKeyMgr::UnRegisterObserver(_uint _eSceneID, CBase * _pObserver)
{
	if (_eSceneID >= m_iMaxScene)
		return E_FAIL;

	if (nullptr == _pObserver)
		return E_FAIL;

	auto& iter = m_listObservers[_eSceneID].begin();
	for (;iter != m_listObservers[_eSceneID].end(); ++iter)
	{
		if (*iter == _pObserver)
		{
			m_listObservers[_eSceneID].erase(iter);
			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CKeyMgr::ClearObservers(_uint _eSceneID)
{
	if (_eSceneID >= m_iMaxScene)
		return E_FAIL;


	m_listObservers[_eSceneID].clear();
	return S_OK;
}

void CKeyMgr::Free()
{
	for (int i = 0; i < m_iMaxScene; ++i)
	{
		m_listObservers[i].clear();
	}

	Safe_Delete_Array(m_listObservers);
}
