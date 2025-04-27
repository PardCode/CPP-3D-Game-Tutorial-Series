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




namespace dx3d
{
	class Logger final
	{
	public:
		enum class LogLevel
		{
			Error = 0,
			Warning,
			Info
		};

		explicit Logger(LogLevel logLevel = LogLevel::Error);
		void log(LogLevel level, const char* message) const;
	private:
		LogLevel m_logLevel = LogLevel::Error;
	};


#define DX3DLogInfo(message)\
	getLogger().log((Logger::LogLevel::Info), message);

#define DX3DLogWarning(message)\
	getLogger().log((Logger::LogLevel::Warning), message);

#define DX3DLogError(message)\
	getLogger().log((Logger::LogLevel::Error), message);

#define DX3DLogErrorAndThrow(message)\
	{\
	DX3DLogError(message);\
	throw std::runtime_error(message);\
	}


}