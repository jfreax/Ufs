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
#include "quit.hpp"

namespace gui
{


CQuitButton::CQuitButton ( bool noLocale  )
{
	if ( noLocale ) {
		this->SetText ( "Quit" );
		this->AdjustSize ( 10 );
	} else {
		this->SetText ( SYS, "QUIT" );
	}
}


bool CQuitButton::onLeftClick()
{
	GetGameClass()->Stop();
	
	return true;
}


/* --------------------- */	


CCancelButton::CCancelButton ( bool noLocale )
{
	if ( noLocale ) {
		this->SetText ( "Ignore" );
		this->AdjustSize ( 10 );
	} else {
		this->SetText ( SYS, "CANCEL" );
	}
}


bool CCancelButton::onLeftClick()
{
	GetGameClass()->SetGameType ( SINGLEPLAYER ); /* TODO right gametype! */
	motherWin_->SetShow ( false );
	
	return true;
}


} /* namespace gui */
