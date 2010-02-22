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

#include "../../util/util.hpp"
#include "../../settings/locale.hpp"
#include "../../gfx/graphic.hpp"
#include "util.hpp"


namespace gui
{
	
enum WIDGET_TYPE {
	widget,
	button,
	planetbutton
};
	
class CWidget
{
	public:
		CWidget();
		~CWidget();
		
		unsigned int GetId() { return id_; }
		WIDGET_TYPE GetType() const;

		virtual void Render();
		virtual bool Update ( bool doIt = false );
	
		virtual bool Call() {};

		virtual bool onLeftClick() {};
		virtual bool onHoverMouse();
		virtual bool onUnHoverMouse();
		
		virtual void PressedKey ( sf::Key::Code code ) {};

		void SetMotherWin ( class CWindow* win );

		void SetPosition ( sf::Vector2f position );
		void MovePosition ( LAYOUT direction, unsigned int distance );
		void MovePosition ( LAYOUT direction, POSITION to );
		
		sf::Vector2f GetPosition();
		sf::Vector2f GetPositionInScreen();
		sf::Rect<float> GetDimension();
		sf::Rect< float > GetDimensionInScreen();
		
		void SetSize ( sf::Vector2f size );
		void AdjustSize ( unsigned int border = 0 );

		void SetName ( std::string name );
		void SetFontSize ( int size );

		void SetDrawBackground ( bool ison );

		void SetBackground ( sf::Sprite* background );
		void SetBackground ( sf::Image* background );
		void SetBackground ( CGraphic* background );
		
		CGraphic* GetBackground();
		
		void SetBackgroundColor ( sf::Color color );
		sf::Color GetBackgroundColor();

		class CWindow* GetMotherWin();
		
		
		void SetShow ( bool ison = true );
		bool GetShow();
		
		std::string GetName();
		
		void SetText ( std::string text );
		void SetText ( HEADERNAME header, std::string text );
		sf::String* GetText();
		
		void SetTextPosition ( sf::Vector2f pos );
		sf::Vector2f GetTextPosition();
		
		void SetTextSize ( int size );
		void SetFont ( std::string fontname );

	protected:
		virtual void Calc();
		void AdjustTextPosition();
		
		CWindow* ActivateTooltip ( std::string text = "" );
		CWindow* ActivateTooltip ( HEADERNAME header, std::string var );
		
	public:
		sf::Clock lastClickTime;
		
	protected:
		/* --- Data variables --- */
		unsigned int id_;
		class CWindow* motherWin_;
		
		WIDGET_TYPE type_;
		
		bool update_;

		bool isMouseHere_;
		bool wasMouseHere_;
		
		bool hasTooltip_;
		bool unHover_;
		sf::Clock lastMouseUnHover_;
		int showTooltip_;
		class CTooltip* toolTip_;
		
		double angle_;
		
		bool show_;
		
		std::string name_;
		sf::String text_;
		sf::Vector2f textPos_;

		sf::Vector2f position_;
		sf::Vector2f fakePosition_;
		sf::Vector2f curSize_;

		bool drawBackground_;
		CGraphic background_;
// 		sf::Sprite background_;

		sf::Color backgroundColor_;
// 		sf::Shape form_;

		unsigned int border_;
		sf::Color borderColor_;
};


} // namespace gui

#endif // WIDGET_HPP

