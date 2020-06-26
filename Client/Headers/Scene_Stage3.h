#pragma once

// 로딩씬을 의미하는 클래스.
// 내부적으로 로딩객체를 생성.
#include "Client_Defines.h"
#include "Scene.h"
#include "Loading.h"

BEGIN(Client)

class CScene_Stage3 final : public CScene
{
private:
	explicit CScene_Stage3(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Stage3() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();
public:
	static CScene_Stage3* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

END