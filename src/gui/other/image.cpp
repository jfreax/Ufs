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
#include "../../settings/settings.hpp"
#include "image.hpp"


namespace gui
{

	
CImage::CImage ( std::string filename )
{
	background_.SetImage ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/" + filename ) );
	this->SetSize ( background_.GetSize() );
}


void CImage::Render()
{
	if ( !show_ )
		return;
	
	this->Calc();
	
	sf::RenderWindow* app = GetGameClass()->GetApp();
	if ( drawBackground_ && background_.GetSize().x != 1.f )
		app->Draw ( background_ );
}



} /* namespace gui */
