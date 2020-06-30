#pragma once
#include "Base.h"
#include "Unit.h"
#include "Building.h"

BEGIN(Client)
class CMyImage;
class CMyButton;
class CStageUIMgr : public CBase
{
	DECLARE_SINGLETON(CStageUIMgr)
public:
	explicit CStageUIMgr() {};
	virtual ~CStageUIMgr() = default;


public:
	virtual HRESULT Ready_StageUIMgr();

public:
	vector<RECT>	Get_UIRect();


public:
	void Set_StateToPanel(CUnit::UNITSTATS tagStats);
	void Set_StateToPanel_Building(CBuilding::BUILDING_STAT tagStats_building);
	void Set_Active_GameOverPanel();

private:

	CMyButton*	m_pGameStartBtn = nullptr;
	CMyButton*	m_pBuildBarricadeBtn = nullptr;
	CMyButton*	m_pBuildCommandCenterBtn = nullptr;
	CMyButton*	m_pCreateUnit1Btn = nullptr;
	CMyButton*	m_pCreateUnit2Btn = nullptr;
	CMyButton*	m_pCreateUnit3Btn = nullptr;
	CMyButton*	m_pResellBtn = nullptr;

	CMyImage*	m_pWaveIndicator = nullptr;
	CMyImage*	m_pGoldIndicator = nullptr;
	CMyImage*	m_pStatsUI = nullptr;
	CMyImage*	m_pGameOverBanner = nullptr;
	
	
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

};
END
