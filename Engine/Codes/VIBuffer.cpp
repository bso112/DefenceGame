#include "..\Headers\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVBuffer(rhs.m_pVBuffer)
	, m_pIBuffer(rhs.m_pIBuffer)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_pVerticesPos(rhs.m_pVerticesPos)
	, m_iIndexSize(rhs.m_iIndexSize)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_eIndexFormat(rhs.m_eIndexFormat)
{
	Safe_AddRef(m_pIBuffer);
	Safe_AddRef(m_pVBuffer);	
}

HRESULT CVIBuffer::Ready_Component_Prototype()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pVerticesPos = new _float3[m_iNumVertices];

	// 정점들을 보관하는 배열공간을 할당했다. 	
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVBuffer, nullptr)))
		return E_FAIL;	

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSize * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIBuffer, nullptr)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CVIBuffer::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render_VIBuffer()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVBuffer)
		return E_FAIL;

	// 1. 장치객체가 참조해야할 정점버퍼를 장치 소켓중 몇번째에 연결할까요? 
	m_pGraphic_Device->SetStreamSource(0, m_pVBuffer, 0, m_iStride);	
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetIndices(m_pIBuffer);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

_bool CVIBuffer::Pick_Polygon(HWND hWnd, _matrix WorldMatrix, _float3 * pOut)
{
	MSG_BOX("Error : Calling Parent VIBuffer Pick");

	return false;
}

void CVIBuffer::Free()
{
	if(false == m_isClone)
		Safe_Delete_Array(m_pVerticesPos);

	Safe_Release(m_pIBuffer);
	Safe_Release(m_pVBuffer);

	CComponent::Free();
}
