/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef UTIL_HPP
#define UTIL_HPP

#include <typeinfo>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include "input.hpp"


enum TYPES
{
	INTEGER,
	FLOAT,
	STRING,
	COLOR,
	IMAGE,
	VECTOR_INTEGER,
	VECTOR_FLOAT,
	VECTOR3_FLOAT
};


enum MOUSESCOPE
{
	NONE,
	TITLE,
	RESIZE,
	WINDOW,
	BUTTON
};


namespace util
{


template<typename To, typename From>
To lCast ( From a, To def = To() )
{
	To res;
	std::stringstream str;

	if ( ! ( str << a && str >> res ) )
	{
		return def;
	}
	else
	{
		return res;
	}
}


class DataHolder
{
	public:
		DataHolder ( void );

		int frame;
		
		bool b;

		int x;
		int y;
		int z;

		double d;
		float f;

		std::string string_one;
		std::string two;

		sf::Color color_one;
		sf::Color color_two;

		sf::Sprite* sprite_one;
		sf::Sprite* sprite_two;
};


bool isFile ( std::string filename );
void deleteChar ( std::string &stringToReplace, char delThis = ' ' );
void lowercase ( std::string& str );

std::string colorToString ( sf::Color color );
MOUSE::TYPES sfMouseToMouse ( sf::Mouse::Button button );
}

#endif // UTIL_HPP
