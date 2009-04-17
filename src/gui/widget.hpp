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

#include <SFML/Graphics.hpp>

namespace gui
{


class CWidget
{
  public:
    CWidget ( class CWindow* motherWin_, sf::Vector2f position_, sf::Vector2f size_ );

    bool noUpdate; // kein "update()" mehr machen! Vorsicht! Aufjedenfall wieder zurücksetzen...
    virtual bool Update ( void );
    virtual void Render ( void ) const = 0;

    void SetPosition ( sf::Vector2f position_ );
    void SetSize ( sf::Vector2f size_ );

    void SetName ( std::string name_ );
    void SetFontSize ( int size_ );

    void SetBackgroundColor ( sf::Color color_ );

  protected:
    unsigned int id;
    class CWindow* motherWin;

    std::string name;
    sf::String text;

    sf::Vector2f position;
    sf::Vector2f curSize;

    sf::Sprite background;
    sf::Color backgroundColor;
    sf::Shape form;

    unsigned int border;
    sf::Color borderColor;

};


} // namespace gui

#endif // WIDGET_HPP
