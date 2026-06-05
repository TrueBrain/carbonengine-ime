// Copyright © 2014 CCP ehf.

#pragma once

#if _WIN32

class ImeWrapper
{
public:
	ImeWrapper() = delete;

	static void InitializeImm();
	static void UninitializeImm();

	static LONG ImmGetCompositionStringW( HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen );
	static HIMC ImmGetContext( HWND hWnd );
	static BOOL ImmReleaseContext( HWND hWnd, HIMC hIMC );
	static HIMC ImmAssociateContext( HWND hWnd, HIMC hIMC );
	static BOOL ImmGetOpenStatus( HIMC hIMC );
	static BOOL ImmSetOpenStatus( HIMC hIMC, BOOL fOpen );
	static UINT ImmGetVirtualKey( HWND hWnd );
	static BOOL ImmNotifyIME( HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue );
	static BOOL ImmSimulateHotKey( HWND hWnd, DWORD dwHotKeyID );

private:
	static HINSTANCE s_hDllImm32;

	static LONG( WINAPI* m_fpImmGetCompositionStringW )( HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen );
	static HIMC( WINAPI* m_fpImmGetContext )( HWND hWnd );
	static BOOL( WINAPI* m_fpImmReleaseContext )( HWND hWnd, HIMC hIMC );
	static HIMC( WINAPI* m_fpImmAssociateContext )( HWND hWnd, HIMC hIMC );
	static BOOL( WINAPI* m_fpImmGetOpenStatus )( HIMC hIMC );
	static BOOL( WINAPI* m_fpImmSetOpenStatus )( HIMC hIMC, BOOL fOpen );
	static UINT( WINAPI* m_fpImmGetVirtualKey )( HWND hWnd );
	static BOOL( WINAPI* m_fpImmNotifyIME )( HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue );
	static BOOL( WINAPI* m_fpImmSimulateHotKey )( HWND hWnd, DWORD dwHotKeyID );
};

#endif
