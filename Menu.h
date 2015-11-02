#pragma once

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>

using namespace std;

#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define UP_ARROW 72
#define DOWN_ARROW 80
#define SPACE_KEY 32
#define ENTER_KEY 13

char* FixString(char *oldString)
{
	int length = strlen(oldString) + 1;
	char *newString = new char[length];
	strcpy_s(newString, length, oldString);

	return newString;
}

void SetCursorPosition(short xPos, short yPos)
{
	COORD position = { xPos, yPos };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

class MenuItem
{
	char *title;
	void(*method)();
public:
	MenuItem(char *_title, void(*_method)())
	{
		SetTitle(_title);
		SetMethod(_method);
	}

	MenuItem(MenuItem *menuItem)
	{
		SetTitle(menuItem->GetTitle());
		SetMethod(menuItem->GetMethod());
	}

	void SetTitle(char *_title)
	{
		title = FixString(_title);
	}

	char* GetTitle()
	{
		return title;
	}

	void SetMethod(void(*_method)())
	{
		method = _method;
	}

	typedef void(*ptr)();
	ptr GetMethod()
	{
		return method;
	}

	void Print()
	{
		cout << title << endl;
	}

	void Run()
	{
		method();
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
	char *menuTitle;
	int menuItemsCount;
	MenuItem **menuItems;
	
	int activeMenuItem;
public:
	Menu(char *_menuTitle)
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		menuPosition = {0,0};

		menuTitle = FixString(_menuTitle);

		menuItemsCount = 0;

		menuItems = new MenuItem*[menuItemsCount];

		activeMenuItem = 0;
	}

	void AddMenuItem(char *_menuItemTitle, void(*_method)())
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

		newMenuItems[menuItemsCount] = new MenuItem(_menuItemTitle, _method);

		++menuItemsCount;

		menuItems = newMenuItems;
	}

	void SetPosition(int _x, int _y)
	{
		menuPosition.X = _x;
		menuPosition.Y = _y;
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
		cout << menuTitle << endl;
		for (int i = 0; i < menuItemsCount; i++)
		{
			SetCursorPosition(x, ++y);
			if(i == activeMenuItem)
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
		int key;
		int lastMenuItem = menuItemsCount - 1;

		Print();
		while (true)
		{
			key = _getch();

			switch (key)
			{
			case UP_ARROW:
				if (0 < activeMenuItem)
				{
					activeMenuItem -= 1;
				}
				else
				{
					activeMenuItem = lastMenuItem;
				}
				Print();
				break;

			case DOWN_ARROW:
				if (activeMenuItem < lastMenuItem)
				{
					activeMenuItem += 1;
				}
				else
				{
					activeMenuItem = 0;
				}
				Print();
				break;

			case ENTER_KEY:
				menuItems[activeMenuItem]->Run();
				break;

				default:
					Print();
					break;
			}
		}

	}

	~Menu()
	{
		delete[] menuTitle;

		for (int i = 0; i < menuItemsCount; i++)
		{
			delete menuItems[i];
		}
	}
};