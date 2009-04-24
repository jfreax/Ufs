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

#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "../util.hpp"


namespace gui
{


class CWidget
{
	public:
		CWidget ( );

		virtual void Render ( void ) = 0;
		virtual bool Update ( void );
		void NoUpdate ( bool ison );
		
		virtual bool Call ( void ) = 0;

		virtual bool onLeftClick ( void ) = 0;
		virtual bool onHoverMouse ( void ) = 0;
		virtual bool onUnHoverMouse ( void ) = 0;


		void SetMotherWin ( class CWindow* win );

		void SetPosition ( sf::Vector2f position );
		sf::Vector2f GetPosition ( );
		
		void SetSize ( sf::Vector2f size );

		void SetName ( std::string name );
		void SetFontSize ( int size );

		void SetBackground ( sf::Sprite background );
		sf::Sprite* GetBackground ( void );
		void SetBackground ( sf::Image* background );
		void SetBackgroundColor ( sf::Color color );
		sf::Color GetBackgroundColor ( void );

		class CWindow* GetMotherWin ( void );
		sf::Rect< float > GetDimensionInScreen ( void );

		sf::Clock lastClickTime;
		
	protected:
		virtual void Calc ( void );
	
		unsigned int id_;
		class CWindow* motherWin_;
		bool noUpdate_; // kein "update()" mehr machen! Vorsicht! Aufjedenfall wieder zurücksetzen...

		bool isMouseHere;
		bool wasMouseHere;
		
		
		std::string name_;
		sf::String text_;

		sf::Vector2f position_;
		sf::Vector2f fakePosition_;
		sf::Vector2f curSize_;

		sf::Sprite background_;
		sf::Color backgroundColor_;
		sf::Shape form_;

		unsigned int border_;
		sf::Color borderColor_;

};


} // namespace gui

#endif // WIDGET_HPP

