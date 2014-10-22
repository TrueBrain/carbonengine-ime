#include "StdAfx.h"

#if BLUE_WITH_PYTHON
#if IME_ENABLED

#include "Ime.h"

BLUE_DEFINE( Ime );
BLUE_DEFINE_INTERFACE( IIme );
BLUE_DEFINE_INTERFACE( IPythonMethods );

const Be::ClassInfo* Ime::ExposeToBlue()
{
	EXPOSURE_BEGIN(Ime, IME_Description)
		MAP_INTERFACE(IIme)
		MAP_INTERFACE(IPythonMethods)

		MAP_METHOD_AS_METHOD
		( 
			"__init__",
			Py__init__, 
			"Constructor arguments" 
		)	
		MAP_METHOD_AS_METHOD
		( 
			"SetHWND",
			PySetHWND,
			"Please pass eve.triapp.GetHwnd()" 
		)
		MAP_METHOD_AS_METHOD
		( 
			"AssociateContext",
			PyAssociateContext,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"OnLanguageChanged",
			PyOnLanguageChanged,
			"Call this to set a new language" 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetCompositionString",
			PyGetCompositionString,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetCandidateList",
			PyGetCandidateList,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetConversionStatus",
			PyGetConversionStatus,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"SetConversionStatus",
			PySetConversionStatus,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetConversionList",
			PyGetConversionList,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"IsIME",
			PyIsIME,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetImeId",
			PyGetImeId,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetReadingString",
			PyGetReadingString,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"ActivateKeyboardLayout",
			PyActivateKeyboardLayout,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetKeyboardLayout",
			PyGetKeyboardLayout,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetKeyboardLayoutList",
			PyGetKeyboardLayoutList,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"ShowReadingWindow",
			PyShowReadingWindow,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"NotifyIME",
			PyNotifyIME,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"DisableTextFrameService",
			PyDisableTextFrameService,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"SimulateHotKey",
			PySimulateHotKey,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetOpenStatus",
			PyGetOpenStatus,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"SetOpenStatus",
			PySetOpenStatus,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetGuideLine",
			PyGetGuideLine,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"SetCompositionString",
			PySetCompositionString,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"Backspace",
			PyBackspace,
			"..." 
		)
		MAP_METHOD_AS_METHOD
		( 
			"GetIMEFileName",
			PyGetIMEFileName,
			"..." 
		)

	EXPOSURE_END()

}


#endif
#endif

