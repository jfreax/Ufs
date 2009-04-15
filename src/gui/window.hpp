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
    CWindow ( class CTheme *theme, sf::Vector2f position_ = sf::Vector2f ( 0.f, 0.f ), sf::Vector2f size_ = sf::Vector2f ( 0.f, 0.f ) );

    void update( );
    void draw ( );

    const unsigned int getId();

    void setSize ( sf::Vector2f size_ );
    void setSizeInPercent ( sf::Vector2f sizePercent_ );
    void setPosition ( sf::Vector2f position_ );
    sf::Vector2f getPosition();

    sf::Rect<float> getWindowDimension();
    sf::Rect<float> getTitlebarDimension();
    sf::Rect<float> getResizeArea();

    void setTitlebar ( unsigned int titlebar_ );

    void setMoveWindow ( bool ison );
    bool getMoveWindow( );

  private:
    unsigned int id;

    sf::Shape formWin;
    sf::Shape formTitlebar;

    bool moveWindow;

    sf::Vector2f position;

    sf::Vector2f curSize;
    sf::Vector2f minSize;
    sf::Vector2f maxSize;

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
