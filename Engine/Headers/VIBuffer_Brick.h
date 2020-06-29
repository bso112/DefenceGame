#pragma once
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Brick :public CVIBuffer
{
protected:
	explicit CVIBuffer_Brick(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Brick(const CVIBuffer_Brick& rhs);
	virtual ~CVIBuffer_Brick() = default;
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	virtual _bool Picking(POINT pt, HWND hWnd, _matrix WorldMatrix, _float3* pOut) override;
public:
	static CVIBuffer_Brick*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent*		Clone_Component(void * pArg);
public:
	virtual void Free();
};
END
