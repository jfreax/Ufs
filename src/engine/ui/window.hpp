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

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>


namespace gui
{


class CWindow
{
	public:
		CWindow();
		const unsigned int GetId() const;
		void NoUpdate ( bool ison );

		void Update();
		bool Render();

		bool Close();
		class CWidget* AddWidget ( class CWidget* widget );

		void calcBackground();
		void SetBackgroundImage ( sf::Image* img );
		
		void SetName ( std::string str );
		
		void Rotate ( double angle );
		void SetAngle ( double angle );
		double GetAngle();
		
		std::vector< gui::CWidget* >* GetWidgetList();

		void SetSize ( sf::Vector2f size, bool force = false );
		void SetSizeInPercent ( sf::Vector2f sizePercent );
		sf::Vector2f GetSize() const;

		void SetPosition ( sf::Vector2f position );
		sf::Vector2f GetPosition() const;
		
		int GetTitlebarHeight();
		int GetTitlebarLength();

		sf::Rect<float> GetWindowDimension() const;
		sf::Rect<float> GetTitlebarDimension() const ;
		sf::Rect<float> GetResizeArea() const;

		void SetTitlebar ( unsigned int titlebar );
		void SetNoRoundTitlebar ( bool ison );

		void SetMoveWindow ( bool ison );
		bool GetMoveWindow () const;
		
		void SetMoveAble ( bool ison );
		bool GetMoveAble ();
		
		void SetCloseAble ( bool ison );
		bool GetCloseAble ();

	protected:
		unsigned int id_;
		bool noUpdate_;
		
		sf::String name_;

		std::vector< gui::CWidget* > widgetList_;

		sf::Shape* formWin_;
		sf::Shape* formWinBorder_;
		sf::Shape* formTitlebar_;
		bool noRoundTitlebar_;

		bool moveWindow_;

		double angle_;

		sf::Vector2f position_;

		sf::Vector2f curSize_;
		sf::Vector2f minSize_;
		sf::Vector2f maxSize_;

		sf::Image* backgroundImage_;
		sf::Image* titlebarImage_;
		
		sf::Sprite background_;
		sf::Color backgroundColor_;

		unsigned int border_;
		sf::Color borderColor_;
		float formRound_;
		
		unsigned int titlebar_;
		int titlebarLength_;
		sf::Color titlebarColor_;

		bool closeAble_;
		bool moveAble_;
		bool resizeAble_;
};

} // namespace gui

#endif // WINDOW_HPP
