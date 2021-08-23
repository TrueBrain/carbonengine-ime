#import "KeyboardLayoutMac.h"

#import <Carbon/Carbon.h>


std::wstring GetKeyboardLayoutMac()
{
    TISInputSourceRef source = TISCopyCurrentKeyboardInputSource();
    auto layoutID = static_cast<NSString*>(TISGetInputSourceProperty(source, kTISPropertyInputSourceID));
    NSStringEncoding pEncode = CFStringConvertEncodingToNSStringEncoding( NSUTF32LittleEndianStringEncoding );
    NSData* pSData = [layoutID dataUsingEncoding: pEncode];
    return std::wstring ( static_cast<const wchar_t*>( [pSData bytes] ), [pSData length] / sizeof( wchar_t ) );
}
