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

#include <iostream>

#include "error.hpp"
#include "../other/image.hpp"
#include "../other/spacer.hpp"
#include "../button/quit.hpp"
#include "../../settings/settings.hpp"



namespace gui
{


CErrorWindow::CErrorWindow ( std::string text )
{
	/* No titlebar */
	this->SetTitlebar ( 0 );
	
	/* Change window properties */
	this->SetCloseAble ( false );
	this->SetResizeAble ( false );
	this->SetLayout ( VERTICAL, 5 );
	
	/* Add widgets */
	this->AddWidget ( new CLabel ( "An error occurred with the following message...", 18 ) );
	this->AddWidget ( new CImage ( settings::GetThemePath() + "icons/dialog-close.png" ) )->MovePosition ( HORIZONTAL, LEFT );
	
	this->SetLayout ( HORIZONTAL, 5 );
	label_ = dynamic_cast< CLabel* > ( this->AddWidget ( new CLabel ( text ) ) );
	
	this->AddWidget ( new CQuitButton ( true ), true );
	this->AddWidget ( new CCancelButton ( true ) )->MovePosition ( HORIZONTAL, RIGHT );
	
	/* Set size and position (depend on buttons) */
	this->AdjustSize();
	this->SetPosition ( CENTER, CENTER );
// this->Update();
	
// 	this->SetShow ( true );
}


void CErrorWindow::SetText ( std::string text )
{
	label_->SetText ( text );
	label_->AdjustSize();
	this->AdjustSize();
}


} /* namespace gui */