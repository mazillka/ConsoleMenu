using System;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;

namespace ConsoleApplication
{
	public delegate void Method();

	public class Menu
	{
		private readonly Dictionary<string, Method> _menuItems;
		private readonly string _name;
		private int _currentMenuItem;
		private Point _position;

		public Menu(string name)
		{
			_position = new Point(0, 0);
			_name = name;
			_currentMenuItem = 0;
			_menuItems = new Dictionary<string, Method>();
        }

		public void AddMenuItem(string name, Method method)
		{
			_menuItems.Add(name, method);
		}

		public void SetPosition(int x, int y)
		{
			_position.X = x;
			_position.Y = y;
        }

		private void Print()
		{
			var x = _position.X;
			var y = _position.Y;

			Console.Clear();
			Console.SetCursorPosition(x, y++);
			Console.WriteLine("{0}", _name);
			foreach (var entry in _menuItems)
			{
				Console.SetCursorPosition(x, y++);
				if (entry.Equals(_menuItems.ElementAt(_currentMenuItem)))
				{
					Console.ForegroundColor = ConsoleColor.Green;
				}
				Console.WriteLine("{0}", entry.Key);
				Console.ResetColor();
			}
		}

		public void Show()
		{
			var lastMenuItem = _menuItems.Count - 1;

			bool menu = true;
			while (menu)
			{
				Print();
				switch (Console.ReadKey(true).Key)
				{
					case ConsoleKey.UpArrow:
						if (0 < _currentMenuItem)
						{
							_currentMenuItem -= 1;
						}
						else
						{
							_currentMenuItem = lastMenuItem;
						}
						break;

					case ConsoleKey.DownArrow:
						if (_currentMenuItem < lastMenuItem)
						{
							_currentMenuItem += 1;
						}
						else
						{
							_currentMenuItem = 0;
						}
						break;

					case ConsoleKey.Enter:
						_menuItems.ElementAt(_currentMenuItem).Value();
						break;

					case ConsoleKey.Escape:
						menu = false;
						Console.Clear();
                        break;
				}
			}
		}
	}
}
