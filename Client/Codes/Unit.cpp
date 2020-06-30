#include "stdafx.h"
#include "..\Headers\Unit.h"
#include "PickingMgr.h"
#include "AIStateController.h"
#include "StageUIMgr.h"

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


