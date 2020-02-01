// Copyright (c) 2019 - 2020 PardCode
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
//Special thanks to albinopapa https://github.com/albinopapa for contributing to improve this code

#include "AppWindow.h"
#include <stdexcept>


int main()
{
	try
	{
		AppWindow app;
		if (app.init())
		{
			while (app.isRun())
			{
				app.broadcast();
			}
		}
	}
	catch( const std::runtime_error& error )
	{
		MessageBox( nullptr, error.what(), "An error has occured", MB_OK );
	}


	return 0;
}