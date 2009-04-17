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

#include "button.hpp"

namespace gui
{


CButton::CButton ( class CWindow* motherWin_, sf::Vector2f position_, sf::Vector2f size_ ) : CWidget ( motherWin_, position_, size_ )
{
  CTheme* theme = GetGameClass()->GetGuiManager()->GetTheme();

  this->noUpdate = true;
  {
    background.SetImage ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/" + theme->button.background ) );
    this->SetBackgroundColor ( theme->button.backgroundColor );
    this->SetFontSize ( theme->button.fontSize );
  }

  this->noUpdate = false;
  this->Update();
}


void CButton::Render ( void ) const
{
  sf::RenderWindow* app = GetGameClass()->GetApp();

  if ( background.GetSize().x != 1.f )
  {
    app->Draw ( background );
  }
  else
  {

  }

  app->Draw ( text );
}



} // namespace gui
