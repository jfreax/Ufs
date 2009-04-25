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

	background_.SetImage ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/" + theme->button_.background ) );
	this->SetBackgroundColor ( theme->button_.backgroundColor );
	this->SetFontSize ( theme->button_.fontSize );
	
	this->SetSize ( background_.GetSize() );
	
	WaitOnHoverMouse = 0.0f;
}


void CButton::Render ( void )
{
	/* Wenn es nicht gezeichnet werden soll, dann ist hier schon Schluss */
	if ( !show_ )
	{
		return;
	}
	
	/* Berechnung durchführen */
	this->Calc();
	
	
// 	std::cout <<   position_.y << " + "<< curSize_.y << " und " << motherWin_->GetSize().y << std::endl;
// 	/* Nur zeichnen wenn es innerhalb des sichtbaren Bereichs des Fensters liegt */
// 	if ( position_.y + curSize_.y -1 > motherWin_->GetSize().y )
// 	{
// 		return;
// 	}

	/* Hintergrund zeichnen */
	sf::RenderWindow* app = GetGameClass()->GetApp();
	if ( drawBackground_ && background_.GetSize().x != 1.f )
	{
		app->Draw ( background_ );
	}
	else
	{

	}

	/* Ggf. Text zeichnen */
	app->Draw ( text_ );
}


bool CButton::onHoverMouse ( void )
{
	isMouseHere_ = wasMouseHere_ = true;
	background_.SetColor( sf::Color ( 200, 200, 200 ) );
}


bool CButton::onUnHoverMouse ( void )
{
std::cout << "hier " << std::endl;
	background_.SetColor( sf::Color ( 255, 255, 255 ) );
}



} // namespace gui
