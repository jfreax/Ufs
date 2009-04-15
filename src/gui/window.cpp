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

// #include "widget.hpp"
#include "manager.hpp"

#include "../game.hpp"
#include "../resource.hpp"
#include "../settings.hpp"

#include "window.hpp"

namespace gui
{

CWindow::CWindow ( CTheme *theme, sf::Vector2f position_, sf::Vector2f size_ )
{
  static unsigned int globalId = 0;
  id = ++globalId;

  minSize = theme->window.minSize;
  maxSize = theme->window.maxSize;

  background.SetImage ( *getGameClass()->getImgResource()->get( "/themes/" + settings::getTheme() + "/" + theme->window.background ) );
  backgroundColor = theme->window.backgroundColor;

  border = theme->window.border;
  borderColor = theme->window.borderColor;

  titlebar = theme->window.titlebar;
  titlebarColor = theme->window.titlebarColor;

  closeAble = true;
  moveAble = true;
  resizeAble = true;

  this->setPosition ( position_ );
  this->setSize ( size_ );

  this->update();
}


const unsigned int CWindow::getId()
{
  return id;
}


void CWindow::update( )
{
  sf::Vector2f titlebarPosition ( position.x, position.y - titlebar );
  sf::Vector2f titlebarEndPosition ( position.x + curSize.x, position.y );
  formTitlebar = sf::Shape::Rectangle ( titlebarPosition, titlebarEndPosition, titlebarColor );

  // Hintergrundbild / -Shape

  if ( background.GetSize().x != 1.f ) {
    background.SetPosition ( position );
    background.Resize ( curSize );
  } else {
    formWin = sf::Shape::Rectangle ( position, position + curSize, backgroundColor, border, borderColor );
  }

}


void CWindow::draw ( )
{
  sf::RenderWindow* app = getGameClass()->getApp();

  app->Draw ( formTitlebar );

  if ( background.GetSize().x && background.GetSize().x != 1.f ) {
    app->Draw ( background );
  } else {
    app->Draw ( formWin );
  }

  for ( std::vector<gui::CWidget*>::size_type i = widgetList.size(); i; --i ) {
    widgetList[i-1]->draw();
  }
}


void CWindow::addWidget ( CWidget* widget_ )
{
  widgetList.push_back ( widget_ );
}


void CWindow::setSize ( sf::Vector2f size_ )
{
  curSize = size_;

  if ( curSize.x < minSize.x ) {
    curSize.x = minSize.x;
  }
  if ( curSize.y < minSize.y ) {
    curSize.y = minSize.y;
  }
  if ( curSize.x > maxSize.x ) {
    curSize.x = maxSize.x;
  }
  if ( curSize.y > maxSize.y ) {
    curSize.y = maxSize.y;
  }

  this->update();
}


void CWindow::setSizeInPercent ( sf::Vector2f sizePercent_ )
{
  this->setSize ( sf::Vector2f ( settings::getWidth() * sizePercent_.x * 0.01, settings::getHeight() * sizePercent_.y * 0.01 ) );
}


sf::Vector2f CWindow::getSize()
{
  return curSize;
}


void CWindow::setPosition ( sf::Vector2f position_ )
{
  position = position_;

  if ( position.x + curSize.x < 0 )
    position.x = - curSize.x + 10;

  if ( position.x > settings::getWidth() - 10 )
    position.x = settings::getWidth() - 10;

  if ( position.y - titlebar < 0 )
    position.y = titlebar + 1;

  if ( position.y > settings::getHeight() )
    position.y = settings::getHeight();

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
  sf::Vector2f titlePos ( position.x, position.y - titlebar );
  sf::Vector2f titleEndPos ( position.x + curSize.x, position.y );
  return sf::Rect<float> ( titlePos.x , titlePos.y, titleEndPos.x, titleEndPos.y );
}


sf::Rect<float> CWindow::getResizeArea()
{


}


void CWindow::setTitlebar ( unsigned int titlebar_ )
{
  titlebar = titlebar_;
  this->update();
}


void CWindow::setMoveWindow ( bool ison )
{
  moveWindow = ison;
}


bool CWindow::getMoveWindow( )
{
  return moveWindow;
}


} // namespace gui
