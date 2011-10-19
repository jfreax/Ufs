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

#include "../../engine/ui/button.hpp"

#ifndef MAIN_MENU_H
#define MAIN_MENU_H


namespace gui
{


class CSingleplayerButton : public CButton
{
	public:
		CSingleplayerButton ( void );

		bool Call ( void );
		
		bool onHoverMouse ( void );
		bool onUnHoverMouse ( void );
		
	private:
		bool hoverEffect_;
		bool unHoverEffect_;
		
		float diff_;
		sf::Vector2f endTextPos_;
		
		sf::Shape testShape;
	
};


} /* namespace gui */


#endif // MAIN_MENU_H
