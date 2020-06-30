#include "stdafx.h"
#include "..\Headers\Unit.h"
#include "PickingMgr.h"
#include "AIStateController.h"
#include "StageUIMgr.h"
#include "FontManager.h"

CUnit::CUnit(PDIRECT3DDEVICE9 pGraphic_Device)
	:CInteractable(pGraphic_Device)
{
}

CUnit::CUnit(const CUnit & _rhs)
	: CInteractable(_rhs)
{
}

_int CUnit::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransform)
		return -1;
	//루트따라 이동
	if (m_bMoving)
	{
		if (m_iCurrDst >= m_Route.size())
		{
			m_bMoving = false;
			m_iCurrDst = 0;
			m_Route.clear();
			m_bControlMode = false;
			return 0;
		}

		_float fDist = D3DXVec3Length(&(m_Route[m_iCurrDst] - m_pTransform->Get_State(CTransform::STATE_POSITION)));
		if (fDist > 0.2f)
			m_pTransform->Go_Dst(m_Route[m_iCurrDst], TimeDelta);
		else
			++m_iCurrDst;
	}
	return _int();
}

void CUnit::Attack()
{

}

void CUnit::GoToDst(_float3 _vDst)
{
	CPickingMgr::Get_Instance()->Get_Route(m_pTransform->Get_State(CTransform::STATE_POSITION), _vDst, m_Route);
	if (m_Route.size() > 0)
		m_bMoving = true;

}

void CUnit::GoToDst(POINT _pt)
{
	CPickingMgr::Get_Instance()->Get_Route(m_pTransform->Get_State(CTransform::STATE_POSITION), _pt, m_Route);
	if (m_Route.size() > 0)
		m_bMoving = true;
}

void CUnit::TakeDamage(_int iDamage, _int iInfection)
{

}

void CUnit::Display_Stats()
{
	_float fOffX = 0.f;
	_float fOffY = 0.f;
	_tchar szChar[128];

	wsprintf(szChar, L"Attack \t :%d",_int(m_tUnitStats.iAtt.GetValue()));
	CFontManager::Get_Instance()->Draw_Font(_float3(g_iWinSizeX - 200.f + fOffX, 100.f + fOffY, 0.f), _float3(1.2f, 1.2f, 1.2f), szChar, D3DXCOLOR(0.f, 0.f, 0.f, 1.f),700);

	wsprintf(szChar, L"HP\t\t\t :%d / %d", _int(m_tUnitStats.iCurrHp), _int(m_tUnitStats.iMaxHp.GetValue()));
	CFontManager::Get_Instance()->Draw_Font(_float3(g_iWinSizeX - 200.f + fOffX, 100.f + fOffY + 25.f, 0.f), _float3(1.2f, 1.2f, 1.2f), szChar, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 700);

	wsprintf(szChar, L"Resell \t :%d", _int(m_tUnitStats.iCost * 0.5f));
	CFontManager::Get_Instance()->Draw_Font(_float3(g_iWinSizeX - 200.f + fOffX, 100.f + fOffY + 50.f, 0.f), _float3(1.2f, 1.2f, 1.2f), szChar, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 700);


	//CFontManager::Get_Instance()->Draw_Gold(_float3(g_iWinSizeX*0.5f, g_iWinSizeY*0.5f, 0.f),100, _float3(10.f, 10.f, 10.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f),700);
		//m_tUnitStats.iAtt;
}

void CUnit::Interact()
{
	CStageUIMgr::Get_Instance()->Set_StateToPanel(m_tUnitStats);
}

void CUnit::OnTakeDamage(_int iDamage, _int iInfection)
{
}

void CUnit::Free()
{
	Safe_Release(m_pAICon);
	Safe_Release(m_pTransform);
	CInteractable::Free();
}


