/**
* Overlord Project
* Copyright (C) 2016-2017 RangeMachine
*/

#pragma once

#include "dllmain.h"

class Input
{
public:
	static Input* GetInstance();

	void StartThread();
	void StopThread();

private:
	Input();
	~Input();

	static void MenuKeyMonitor();

	HANDLE m_hThread;

	static Input* m_pInstance;
};
