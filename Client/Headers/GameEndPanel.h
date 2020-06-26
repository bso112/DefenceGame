#pragma once
#include "GameObject.h"
#include "Management.h"

BEGIN(Client)
class CMyImage;
class CMyButton;
class CClock_Basic;
class CGameEndPanel final : public CGameObject
{
public:
	typedef struct tagStateDesc
	{
		_uint					m_iDepth = 0;
	}STATEDESC;

protected:
	explicit CGameEndPanel(PDIRECT3DDEVICE9 _pGraphic_Device) :CGameObject(_pGraphic_Device) {};
	explicit CGameEndPanel(CGameEndPanel& _rhs);
	virtual ~CGameEndPanel() = default;


	
private:
	STATEDESC		m_tDesc;
	CMyImage*		m_pWinBanner = nullptr;
	CMyButton*		m_pNextStageBtn = nullptr;

	CClock_Basic*	m_pBtnActiveClock = nullptr;

	_bool			m_bSceneChange = false;


public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual _uint Get_Depth();


public:
	static CGameEndPanel*	Create(PDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
public:

	virtual void Free() override;

};

END