#include "stdafx.h"
#include "..\Headers\FontManager.h"

USING(Client)

IMPLEMENT_SINGLETON(CFontManager)

CFontManager::CFontManager()
{
}

HRESULT CFontManager::Ready_FontManager(PDIRECT3DDEVICE9 pGraphic_Device)
{
	if (nullptr == pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);

	if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
		return E_FAIL;

	D3DXFONT_DESC			FontDesc;
	ZeroMemory(&FontDesc, sizeof(D3DXFONT_DESC));

	FontDesc.Width = 7.5;
	FontDesc.Height = 15;
	FontDesc.Weight = FW_BOLD;
	lstrcpy(FontDesc.FaceName, L"Consolas");
	FontDesc.CharSet = HANGUL_CHARSET;

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &FontDesc, &m_pFont)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFontManager::Draw_Font(_float3 vPos, _float3 vScale, _tchar szString[], D3DXCOLOR tColor,_int Weight)
{
	if (nullptr == szString)
		return E_FAIL;

	_matrix		FontMatrix;
	D3DXMatrixIdentity(&FontMatrix);
	_matrix		ScaleMatrix;


	D3DXMatrixScaling(&FontMatrix, vScale.x, vScale.y, vScale.z);
	D3DXMatrixTranslation(&ScaleMatrix,vPos.x, vPos.y, vPos.z);

	FontMatrix = FontMatrix*ScaleMatrix;

	m_pSprite->SetTransform(&FontMatrix);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, szString, lstrlen(szString), nullptr, DT_LEFT, tColor);

	m_pSprite->End();
	return S_OK;
}

void CFontManager::Draw_Gold(_float3 vPos, _int iGod,_float3 vScale, D3DXCOLOR tColor, _int Weight)
{
	_tchar szGold[16] = L"";
	wsprintf(szGold, L"%d", iGod);
	_matrix		FontMatrix;
	D3DXMatrixIdentity(&FontMatrix);
	_matrix		ScaleMatrix;


	D3DXMatrixScaling(&FontMatrix, vScale.x, vScale.y, vScale.z);
	D3DXMatrixTranslation(&ScaleMatrix, vPos.x, vPos.y, vPos.z);

	FontMatrix = FontMatrix*ScaleMatrix;

	m_pSprite->SetTransform(&FontMatrix);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, szGold, lstrlen(szGold), nullptr, DT_LEFT, tColor);

	m_pSprite->End();

}

void CFontManager::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pSprite);
	Safe_Release(m_pFont);
}

