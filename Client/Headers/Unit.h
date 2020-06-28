#pragma once
#include "GameObject.h"
#include "Value.h"
BEGIN(Client)
class CAIStateController;
class CUnit abstract : public CGameObject
{
public:
	typedef struct tagUnitStats
	{
		CValue<int>			iMaxHp = CValue<int>(0);
		_int				iCurrHp = 0;
		CValue<int>			iAtt = CValue<int>(0);
		_int				iAttRange = 0;
		_int				iMaxInfection = 0;
		_int				iCurrInfection = 0;
	}UNITSTATS;

protected:
	explicit CUnit(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUnit(const CUnit& _rhs);
	virtual ~CUnit() = default;


public:
	virtual	void Attack();

public:
	void	TakeDamage(_int iDamage, _int iInfection);
	
protected:
	virtual	void OnTakeDamage(_int iDamage, _int iInfection);

protected:
	UNITSTATS				m_tStats = {};
	_int					m_iRecogRange = 0;
	_bool					m_bInBuilding = false;
	_bool					m_bFriendly = false;
	CAIStateController*		m_pAICon = nullptr;




public:
	virtual	void	Free() override;
};

END