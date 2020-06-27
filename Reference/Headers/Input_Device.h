#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:
	enum DIMOUSEKEY { DIMK_LBUTTON, DIMK_RBUTTON, DIMK_WHEEL, DIMK_X, DIMK_END };
	// DIMM_X 는 x축으로 움직인 정도 (LONG)를 의미한다.
	enum DIMOUSEMOVE { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };
private:
	explicit CInput_Device();
	virtual ~CInput_Device() = default;


public:
	_byte Get_DIKeyState(_ubyte byKey) { 
		return m_byKeyState[byKey]; }

	_byte Get_DIMouseKeyState(DIMOUSEKEY eMouseKey) {
		return m_MouseState.rgbButtons[eMouseKey]; }

	//DIMOUSESTATE 는 모든 멤버변수가 4바이트라서 이런식으로 가능.
	//마우스가 움직인 정도를 반환한다.
	_long Get_DIMouseMoveState(DIMOUSEMOVE eMouseMove) {
		return *((_long*)&m_MouseState + eMouseMove); }
public:
	HRESULT Ready_Input_Device(HINSTANCE g_hInst, HWND hWnd);
	HRESULT Set_Input_Device_State();
private:
	LPDIRECTINPUT8			m_pSDK = nullptr; // 장치의 정보를 조사하거나 생성하기위한 객체
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
private:
	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_MouseState;
public:
	virtual void Free();
};

END