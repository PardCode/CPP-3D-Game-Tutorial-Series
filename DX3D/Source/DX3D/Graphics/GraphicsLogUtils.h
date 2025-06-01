/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2025, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#pragma once
#include<DX3D/Core/Logger.h>
#include <d3d11.h>

namespace dx3d
{
	namespace GraphicsLogUtils
	{
		inline void CheckShaderCompile(Logger& logger, HRESULT hr, ID3DBlob* errorBlob)
		{
			auto errorMsg = errorBlob ? static_cast<const char*>(errorBlob->GetBufferPointer()) : nullptr;

			if (FAILED(hr))
				DX3DLogThrow(logger, std::runtime_error, Logger::LogLevel::Error, errorMsg ? errorMsg :
					"Shader compilation failed.");
			if (errorMsg)
				DX3DLog(logger, Logger::LogLevel::Warning, errorMsg);
		}

	}
}

#define DX3DGraphicsLogThrowOnFail(hr,message)\
	{\
	auto res = (hr);\
	if (FAILED(res))\
		DX3DLogThrowError(message);\
	}


#define DX3DGraphicsCheckShaderCompile(hr, errorBlob)\
{\
auto res = (hr);\
dx3d::GraphicsLogUtils::CheckShaderCompile(getLogger(), res, errorBlob);\
}