#pragma once
#include "Interactable.h"
#include "Value.h"
#include "Management.h"
BEGIN(Client)
class CAIStateController;
class CUnit abstract : public CInteractable
{
	friend class CAIState;
	friend class CAIHunting;

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
	virtual _int Update_GameObject(_double TimeDelta);

public:
	virtual	void Attack();
	
public:
	void	GoToDst(_float3 _vDst);
	void	GoToDst(POINT _pt);
	void	TakeDamage(_int iDamage, _int iInfection);

public:
	_bool	IsControllable() { return m_bFriendly && m_bControlMode; }
	
protected:
	virtual	void OnTakeDamage(_int iDamage, _int iInfection);
protected:
	UNITSTATS				m_tUnitStats;
	_int					m_iRecogRange = 0;
	_bool					m_bInBuilding = false;
	_bool					m_bFriendly = false;
	CAIStateController*		m_pAICon = nullptr;
	CTransform*				m_pTransform = nullptr;
	_bool					m_bMoving = false;
	_bool					m_bControlMode = false;


protected:
	_int					m_iCurrDst = 0;
	vector<_float3>			m_Route;





public:
	virtual	void	Free() override;
};

END