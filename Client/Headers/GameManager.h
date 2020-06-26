#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel;
class CCube;
class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager)
private:
	explicit CGameManager();
	virtual ~CGameManager() = default;
public:
	HRESULT Set_CurrentLevel(_uint _iLevel);
	void	Set_IsGameStart(_bool IsStart) { m_bGameStart = IsStart; }

public:
	_bool	Get_IsGameStart() { return m_bGameStart; }
	

	CCube* PickBrick(POINT pt);
	HRESULT OnGameStart();
	HRESULT OnGameEnd();
private:
	CLevel*		m_pCurrentLevel = nullptr;
	_bool		m_bGameStart = false;
	_bool		m_bGoal = false;

	_bool		m_bPickObject = false;
public:
	virtual void Free() override;
};

END