#include "MainForm.h"
#include "KeyListener.h"
#include "BoomarkManager.h"

#include <thread>

using namespace System;
using namespace System::Windows::Forms;

const bool SHOW_CONSOLE = true;

void ProcessBoomark()
{
	if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000)) return;
	if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000)) return;

	std::cout << "HELO" << std::endl;

	BoomarkManager::SaveBoomark();
}

void thread1()
{
	while (true)
	{
		//std::cout << "loop" << std::endl;

		KeyListener::Update();

		Sleep(1);
	}
}

void main(array<String^>^ args)
{
	//console

	if (SHOW_CONSOLE)
	{
		AllocConsole();

		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}

	//

	std::cout << "Listening keys..." << std::endl;

	KeyListener::Listen(VK_F11, ProcessBoomark); // CTRL + SHITF + F11
	KeyListener::Listen(66, ProcessBoomark); // CTRL + SHITF + B

	std::cout << "Starting thread..." << std::endl;

	std::thread t1(thread1);

	//

	std::cout << "Running application..." << std::endl;

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	PersonalTools::MainForm form;

	Application::Run(% form);

	t1.detach();
}