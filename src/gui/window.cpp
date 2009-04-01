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

#include "window.hpp"

#include "../game.hpp"

namespace gui
{

CWindow::CWindow( CTheme *theme, sf::Vector2f position_, sf::Vector2f size_ )
{
  static unsigned int id = 0;
  ++id;

  minSize = theme->winMinSize;
  maxSize = theme->winMaxSize;

  background = theme->winBackground;
  backgroundColor = theme->winBackgroundColor;

  border = theme->winBorder;
  borderColor = theme->winBorderColor;

  titlebar = theme->winTitlebar;
  titlebarColor = theme->winTitlebarColor;

  moveAble = true;
  resizeAble = true;

  setPosition( position_ );
  setSize( size_ );

  this->update();
}


void CWindow::update( )
{
  sf::Vector2f titlebarPosition( position.x, position.y - titlebar );
  sf::Vector2f titlebarEndPosition( position.x + curSize.x, position.y );
  formTitlebar = sf::Shape::Rectangle( titlebarPosition, titlebarEndPosition, titlebarColor );

  // Hintergrundbild / -Shape

  if ( background.GetSize().x != 1.f ) {
    background.SetPosition( position );
    background.Resize( curSize );
  } else {
    formWin = sf::Shape::Rectangle( position, position + curSize, backgroundColor, border, borderColor );
  }

}


void CWindow::draw( void* game_ )
{
  CGame* game = ( CGame* )game_;

  if ( background.GetSize().x != 1.f )
    game->getApp()->Draw( background );
  else
    game->getApp()->Draw( formWin );

  game->getApp()->Draw( formTitlebar );
}


void CWindow::setSize( sf::Vector2f size_ )
{
  curSize = size_;

  if ( curSize.x < minSize.x )
    curSize.x = minSize.x;

  if ( curSize.y < minSize.y )
    curSize.y = minSize.y;

  if ( curSize.x > maxSize.x )
    curSize.x = maxSize.x;

  if ( curSize.y > maxSize.y )
    curSize.y = maxSize.y;

  this->update();
}


void CWindow::setPosition( sf::Vector2f position_ )
{
  position = position_;

  if ( position.x + curSize.x < 0 )
    position.x = - curSize.x;

  if ( position.y - titlebar  + curSize.y < 0 )
    position.y = titlebar + 1 - curSize.x ;

  this->update();
}


sf::Vector2f CWindow::getPosition()
{
  return position;
}


sf::Rect<float> CWindow::getWindowDimension()
{
  return sf::Rect<float> ( position.x, position.y, position.x + curSize.x, position.y + curSize.y );
}


sf::Rect<float> CWindow::getTitlebarDimension()
{
  sf::Vector2f titlePos( position.x, position.y - titlebar );
  sf::Vector2f titleEndPos( position.x + curSize.x, position.y );
  return sf::Rect<float> ( titlePos.x , titlePos.y, titleEndPos.x, titleEndPos.y );
}


void CWindow::setMoveWindow( bool ison )
{
  moveWindow = ison;
}


bool CWindow::getMoveWindow( )
{
  return moveWindow;
}


} // namespace gui
