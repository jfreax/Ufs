/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../../game.hpp"

#include "../other/terminal.hpp"
#include "terminal.hpp"


namespace gui
{
	
	
CTerminalWindow::CTerminalWindow()
{
	/* No titlebar */
	this->SetTitlebar ( 0 );
	
	/* Set properies */
	this->SetMoveAble ( false );
	this->SetCloseAble ( false );
	
	/* Add buttons */
	terminal_ = dynamic_cast < CTextArea* > ( this->AddWidget ( new CTerminal ) );
	
	/* Set size (depend on buttons) */
	this->AdjustSize();
	this->SetFreeToSet ( true );
	this->SetPosition ( CENTER, TOP );
	this->MovePosition( VERTICAL, - this->GetSize().y );

	/* Register this window as special window */
	GetGameClass()->GetGuiManager()->RegisterTerminal ( this );
	
	toggle_ = slideIn_ = false;
	slideSpeed_ = 1000;
}


void CTerminalWindow::Call()
{
	if ( !slideIn_ && toggle_ && this->GetPosition().y < 0 ) { /* slide out */
		this->MovePosition( VERTICAL, slideSpeed_ * GetGameClass()->GetApp()->GetFrameTime() );
		
		if ( this->GetPosition().y >= 0 ) {
			this->SetPosition( sf::Vector2f ( -1, 0 ) );
			toggle_ = false;
			GetGameClass()->GetGuiManager()->FocusTextArea ( terminal_ );
		}
		
	} else if ( slideIn_ || toggle_  && this->GetPosition().y >= 0 ) {
		this->MovePosition( VERTICAL, -slideSpeed_ * GetGameClass()->GetApp()->GetFrameTime() );
		slideIn_ = true;
		
		if ( this->GetPosition().y <= -this->GetSize().y ) {
			toggle_ = slideIn_ = false;
			GetGameClass()->GetGuiManager()->FocusTextArea ( NULL );			
		}
	}
}


void CTerminalWindow::ToggleShow()
{
	toggle_ = true;
	
	GetGameClass()->GetGuiManager()->BringToFront( this );
}




} /* namespace gui */