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
// #include "../action.hpp"

#include "button.hpp"

namespace gui
{


CButton::CButton ( )
{
	CTheme* theme = GetGameClass()->GetGuiManager()->GetTheme();

	this->NoUpdate ( true );
	{
		background_.SetImage ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/" + theme->button_.background ) );
		this->SetBackgroundColor ( theme->button_.backgroundColor );
		this->SetFontSize ( theme->button_.fontSize );
		
		this->SetSize ( background_.GetSize() );
	}
	this->NoUpdate ( false );
	
	WaitOnHoverMouse = 0.0f;
}


void CButton::Render ( void )
{
	sf::RenderWindow* app = GetGameClass()->GetApp();
	
	this->Calc();

	if ( background_.GetSize().x != 1.f )
	{
		app->Draw ( background_ );
	}
	else
	{

	}

	app->Draw ( text_ );
}


bool CButton::onHoverMouse ( void )
{
	isMouseHere = wasMouseHere = true;
	background_.SetColor( sf::Color ( 200, 200, 200 ) );
}


bool CButton::onUnHoverMouse ( void )
{
std::cout << "hier " << std::endl;
	background_.SetColor( sf::Color ( 255, 255, 255 ) );
}



} // namespace gui
