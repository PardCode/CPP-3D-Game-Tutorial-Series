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
#include <DX3D/Core/Common.h>


namespace dx3d
{
	class Base
	{
	public:
		explicit Base(const BaseDesc& desc);
		virtual ~Base();
		virtual Logger& getLogger() noexcept final;

	protected:
		Base(const Base&) = delete;
		Base(Base&&) = delete;
		Base& operator = (const Base&) = delete;
		Base& operator=(Base&&) = delete;

	protected:
		Logger& m_logger;
	};
}

#define DX3DLogInfo(message)\
	DX3DLog(getLogger(), Logger::LogLevel::Info, message)

#define DX3DLogWarning(message)\
	DX3DLog(getLogger(), Logger::LogLevel::Warning, message)

#define DX3DLogError(message)\
	DX3DLog(getLogger(), Logger::LogLevel::Error, message)

#define DX3DLogThrowError(message)\
	DX3DLogThrow(getLogger(), std::runtime_error, Logger::LogLevel::Error, message)

#define DX3DLogThrowInvalidArg(message)\
	DX3DLogThrow(getLogger(), std::invalid_argument, Logger::LogLevel::Error, message)

