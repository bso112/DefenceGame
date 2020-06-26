#pragma once



typedef struct tagBaseDesc
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;

	tagBaseDesc()
	{
		ZeroMemory(this, sizeof(tagBaseDesc));
	};
	tagBaseDesc(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vSize)
	{
		vPos = _vPos;
		vSize = _vSize;
	}

}BASEDESC;
