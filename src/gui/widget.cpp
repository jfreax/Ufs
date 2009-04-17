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

  CTheme* theme = GetGameClass()->GetGuiManager()->GetTheme();

  motherWin = motherWin_;
  motherWin->AddWidget ( this );

  this->noUpdate = true;
  {
    this->SetPosition ( position_ );
    this->SetSize ( size_ );
  }

  this->noUpdate = false;
  this->Update();
}


bool CWidget::Update ( void )
{
  if ( noUpdate )
  {
    return false;
  }

  if ( background.GetSize().x != 1.f )
  {
    background.SetPosition ( position + motherWin->GetPosition() );
    background.Resize ( curSize );
  }
  else
  {
    form = sf::Shape::Rectangle ( position, position + curSize, backgroundColor, border, borderColor );
  }

  sf::Vector2f textPos ( ( curSize.x - text.GetRect().GetWidth() ) * 0.5f, ( curSize.y - text.GetRect().GetHeight() ) * 0.5f );

  text.SetPosition ( position + motherWin->GetPosition() + textPos );

  return true;
}



void CWidget::SetPosition ( sf::Vector2f position_ )
{
  position = position_;

  if ( position.x < 0 )
  {
    position.x += motherWin->GetSize().x;
  }
  else if ( position.x >= motherWin->GetSize().x )
  {
    position.x -= motherWin->GetSize().x;
  }

  if ( position.y < 0 )
  {
    position.y += motherWin->GetSize().y;
  }
  else if ( position.y >= motherWin->GetSize().y )
  {
    position.y -= motherWin->GetSize().y;
  }

  this->Update();
}


void CWidget::SetSize ( sf::Vector2f size_ )
{
  curSize = size_;
  this->Update();
}


void CWidget::SetName ( std::string name_ )
{
  name = name_;
  text.SetText ( name );

  this->Update();
}


void CWidget::SetFontSize ( int size_ )
{
  text.SetSize ( size_ );
}


void CWidget::SetBackgroundColor ( sf::Color color_ )
{
  backgroundColor = color_;
}



} // namespace gui
