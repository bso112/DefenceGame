#pragma once
#include "GameObject.h"

BEGIN(Client)
class CUI : public CGameObject
{
protected:
	explicit CUI(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	_int Get_Depth() { return m_iDepth; }
protected:
	_int	m_iDepth = 0;





};
END
