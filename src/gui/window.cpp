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

  CGame* game = GetGameClass();

  minSize = theme->window.minSize;
  maxSize = theme->window.maxSize;

  background.SetImage ( *game->GetImgResource()->Get( settings::GetThemePath() + theme->window.background ) );
  backgroundColor = theme->window.backgroundColor;

  iconCloseImg.SetImage( *game->GetImgResource()->Get( settings::GetThemePath() + "close.png" ) );
  this->SetIconClose ( theme->window.iconClose );

  border = theme->window.border;
  borderColor = theme->window.borderColor;

  titlebar = theme->window.titlebar;
  titlebarColor = theme->window.titlebarColor;

  closeAble = true;
  moveAble = true;
  resizeAble = true;

  this->SetPosition ( position_ );
  this->SetSize ( size_ );

  this->Update();
}


const unsigned int CWindow::GetId ( void ) const
{
  return id;
}


void CWindow::Update ( void )
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

  // Titelbar
  int iCx = iconClose.x < 0 ? iconClose.x + GetSize().x : iconClose.x;
  int iCy = iconClose.y - titlebar;
  iconCloseImg.SetPosition( iCx + GetPosition().x, iCy + GetPosition().y );

  // Inhalte ebenfalls aktualisieren
  for ( std::vector<gui::CWidget*>::size_type i = widgetList.size(); i; --i ) {
    widgetList.at( i-1 )->Update();
  }
}


bool CWindow::Render ( void )
{
  sf::RenderWindow* app = GetGameClass()->GetApp();

  // Titlebar + icons
  app->Draw ( formTitlebar );
  app->Draw ( iconCloseImg );



  if ( background.GetSize().x && background.GetSize().x != 1.f ) {
    app->Draw ( background );
  } else {
    app->Draw ( formWin );
  }

  for ( std::vector<gui::CWidget*>::size_type i = widgetList.size(); i; --i ) {
    widgetList[i-1]->Render();
  }
}


void CWindow::AddWidget ( CWidget* widget_ )
{
  widgetList.push_back ( widget_ );
}


void CWindow::SetSize ( sf::Vector2f size_ )
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

  this->Update();
}


void CWindow::SetSizeInPercent ( sf::Vector2f sizePercent_ )
{
  this->SetSize ( sf::Vector2f ( settings::GetWidth() * sizePercent_.x * 0.01, settings::GetHeight() * sizePercent_.y * 0.01 ) );
}


sf::Vector2f CWindow::GetSize ( void ) const
{
  return curSize;
}


void CWindow::SetPosition ( sf::Vector2f position_ )
{
  position = position_;

  if ( position.x + curSize.x < 0 )
    position.x = - curSize.x + 10;

  if ( position.x > settings::GetWidth() - 10 )
    position.x = settings::GetWidth() - 10;

  if ( position.y - titlebar < 0 )
    position.y = titlebar + 1;

  if ( position.y > settings::GetHeight() )
    position.y = settings::GetHeight();

  this->Update();
}


sf::Vector2f CWindow::GetPosition ( void ) const
{
  return position;
}


sf::Rect<float> CWindow::GetWindowDimension ( void ) const
{
  return sf::Rect<float> ( position.x, position.y, position.x + curSize.x, position.y + curSize.y );
}


sf::Rect<float> CWindow::GetTitlebarDimension ( void ) const
{
  sf::Vector2f titlePos ( position.x, position.y - titlebar );
  sf::Vector2f titleEndPos ( position.x + curSize.x, position.y );
  return sf::Rect<float> ( titlePos.x, titlePos.y, titleEndPos.x, titleEndPos.y );
}


sf::Rect<float> CWindow::GetResizeArea ( void ) const
{
  sf::Vector2f point = GetPosition() + GetSize() - sf::Vector2f ( 2, 2 );

  return sf::Rect<float> ( point.x, point.y, point.x + 3, point.y + 3 );
}


sf::Rect<float> CWindow::GetIconCloseCoord ( void ) const
{
  return sf::Rect<float> ( iconCloseImg.GetPosition().x, iconCloseImg.GetPosition().y,
    iconCloseImg.GetPosition().x + iconCloseImg.GetSize().x, iconCloseImg.GetPosition().y + iconCloseImg.GetSize().y );
}


void CWindow::SetTitlebar ( unsigned int titlebar_ )
{
  titlebar = titlebar_;
  this->Update();
}


void CWindow::SetMoveWindow ( bool ison )
{
  moveWindow = ison;
}


bool CWindow::GetMoveWindow ( void ) const
{
  return moveWindow;
}


void CWindow::SetIconClose ( sf::Vector3f position_ )
{
  iconClose = position_;
}


} // namespace gui
