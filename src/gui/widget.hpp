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
	private:
		struct Arguments_
		{
			bool ( *function ) ( CWidget*, util::DataHolder& );
			util::DataHolder args;
			
			float wait;
			sf::Clock timer;
			
			bool isCalled;
			bool delNow;
			bool shouldDelete;
		};
	

	public:
		CWidget ( class CWindow* motherWin, sf::Vector2f position, sf::Vector2f size );

		virtual void Render ( void ) = 0;
		virtual bool Update ( void );
		virtual void Calc ( void );
		void NoUpdate ( bool ison );


		bool Call ( void );
		bool Mouse ( MOUSE::TYPES type );
		
		util::DataHolder* AddCall ( bool ( *mouseClick ) ( CWidget*, util::DataHolder& ), float wait = 0.f );
		util::DataHolder* AddMouseEvent ( bool ( *mouseClick ) ( CWidget*, util::DataHolder& ), MOUSE::TYPES type = MOUSE::LEFT, float wait = 0.f );

		void SetPosition ( sf::Vector2f position );
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

	protected:
		unsigned int id_;
		class CWindow* motherWin_;

		bool noUpdate_; // kein "update()" mehr machen! Vorsicht! Aufjedenfall wieder zurücksetzen...

		std::vector< Arguments_* > mouseHover_;
		std::vector< Arguments_* > mouseUnHover_;
		std::vector< Arguments_* > mouseLClick_;
		std::vector< Arguments_* > mouseRClick_;
		std::vector< Arguments_* > mouseMClick_;

		std::list< Arguments_* > callThis_;
		
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
