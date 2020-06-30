#pragma once
#include "Base.h"

BEGIN(Client)

class CFontManager final:
	public CBase
{
	DECLARE_SINGLETON(CFontManager)
private:
	CFontManager();
	virtual ~CFontManager()=default;
public:
	HRESULT Ready_FontManager(PDIRECT3DDEVICE9 pGraphic_Device);
	HRESULT Draw_Font(_float3 vPos, _float3 vScale, _tchar szString[], D3DXCOLOR tColor,_int Weight= FW_BOLD);
	void	Draw_Gold(_float3 vPos, _int iGod, _float3 vScale, D3DXCOLOR tColor, _int Weight);
private:
	PDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	LPD3DXFONT			m_pFont = nullptr;
	LPD3DXSPRITE		m_pSprite = nullptr;
public:
	virtual void Free();
};

END