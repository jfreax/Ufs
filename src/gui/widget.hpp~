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
#include "window.hpp"

namespace gui
{


class CWidget
{
  public:
//     CWidget ( gui::CWindow* motherWin_, sf::Vector2f position_ = sf::Vector2f ( 0.f, 0.f ), sf::Vector2f size_ = sf::Vector2f ( 0.f, 0.f ) );

    virtual bool draw();

    void setPosition ( sf::Vector2f position_ );
    void setSize ( sf::Vector2f size_ );

  private:
    unsigned int id;

    sf::Vector2f position;
    sf::Vector2f size;

};


} // namespace gui

#endif // WIDGET_HPP
