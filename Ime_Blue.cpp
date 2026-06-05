// Copyright © 2014 CCP ehf.

#include "StdAfx.h"
#include "Ime.h"

BLUE_DEFINE( Ime );

const Be::ClassInfo* Ime::ExposeToBlue()
{
	EXPOSURE_BEGIN( Ime, "" )
		MAP_INTERFACE( Ime )

#if __APPLE__
        MAP_METHOD_AND_WRAP
        (
            "GetKeyboardLayout",
            GetKeyboardLayout,
            "Returns the input source ID string for the currently selected keyboard layout."
        )
#elif _WIN32
		MAP_METHOD_AND_WRAP( "SetHWND", SetHWND, "Please pass trinity.app.GetHwndAsLong()" )
		MAP_METHOD_AND_WRAP( "AssociateContext", AssociateContext, "..." )
		MAP_METHOD_AND_WRAP( "GetCursorPos", GetCursorPos, "..." )
		MAP_METHOD_AND_WRAP( "GetCompositionString", GetCompositionString, "..." )
		MAP_METHOD_AND_WRAP( "GetKeyboardLayout", GetKeyboardLayout, "..." )
		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS( "NotifyIME", NotifyIME, 1, "..." )
		MAP_METHOD_AND_WRAP( "SimulateHotKey", SimulateHotKey, "..." )
		MAP_METHOD_AND_WRAP( "GetOpenStatus", GetOpenStatus, "..." )
		MAP_METHOD_AND_WRAP( "SetOpenStatus", SetOpenStatus, "..." )
#endif
	
	EXPOSURE_END()
}

