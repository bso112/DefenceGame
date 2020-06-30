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
	void	SetGameStart(_bool IsStart) { m_bGameStart = IsStart; }

public:
	_bool	IsGameStart() { return m_bGameStart; }
	

	HRESULT OnGameStart();
	HRESULT OnGameEnd();

	_int	Get_Gold() { return m_iGold; }
	void	Add_Gold(_int amount) { m_iGold += amount; }
	void	Sub_Gold(_int amount) { m_iGold -= amount; }
private:
	_int		m_iGold = 0;
	_bool		m_bGameStart = false;
public:
	virtual void Free() override;
};

END