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

#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>
#include <SFML/Graphics.hpp>

#include "parser.hpp"


namespace MOUSE
{

enum TYPES {
	LEFT,
	RIGHT,
	MIDDLE,
	HOVER,
	UNHOVER
};

} // namespace MOUSE

struct KeyHolder {
	sf::Key::Code key;

	bool strg;
	bool alt;
	bool shift;
};


class CInput
{
	public:
		CInput ( void );
		bool LoadKeys ( std::string file );

		bool TestPressedKeys ( std::string keyName_ );
		bool Events ( void );

	private:
		std::map<std::string, sf::Key::Code> availableKeys;
		std::map<std::string, KeyHolder> globalKeys;
		sf::Event event;

		Parser keyfile;
};

#endif // INPUT_HPP
