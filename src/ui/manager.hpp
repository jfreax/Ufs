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

#ifndef MANAGER_HPP
#define MANAGER_HPP

// #include <SFML/Graphics.hpp>

#include "theme.hpp"
#include "window.hpp"
#include "../parser.hpp"

namespace gui
{


class CManager
{

	public:
		CManager ( void );

		bool Render ( void );
		bool MouseClick ( const int x, const int y, const sf::Mouse::Button button );
		bool MouseHover ( const int x, const int y );

		CWindow* NewWindow ( sf::Vector2f position = sf::Vector2f ( 0.f, 0.f ), sf::Vector2f size = sf::Vector2f ( 0.f, 0.f ) );
		bool CloseWindow ( CWindow* window = NULL ); // NULL -> schlieﬂt das letzte Fenster
		bool BringToFront ( std::vector< CWindow* >::iterator iter );

		CTheme* GetTheme ( void );
		CWindow* GetActiveWindow ( void );

	private:
		CTheme theme_;
		std::vector< CWindow* > windowList_;

		sf::Vector2f previousMousePos_;
		MOUSESCOPE previousMouseScope_;
};

} // namespace gui

#endif // MANAGER_HPP
