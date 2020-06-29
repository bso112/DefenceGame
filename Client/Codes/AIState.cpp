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
	vector<CGameObject*> vecGameObject = pPickMgr->OverlapSphere(m_pActor->m_pTransform->Get_State(CTransform::STATE_POSITION), m_pActor->m_iRecogRange);
	if (vecGameObject.empty()) return STATE_END;
	for (auto& go : vecGameObject)
	{
		if (go == m_pActor)
			continue;

		CBuilding* pBuilding = dynamic_cast<CBuilding*>(go);
		if (pBuilding != nullptr)
		{
			pBuilding->Get_Damage(m_pActor->m_tUnitStats.iAtt.GetValue());
		}
	}


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
