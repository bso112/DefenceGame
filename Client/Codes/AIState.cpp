#include "stdafx.h"
#include "AIState.h"
#include "Unit.h"
#include "PickingMgr.h"
#include "Building.h"

void CAIState::Free()
{
	Safe_Release(m_pActor);
}

CAIState::STATE CAIHunting::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{

	CPickingMgr* pPickMgr = CPickingMgr::Get_Instance();
	if (nullptr == pPickMgr) return STATE_END;
	if (nullptr == m_pActor) return STATE_END;
	if (nullptr == m_pActor->m_pTransform) return STATE_END;

	//컨트롤 모드로
	if (m_pActor->m_bControlMode)
		return STATE_WAIT;

	//주변을 조사해서 가장가까운 게임오브젝트를 받는다.
	_float dist = 0.f;
	CGameObject* pTarget = pPickMgr->OverlapSphere_Closest(m_pActor->m_pTransform->Get_State(CTransform::STATE_POSITION), FLT_MAX, &dist,m_pActor);
	if (nullptr == pTarget) return STATE_END;

	//도착이면
	if (dist < 2.f)
	{
		//공격
		CBuilding* pBuilding = dynamic_cast<CBuilding*>(pTarget);
		if (nullptr != pBuilding)
			pBuilding->Get_Damage(m_pActor->m_tUnitStats.iAtt.GetValue());
		CUnit*		pUnit = dynamic_cast<CUnit*>(pTarget);
		if (nullptr != pUnit)
			pUnit->TakeDamage(m_pActor->m_tUnitStats.iAtt.GetValue(), m_pActor->m_tUnitStats.iAtt.GetValue());
	}

	//가장 가까운 게임 오브젝트로 간다.
	CTransform* pTargetTransform = (CTransform*)pTarget->Find_Component(L"Com_Transform");
	if (nullptr == pTargetTransform) return STATE_END;
	m_pActor->m_pTransform->Go_Dst(pTargetTransform->Get_State(CTransform::STATE_POSITION), _timeDelta);
	

	return STATE_END;
}

CAIState::STATE CAIHunting::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return STATE_END;
}

void CAIHunting::Free()
{
	CAIState::Free();
}

CAIState::STATE CAIIdle::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return STATE_END;
}

CAIState::STATE CAIIdle::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return STATE_END;
}

void CAIIdle::Free()
{
	CAIState::Free();
}

CAIState::STATE CAIWait::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return STATE_END;
}

CAIState::STATE CAIWait::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return STATE_END;
}

void CAIWait::Free()
{
	CAIState::Free();
}

CAIState::STATE CAIRetreat::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return STATE_END;
}

CAIState::STATE CAIRetreat::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return STATE_END;
}

void CAIRetreat::Free()
{
	CAIState::Free();
}
