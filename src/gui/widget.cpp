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

#include "../game.hpp"

#include "window.hpp"
#include "widget.hpp"

namespace gui
{


CWidget::CWidget ( CWindow* motherWin_, sf::Vector2f position_, sf::Vector2f size_ )
{
  static unsigned int globalId = 0;
  id = ++globalId;

  CTheme* theme = getGameClass()->getGuiManager()->getTheme();

  motherWin = motherWin_;
  motherWin->addWidget ( this );

//   background.SetImage ( theme->button.background );
  backgroundColor = theme->button.backgroundColor;

  this->setPosition ( position_ );
  this->setSize ( size_ );
}


void CWidget::setPosition ( sf::Vector2f position_ )
{
  position = position_;

  if ( position.x < 0 ) {
    position.x += motherWin->getSize().x;
  } else while ( position.x >= motherWin->getSize().x ) {
      position.x -= motherWin->getSize().x;
  }

  if ( position.y < 0 ) {
    position.y += motherWin->getSize().y;
  } else while ( position.y >= motherWin->getSize().y ) {
      position.y -= motherWin->getSize().y;
  }
}


void CWidget::setSize ( sf::Vector2f size_ )
{
  size = size_;
}




} // namespace gui
