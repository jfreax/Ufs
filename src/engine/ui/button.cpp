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

#include "../../game.hpp"
#include "button.hpp"

namespace gui
{


CButton::CButton()
{
	CTheme* theme = GetGameClass()->GetGuiManager()->GetTheme();

	background_.SetImage ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/" + theme->button_.background ) );

	this->SetBackgroundColor ( theme->button_.backgroundColor );
	this->SetFontSize ( theme->button_.fontSize );
	this->SetSize ( background_.GetSize() );
	
	WaitOnHoverMouse = 0.0f;
}


void CButton::Render()
{
	if ( !show_ )
		return;
	
	this->Calc();	

	/* Draw background */
	sf::RenderWindow* app = GetGameClass()->GetApp();
	if ( drawBackground_ && background_.GetSize().x != 1.f )
		app->Draw ( background_ );
	
	/* If necessary, draw text */
	app->Draw ( text_ );
	
	/* Render things from widget-class */
	CWidget::Render();
}


bool CButton::onHoverMouse()
{
	CWidget::onHoverMouse();
	
	isMouseHere_ = wasMouseHere_ = true;
	background_.SetColor( sf::Color ( 200, 200, 200 ) );
}


bool CButton::onUnHoverMouse()
{
	CWidget::onUnHoverMouse();
	
	background_.SetColor( sf::Color ( 255, 255, 255 ) );
}



} // namespace gui
