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
	void	SetWaveStart(_bool IsStart) { m_bGameStart = IsStart;  m_bInWait = false; if (IsStart) { OnGameStart(); } else { OnGameEnd(); ++m_iWave;} }

public:
	_int	Update();
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
	//정비시간인가
	_bool		m_bInWait = false;
	_int		m_iWave = 0;
public:
	virtual void Free() override;
};

END