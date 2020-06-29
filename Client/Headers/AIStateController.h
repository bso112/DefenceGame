#pragma once
#include "Component.h"
#include "AIState.h"
BEGIN(Client)

class CAIStateController final : public CComponent
{

protected:
	explicit CAIStateController(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAIStateController(const CAIStateController& rhs);
	virtual ~CAIStateController() = default;


public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* _pArg);
	
public:
	_int	Update(CAIState::STATEDESC _tDesc, _double _timeDelta);

public:
	HRESULT	Set_State(CAIState::STATE _eState, CAIState* _pAIState);
	HRESULT	Set_Default_State(CAIState::STATE _eState, CAIState::STATEDESC _tDesc, _double _timeDelta);


private:
	CAIState*			m_pStateArr[CAIState::STATE_END];
	CAIState*			m_pCurrState = nullptr;
	CAIState::STATE		m_eCurrState = CAIState::STATE_END;

public:
	static CAIStateController* Create(PDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CComponent* Clone_Component(void * _pArg = nullptr) override;
	virtual void Free();
};

END