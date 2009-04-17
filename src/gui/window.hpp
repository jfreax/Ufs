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
#include "widget.hpp"


namespace gui
{


class CWindow
{
  public:
    CWindow ( class CTheme *theme, sf::Vector2f position_ = sf::Vector2f ( 0.f, 0.f ), sf::Vector2f size_ = sf::Vector2f ( 0.f, 0.f ) );
    const unsigned int GetId() const;

    void Update ( void );
    bool Render ( void );

    void AddWidget ( CWidget* widget_ );

    void SetSize ( sf::Vector2f size_ );
    void SetSizeInPercent ( sf::Vector2f sizePercent_ );
    sf::Vector2f GetSize ( void ) const;

    void SetPosition ( sf::Vector2f position_ );
    sf::Vector2f GetPosition ( void ) const;

    sf::Rect<float> GetWindowDimension ( void ) const;
    sf::Rect<float> GetTitlebarDimension ( void ) const ;
    sf::Rect<float> GetResizeArea ( void ) const;
    sf::Rect<float> GetIconCloseCoord ( void ) const;

    void SetTitlebar ( unsigned int titlebar_ );

    void SetMoveWindow ( bool ison );
    bool GetMoveWindow ( void ) const;

    void SetIconClose ( sf::Vector3f position_ );

  private:
    unsigned int id;

    std::vector< gui::CWidget* > widgetList;

    sf::Shape formWin;
    sf::Shape formTitlebar;

    bool moveWindow;

    sf::Vector2f position;

    sf::Vector2f curSize;
    sf::Vector2f minSize;
    sf::Vector2f maxSize;

    sf::Vector3f iconClose;
    sf::Sprite iconCloseImg;

    sf::Image bg_img;
    sf::Sprite background;
    sf::Color backgroundColor;

    unsigned int border;
    sf::Color borderColor;

    unsigned int titlebar;
    sf::Color titlebarColor;

    bool closeAble;
    bool moveAble;
    bool resizeAble;
};

} // namespace gui

#endif // WINDOW_HPP
