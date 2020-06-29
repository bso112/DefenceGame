#include "stdafx.h"
#include "..\Headers\UI.h"


CUI::CUI(PDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CUI::CUI(const CUI & rhs)
	:CGameObject(rhs)
{
}
