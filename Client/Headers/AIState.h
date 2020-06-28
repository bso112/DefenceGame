#pragma once
#include "Base.h"

BEGIN(Client)
class CUnit;
class CAIState abstract : CBase
{
public:
	typedef struct tagStateDesc
	{
		_bool bCanAttack;
		_bool bAlerted;

	}STATEDESC;
public:
	//AI의 부모상태들이다.
	enum STATE { STATE_IDLE, STATE_HUNTING, STATE_RETREAT, STATE_WAIT, STATE_END };

public:
	explicit CAIState(CUnit* _pActor) { m_pActor = _pActor; Safe_AddRef(m_pActor); }

public:
	virtual STATE	Update(STATEDESC _tDesc, _double _timeDelta) = 0;
	virtual STATE	Start(STATEDESC _tDesc, _double _timeDelta) = 0;

protected:
	CUnit*	m_pActor = nullptr;

protected:
	virtual	void Free();
};




class CAIHunting : public CAIState
{
public:
	explicit CAIHunting(CUnit* _pActor) :CAIState(_pActor) {};


public:
	virtual STATE	Update(STATEDESC _tDesc, _double _timeDelta) override;
	virtual STATE	Start(STATEDESC _tDesc, _double _timeDelta) override;


	// CAIState을(를) 통해 상속됨
	virtual void Free() override;

};



class CAIIdle : public CAIState
{
public:
	explicit CAIIdle(CUnit* _pActor) :CAIState(_pActor) {};

public:
	virtual STATE	Update(STATEDESC _tDesc, _double _timeDelta) override;
	virtual STATE	Start(STATEDESC _tDesc, _double _timeDelta) override;


	// CAIState을(를) 통해 상속됨
	virtual void Free() override;

};

class CAIWait: public CAIState
{
public:
	explicit CAIWait(CUnit* _pActor) :CAIState(_pActor) {};

public:
	virtual STATE	Update(STATEDESC _tDesc, _double _timeDelta) override;
	virtual STATE	Start(STATEDESC _tDesc, _double _timeDelta) override;


	// CAIState을(를) 통해 상속됨
	virtual void Free() override;

};

class CAIRetreat : public CAIState
{
public:
	explicit CAIRetreat(CUnit* _pActor) :CAIState(_pActor) {};

public:
	virtual STATE	Update(STATEDESC _tDesc, _double _timeDelta) override;
	virtual STATE	Start(STATEDESC _tDesc, _double _timeDelta) override;


	// CAIState을(를) 통해 상속됨
	virtual void Free() override;

};



END