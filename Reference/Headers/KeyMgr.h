#pragma once
#include "Base.h"

BEGIN(Engine)

#define VK_MAX 0xff

/*
만약 여기서 옵저버에 대한 레퍼런스 카운트를 세면 
옵저버가 UnRegister하는 시점이 애매해진다. Free()에서 하면
UnRegisterObserver가 불릴테고, UnRegisterObserver안에서는 Safe_Release(Observer)를 하므로 또 Free()를 부른다.
즉, 스택오버플로우가 난다.
*/
class ENGINE_DLL CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	explicit CKeyMgr();
	virtual	 ~CKeyMgr() = default;

public:
	bool Key_Pressing(int _Key, _uint _iSceneID);
	bool Key_Down(int _Key, _uint _iSceneID);
	bool Key_Up(int _Key, _uint _iSceneID);
	void Key_Update();

public:
	HRESULT Reserve_Key_Manager(_uint _iMaxSceneID);
	HRESULT RegisterObserver(_uint _eSceneID, CBase* _pObserver);
	HRESULT UnRegisterObserver(_uint _eSceneID, CBase* _pObserver);
	HRESULT	ClearObservers(_uint _eSceneID);
private:
	list<CBase*>* m_listObservers = nullptr;

private:
	_uint	m_iMaxScene = 0;

private:
	bool				m_bKeyState[VK_MAX];

	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};

END