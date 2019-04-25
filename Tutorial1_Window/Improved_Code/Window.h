#pragma once
#include <Windows.h>



class Window
{
public:
	//Initialize the window
	Window();
	void broadcast();
	bool isRun();

	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();

	//Release the window
	~Window();
protected:
	HWND m_hwnd = nullptr;
	//set this flag to true to indicate that the window is initialized and running
	bool m_is_run = true;
};

