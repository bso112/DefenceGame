#pragma once
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cube :public CVIBuffer
{
protected:
	explicit CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Cube(const CVIBuffer_Cube& rhs);
	virtual ~CVIBuffer_Cube() = default;
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	virtual _bool Pick_Polygon(HWND hWnd, _matrix WorldMatrix, _float3* pOut) override;
public:
	static CVIBuffer_Cube*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent*		Clone_Component(void * pArg);
public:
	virtual void Free();
};
END
