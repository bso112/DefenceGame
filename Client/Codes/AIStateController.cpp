#include "stdafx.h"
#include "AIStateController.h"
#include "AIState.h"

CAIStateController::CAIStateController(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
	ZeroMemory(m_pStateArr, sizeof(m_pStateArr));
}

CAIStateController::CAIStateController(const CAIStateController & rhs)
	:CComponent(rhs)
{
	ZeroMemory(m_pStateArr, sizeof(m_pStateArr));
}

HRESULT CAIStateController::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CAIStateController::Ready_Component(void * _pArg)
{
	return S_OK;
}

_int CAIStateController::Update(CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	CAIState::STATE eState = m_pCurrState->Update(_tDesc, _timeDelta);
	if (eState < CAIState::STATE_END)
	{
		m_pCurrState = m_pStateArr[eState];
		m_pCurrState->Start(_tDesc, _timeDelta);
	}
	return _int();
}


HRESULT CAIStateController::Set_State(CAIState::STATE _eState, CAIState * _pAIState)
{
	if (CAIState::STATE_END <= _eState ||
		nullptr == _pAIState)
		return E_FAIL;

	m_pStateArr[_eState] = _pAIState;
	return S_OK;
}

HRESULT CAIStateController::Set_Default_State(CAIState::STATE _eState, CAIState::STATEDESC _tDesc, _double _timeDelta)
{
	if (CAIState::STATE_END <= _eState)
		return E_FAIL;

	m_pCurrState = m_pStateArr[_eState];
	m_pCurrState->Start(_tDesc, _timeDelta);
	return S_OK;
}



CAIStateController * CAIStateController::Create(PDIRECT3DDEVICE9 _pGraphic_Device)
{
	CAIStateController* pInstance = new CAIStateController(_pGraphic_Device);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Fail to create CAnimator");
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

CComponent * CAIStateController::Clone_Component(void * _pArg)
{
	CAIStateController* pInstance = new CAIStateController(*this);

	if (FAILED(pInstance->Ready_Component(_pArg)))
	{
		MSG_BOX("Fail to clone CAnimator");
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

void CAIStateController::Free()
{
	for (auto& pState : m_pStateArr)
	{
		Safe_Release(pState);
	}
	CComponent::Free();
}
