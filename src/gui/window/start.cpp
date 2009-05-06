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

#include "start.hpp"
#include "../../settings.hpp"
#include "../../game.hpp"
#include "../../gui/button/main_menu.hpp"
#include "../../gui/button/titlebar.hpp"

namespace gui
{


CStartWindow::CStartWindow ( void )
{
	/* Keine Titelbar! */
	this->SetTitlebar ( 0 );
	
	/* Fenster kann man nicht bewegen */
	this->SetMoveAble ( false );
	
	/* Größe und Position */
	this->SetPosition( sf::Vector2f ( 0, 0 ) );
	this->SetSize ( sf::Vector2f ( settings::GetWidth(), settings::GetWidth() ) );
	
	/* Hintergrundbild */
	this->SetBackgroundImage ( GetGameClass()->GetImgResource()->Get ( "themes/default/images/main.png" ) );
	
	/* Buttons hinzufügen */
	this->AddWidget ( new gui::CSingleplayerButton );

}



} /* namespace gui */

