#pragma once

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>

using namespace std;

typedef void(*Function)();

class MenuItem
{
	char *title;
	Function function;

public:
	MenuItem(char *_title, Function _function)
	{
		SetTitle(_title);
		SetFunction(_function);
	}

	MenuItem(MenuItem *menuItem)
	{
		SetTitle(menuItem->GetTitle());
		SetFunction(menuItem->GetFunction());
	}

	void SetTitle(char *_title)
	{
		int length = strlen(_title) + 1;
		title = new char[length];
		strcpy_s(title, length, _title);
	}

	char* GetTitle()
	{
		return title;
	}

	void SetFunction(Function _function)
	{
		function = _function;
	}

	Function GetFunction()
	{
		return function;
	}

	void Print()
	{
		cout << title << endl;
	}

	void Run()
	{
		function();
	}

	~MenuItem()
	{
		delete[] title;
	}
};

class Menu
{
	HANDLE hConsole;
	COORD menuPosition;
	char *title;
	int menuItemsCount;
	MenuItem **menuItems;
	int activeMenuItem;

public:
	Menu(char *_title)
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		menuPosition = { 0,0 };
		SetTitle(_title);
		menuItemsCount = 0;
		menuItems = new MenuItem*[menuItemsCount];
		activeMenuItem = 0;
	}

	void SetTitle(char *_title)
	{
		int length = strlen(_title) + 1;
		title = new char[length];
		strcpy_s(title, length, _title);
	}

	void AddMenuItem(char *_menuItemTitle, Function _function)
	{
		MenuItem **newMenuItems = new MenuItem*[menuItemsCount + 1];

		for (int i = 0; i < menuItemsCount; i++)
		{
			newMenuItems[i] = new MenuItem(menuItems[i]);
		}

		for (int i = 0; i < menuItemsCount; i++)
		{
			delete menuItems[i];
		}

		newMenuItems[menuItemsCount] = new MenuItem(_menuItemTitle, _function);

		++menuItemsCount;

		menuItems = newMenuItems;
	}

	void SetPosition(short _x, short _y)
	{
		menuPosition = { _x , _y };
	}

private:
	void Print()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);

		int x = menuPosition.X;
		int y = menuPosition.Y;

		system("cls");
		SetCursorPosition(x, y);
		cout << title << endl;
		for (int i = 0; i < menuItemsCount; i++)
		{
			SetCursorPosition(x, ++y);
			if (i == activeMenuItem)
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				menuItems[i]->Print();

				SetConsoleTextAttribute(hConsole, csbi.wAttributes);
			}
			else
			{
				menuItems[i]->Print();
			}
		}
	}

public:
	void Show()
	{
		enum
		{
			UP_ARROW_KEY = 72,
			DOWN_ARROW_KEY = 80,
			SPACE_KEY = 32,
			ENTER_KEY = 13,
			ESCAPE_KEY = 27
		};

		bool menu = true;
		int lastMenuItem = menuItemsCount - 1;

		while (menu)
		{
			Print();
			int key = _getch();
			switch (key)
			{
			case UP_ARROW_KEY:
				if (0 < activeMenuItem)
				{
					activeMenuItem -= 1;
				}
				else
				{
					activeMenuItem = lastMenuItem;
				}
				break;

			case DOWN_ARROW_KEY:
				if (activeMenuItem < lastMenuItem)
				{
					activeMenuItem += 1;
				}
				else
				{
					activeMenuItem = 0;
				}
				break;

			case ENTER_KEY:
				menuItems[activeMenuItem]->Run();
				break;

			case ESCAPE_KEY:
				menu = false;
				system("cls");
				break;
			}
		}
	}

	~Menu()
	{
		delete[] title;

		for (int i = 0; i < menuItemsCount; i++)
		{
			delete menuItems[i];
		}
	}

private:
	void SetCursorPosition(short x, short y)
	{
		SetConsoleCursorPosition(hConsole, COORD{ x , y });
	}
};