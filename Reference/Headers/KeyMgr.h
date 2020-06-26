#pragma once
#include "Base.h"

BEGIN(Engine)

#define VK_MAX 0xff

/*
���� ���⼭ �������� ���� ���۷��� ī��Ʈ�� ���� 
�������� UnRegister�ϴ� ������ �ָ�������. Free()���� �ϸ�
UnRegisterObserver�� �Ҹ��װ�, UnRegisterObserver�ȿ����� Safe_Release(Observer)�� �ϹǷ� �� Free()�� �θ���.
��, ���ÿ����÷ο찡 ����.
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

	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
};

END