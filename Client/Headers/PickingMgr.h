#pragma once
#include "Base.h"
#include "Layer.h"
#include "UI.h"



BEGIN(Client)
class CTerrain;
class CInteractable;
class CPickingMgr : public CBase
{
	DECLARE_SINGLETON(CPickingMgr)

public:
	enum CONTROL_MODE
	{
		MODE_NORMAL,
		MODE_BARRICADE,
		MONE_COMMANDCENTER,
		MODE_ATTACKTOWER,
		MODE_UNIT,
		MODE_BUILDING_INTERACT,
		MODE_UNIT_INTERACT,
		MODE_IN_WAVE,
		MODE_END
	};

	enum UI_TYPE
	{
		UI_ALWAYS,
		UI_INTERACT_ONLY,
		UI_PURCHASE_ONLY,
		UI_NORMAL_ONLY,
		UI_END
	};

private:
	explicit CPickingMgr();
	virtual ~CPickingMgr() = default;

public:
	HRESULT	Ready_PickingMgr(CTerrain* pTerrain);

	HRESULT	Register_Observer(CInteractable* pObj);
	HRESULT	UnRegister_Observer(CInteractable* pObj);
public:
	HRESULT	Get_Route(_float3 _WorldSrc, _float3 _WorldDst, vector<_float3>& _out, CGameObject* pSelf);
	HRESULT Get_Route(_float3 _WorldSrc, POINT _ViewPortDst, vector<_float3>& _out, CGameObject* pSelf);
	_bool	Get_WorldMousePos(POINT _pt, _float3* _vWorldMouse);

public:
	//vPos �� �������� _fRadius �����ȿ� �ִ� ���ӿ�����Ʈ���� ��ȯ�Ѵ�. �ڱ��ڽ�(pSelf)�� �������� �ʴ´�.
	vector<CGameObject*> OverlapSphere(_float3 vPos, _float _fRadius, CGameObject* pSelf = nullptr);
	//�����ȿ� ���� ����� ���ӿ�����Ʈ�� ��ȯ�Ѵ�. �ڱ��ڽ�(pSelf)�� �������� �ʴ´�.
	CGameObject* OverlapSphere_Closest(_float3 vPos, _float _fRadius, _float* pDist, CGameObject* pSelf = nullptr);
	CTerrain* Get_Terrain() { return m_pTerrain; }
	CONTROL_MODE Get_Mode() { return m_eMode; }
	HRESULT	Pick_Object(POINT _ViewPortPoint, _float3* pHitPos);
	void Set_Mode(CONTROL_MODE _eMode) { m_eMode = _eMode; }
	void Display_Stats();

public:
	virtual HRESULT	OnKeyDown(_int KeyCode);
	virtual void Check_Mouse();
	void Add_UI(CUI* _pUI, UI_TYPE _eType) { m_listUI[_eType].push_back(_pUI); }
	void Update_UI();
public:
	void	Clear_DeadFocus();

private:
	HRESULT	PickObject();
	HRESULT	InstallObject();
	void InActiveAllUI();
	void ActiveUI(UI_TYPE _eType);

private:
	//HRESULT	PickObject();
	//HRESULT	InstallObject();
	HRESULT	InteractObject();
private:
	CTerrain*					m_pTerrain = nullptr;
	list<CInteractable*>		m_listObject;
	CInteractable*				m_pFocus = nullptr;

public:
	virtual void Free() override;

private:
	CONTROL_MODE m_eMode = MODE_NORMAL;
	list<CUI*> m_listUI[UI_END];
	_bool m_bStatWinOpen = false;
};
END
