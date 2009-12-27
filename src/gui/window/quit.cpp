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

#include "quit.hpp"
#include "../../settings/settings.hpp"
#include "../button/quit.hpp"
#include "../other/image.hpp"
#include "../other/label.hpp"


namespace gui
{


CQuitWindow::CQuitWindow()
{
	int outline = 3;
	
	/* No titlebar */
	this->SetTitlebar ( 0 );
	
	/* Temp. vars */
	int width = settings::GetWidth();
	int height = settings::GetHeight();
	
	/* Change window properties */
	this->SetMoveAble ( false );
	this->SetCloseAble ( false );
	this->SetResizeAble ( false );
	this->SetLayout ( HORIZONTAL, 5 );
	
	/* Add widgets */
	this->AddWidget ( new CImage ( "icons/dialog-close.png" ) );
	this->AddWidget ( new CLabel ( "Wirklich beenden?" ) ); /* TODO i18n */
	this->AddWidget ( new CQuitButton, true );
	this->AddWidget ( new CCancelButton )->MovePosition ( HORIZONTAL, RIGHT );
	
	/* Set size and position (depend on buttons) */
	this->AdjustSize();
	this->SetPosition ( CENTER, CENTER );
	
	this->SetShow ( false );
}

	
} /* namespace gui */
