#include "AppWindow.h"
#include <stdexcept>


int main()
{
	try
	{
		AppWindow app;
		while( app.isRun() )
		{
			app.broadcast();
		}
	}
	catch( const std::runtime_error& error )
	{
		MessageBox( nullptr, error.what(), "An error has occured", MB_OK );
	}


	return 0;
}