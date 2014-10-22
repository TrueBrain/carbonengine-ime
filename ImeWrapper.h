#pragma once
#include <dimm.h>

#define IMM32_DLLNAME "\\imm32.dll"
#define VER_DLLNAME "\\version.dll"

#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) \
    ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )	// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )	// New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )	// New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )	// New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )	// New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )	// New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )	// New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41	( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )	// MSPY1.5	// SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42	( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )	// MSPY2	// Win2k/WinME
#define IMEID_CHS_VER53	( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )	// MSPY3	// WinXP

class ImeWrapper
{
public:
	ImeWrapper(void);
	~ImeWrapper(void);

	static void SetupImeApi();

    static INPUTCONTEXT* ImmLockIMC(HIMC hIMC);
    static BOOL ImmUnlockIMC(HIMC hIMC);
    static LPVOID ImmLockIMCC(HIMCC hIMCC);
    static BOOL ImmUnlockIMCC(HIMCC hIMCC);
    static BOOL ImmDisableTextFrameService(DWORD idThread);
    static LONG ImmGetCompositionStringW(HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen);
    static DWORD ImmGetCandidateListW(HIMC hIMC, DWORD dwIndex, LPCANDIDATELIST lpCandList, DWORD dwBufLen);
    static HIMC ImmGetContext(HWND hWnd);
    static BOOL ImmReleaseContext(HWND hWnd, HIMC hIMC);
    static HIMC ImmAssociateContext(HWND hWnd, HIMC hIMC);
    static BOOL ImmGetOpenStatus(HIMC hIMC);
    static BOOL ImmSetOpenStatus(HIMC hIMC, BOOL fOpen);
    static BOOL ImmGetConversionStatus(HIMC hIMC, LPDWORD lpfdwConversion, LPDWORD lpfdwSentence);
    static HWND ImmGetDefaultIMEWnd(HWND hWnd);
    static UINT ImmGetIMEFileNameA(HKL hKL, LPSTR lpszFileName, UINT uBufLen);
    static UINT ImmGetVirtualKey(HWND hWnd);
    static BOOL ImmNotifyIME(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue);
    static BOOL ImmSetConversionStatus(HIMC hIMC, DWORD fdwConversion, DWORD fdwSentence);
	static DWORD ImmGetConversionListW(HKL hKL, HIMC hIMC, LPCWSTR lpSrc, LPCANDIDATELIST lpDst, DWORD dwBufLen, UINT uFlag);
    static BOOL ImmSimulateHotKey(HWND hWnd, DWORD dwHotKeyID);
    static BOOL ImmIsIME(HKL hKL);
	static DWORD GetImeId(UINT uIndex = 0);
	static HKL  GetHKL() { return s_hklCurrent; }
	static void SetHKL(HKL hkl){s_hklCurrent = hkl;}
    static WORD GetLanguage() { return LOWORD( s_hklCurrent ); }
    static WORD GetPrimaryLanguage() { return PRIMARYLANGID( LOWORD( s_hklCurrent ) ); }
    static WORD GetSubLanguage() { return SUBLANGID( LOWORD( s_hklCurrent ) ); }
	static DWORD ImmGetGuideLineW(HIMC hIMC, DWORD dwIndex, LPWSTR lpBuf, DWORD dwBufLen);
	static BOOL  ImmSetCompositionStringW(HIMC hIMC, DWORD dwIndex, LPVOID lpComp, DWORD dwCompLen, LPVOID lpRead, DWORD dwReadLen);

    // Function pointers: Traditional Chinese IME
    static UINT GetReadingString(HIMC himc, UINT uReadingBufLen, LPWSTR lpwReadingBufLen, PINT pnErrorIndex, BOOL * pfIsVertical, PUINT puMaxReadingLen);
    static BOOL ShowReadingWindow(HIMC himc, BOOL bShow);

    // Function pointers: Verion library imports
    static BOOL VerQueryValueA(const LPVOID pBlock, LPSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen);
    static BOOL GetFileVersionInfoA(LPSTR lpstFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
    static DWORD GetFileVersionInfoSizeA(LPSTR lpstrFilename, LPDWORD lpdwHandle);

    static void InitializeImm();
    static void UninitializeImm();

	static BOOL HasGetReadingString(){return m_fpGetReadingString !=  NULL;}
	static bool GetReadingWindowOrientation( DWORD dwId );

    static HKL		 s_hklCurrent;          // Current keyboard layout of the process

protected:
    static HINSTANCE s_hDllImm32;  // IMM32 DLL handle
    static HINSTANCE s_hDllVer;    // Version DLL handle
    static HIMC      s_hImcDef;    // Default input context
    static HINSTANCE s_hDllIme;           // Instance handle of the current IME module

    // Function pointers: IMM32
    static INPUTCONTEXT* (WINAPI * m_fpImmLockIMC)(HIMC hIMC);
    static BOOL (WINAPI * m_fpImmUnlockIMC)(HIMC hIMC);
    static LPVOID (WINAPI * m_fpImmLockIMCC)(HIMCC hIMCC);
    static BOOL (WINAPI * m_fpImmUnlockIMCC)(HIMCC hIMCC);
    static BOOL (WINAPI * m_fpImmDisableTextFrameService)(DWORD idThread);
    static LONG (WINAPI * m_fpImmGetCompositionStringW)(HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen);
    static DWORD (WINAPI * m_fpImmGetCandidateListW)(HIMC hIMC, DWORD dwIndex, LPCANDIDATELIST lpCandList, DWORD dwBufLen);
    static HIMC (WINAPI * m_fpImmGetContext)(HWND hWnd);
    static BOOL (WINAPI * m_fpImmReleaseContext)(HWND hWnd, HIMC hIMC);
    static HIMC (WINAPI * m_fpImmAssociateContext)(HWND hWnd, HIMC hIMC);
    static BOOL (WINAPI * m_fpImmGetOpenStatus)(HIMC hIMC);
    static BOOL (WINAPI * m_fpImmSetOpenStatus)(HIMC hIMC, BOOL fOpen);
    static BOOL (WINAPI * m_fpImmGetConversionStatus)(HIMC hIMC, LPDWORD lpfdwConversion, LPDWORD lpfdwSentence);
	static DWORD (WINAPI * m_fpImmGetConversionListW)(HKL hKL, HIMC hIMC, LPCWSTR lpSrc, LPCANDIDATELIST lpDst, DWORD dwBufLen, UINT uFlag);
    static HWND (WINAPI * m_fpImmGetDefaultIMEWnd)(HWND hWnd);
    static UINT (WINAPI * m_fpImmGetIMEFileNameA)(HKL hKL, LPSTR lpszFileName, UINT uBufLen);
    static UINT (WINAPI * m_fpImmGetVirtualKey)(HWND hWnd);
    static BOOL (WINAPI * m_fpImmNotifyIME)(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue);
    static BOOL (WINAPI * m_fpImmSetConversionStatus)(HIMC hIMC, DWORD fdwConversion, DWORD fdwSentence);
    static BOOL (WINAPI * m_fpImmSimulateHotKey)(HWND hWnd, DWORD dwHotKeyID);
    static BOOL (WINAPI * m_fpImmIsIME)(HKL hKL);
	static DWORD (WINAPI * m_fpImmGetGuideLineW)(HIMC hIMC, DWORD dwIndex, LPWSTR lpBuf, DWORD dwBufLen);
	static BOOL (WINAPI * m_fpImmSetCompositionStringW)(HIMC hIMC, DWORD dwIndex, LPVOID lpComp, DWORD dwCompLen, LPVOID lpRead, DWORD dwReadLen);

    // Function pointers: Traditional Chinese IME
    static UINT (WINAPI * m_fpGetReadingString)(HIMC himc, UINT uReadingBufLen, LPWSTR lpwReadingBufLen, PINT pnErrorIndex, BOOL * pfIsVertical, PUINT puMaxReadingLen);
    static BOOL (WINAPI * m_fpShowReadingWindow)(HIMC himc, BOOL bShow);

    // Function pointers: Verion library imports
    static BOOL (APIENTRY * m_fpVerQueryValueA)(const LPVOID pBlock, LPSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen);
    static BOOL (APIENTRY * m_fpGetFileVersionInfoA)(LPSTR lpstFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
    static DWORD (APIENTRY * m_fpGetFileVersionInfoSizeA)(LPSTR lpstrFilename, LPDWORD lpdwHandle);
};
