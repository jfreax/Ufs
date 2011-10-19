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
#include "label.hpp"

namespace gui
{


CLabel::CLabel ( std::string text, unsigned int fontSize )
{
	this->SetTextSize ( fontSize );
	this->SetText ( text );
	this->AdjustSize();
}


CLabel::CLabel ( HEADERNAME header, std::string text, unsigned int fontSize )
{
	this->SetTextSize ( fontSize );
	this->SetText ( l ( header, text ) );
	this->AdjustSize();
}



void CLabel::Render()
{
	sf::RenderWindow* app = GetGameClass()->GetApp();
	
	if ( this->GetShow() ) {
		this->Calc();
		app->Draw ( text_ );
	}
}

} /* namespace gui */
