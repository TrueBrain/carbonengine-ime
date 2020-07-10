#pragma once

#include "ImeWrapper.h"

#if IME_ENABLED

BLUE_CLASS( Ime ) : public IRoot
{
public:
	EXPOSE_TO_BLUE();

	Ime( IRoot* lockobj = nullptr );
	~Ime();

	void SetHWND( uintptr_t handle );
	void AssociateContext( bool show );
	long GetCursorPos();
	std::wstring GetCompositionString( DWORD mask );
	size_t GetKeyboardLayout();
	bool NotifyIME( DWORD dwAction, DWORD dwIndex, DWORD dwValue );
	bool SimulateHotKey( DWORD hotkeyID );
	bool GetOpenStatus();
	bool SetOpenStatus( bool show );

private:
	HWND m_window = nullptr;
	HIMC m_imc = nullptr;
};

TYPEDEF_BLUECLASS( Ime );

#endif