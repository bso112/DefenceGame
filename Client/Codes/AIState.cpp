#include "stdafx.h"
#include "AIState.h"
#include "Unit.h"

CAIState::STATE CAIState::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

CAIState::STATE CAIState::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

void CAIState::Free()
{
	Safe_Release(m_pActor);
}

CAIState::STATE CAIHunting::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

CAIState::STATE CAIHunting::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

void CAIHunting::Free()
{
	CAIState::Free();
}

CAIState::STATE CAIIdle::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

CAIState::STATE CAIIdle::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

void CAIIdle::Free()
{
	CAIState::Free();
}

CAIState::STATE CAIWait::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

CAIState::STATE CAIWait::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

void CAIWait::Free()
{
	CAIState::Free();
}

CAIState::STATE CAIRetreat::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

CAIState::STATE CAIRetreat::Start(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	return CAIState::STATE();
}

void CAIRetreat::Free()
{
	CAIState::Free();
}
