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

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>

#include "widget.hpp"


namespace gui
{


class CButton : public CWidget
{
	public:
		CButton ( class CWindow* motherWin, sf::Vector2f position = sf::Vector2f ( 0.f, 0.f ), sf::Vector2f size = sf::Vector2f ( 0.f, 0.f ) );
		void Render ( void );


	private:
};



} // namespace gui

#endif // BUTTON_HPP
