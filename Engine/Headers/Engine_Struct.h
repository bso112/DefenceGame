#pragma once


BEGIN(Engine)


typedef struct tagVertex_Texture
{
	// 3���� �����̽� ���� ��ġ�� ����. (���ý����̽�)
	D3DXVECTOR3			vPosition;
	D3DXVECTOR2			vTexUV;
}VTXTEX;

typedef struct tagVertex_Texture_Viewport
{
	
	// ��ġ
	D3DXVECTOR4		vPosition;

	// ������ �����ؾ��� �̹����� ��ǥ.
	D3DXVECTOR2		vTexUV;
	D3DXVECTOR4		vWinPos;

}VTXTEX_VIEWPORT;

END