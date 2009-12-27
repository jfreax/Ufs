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
#include "header.hpp"

namespace gui
{
	
	
CHeaderMainButton::CHeaderMainButton()
{
	this->SetSize ( sf::Vector2f ( 32, 32 ) );
	this->SetBackground ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/buttons/header_home.png" ) );
	
	this->ActivateTooltip ( TOOLTIP, "HOME" );
	
}


/* ------------------------- */


CHeaderResearchButton::CHeaderResearchButton()
{
	this->SetSize ( sf::Vector2f ( 32, 32 ) );
	this->SetBackground ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/buttons/header_research.png" ) );
	
	this->ActivateTooltip( TOOLTIP, "RESEARCH" );
	
}


/* ------------------------- */


CHeaderPlanetsButton::CHeaderPlanetsButton()
{
	this->SetSize ( sf::Vector2f ( 32, 32 ) );
	this->SetBackground ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/buttons/header_planets.png" ) );
	
	this->ActivateTooltip ( TOOLTIP, "PLANET" );
}


/* ------------------------- */


CHeaderEmpireButton::CHeaderEmpireButton()
{
	this->SetSize ( sf::Vector2f ( 32, 32 ) );
	this->SetBackground ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/buttons/header_empire.png" ) );
	
	this->ActivateTooltip ( TOOLTIP, "EMPIRE" );
	
}


/* ------------------------- */



CHeaderSpyButton::CHeaderSpyButton()
{
	this->SetSize ( sf::Vector2f ( 32, 32 ) );
	this->SetBackground ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/buttons/header_spy.png" ) );
	
	this->ActivateTooltip ( TOOLTIP, "SPY" );
	
}


/* ------------------------- */


CHeaderStatsButton::CHeaderStatsButton()
{
	this->SetSize ( sf::Vector2f ( 32, 32 ) );
	this->SetBackground ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/buttons/header_stats.png" ) );
	
	this->ActivateTooltip ( TOOLTIP, "STATS" );
	
}


/* ------------------------- */


CHeaderQuitButton::CHeaderQuitButton()
{
	this->SetSize ( sf::Vector2f ( 32, 32 ) );
	this->SetBackground ( *GetGameClass()->GetImgResource()->Get ( "/themes/" + settings::GetTheme() + "/buttons/header_quit.png" ) );
	
	this->ActivateTooltip ( TOOLTIP, "QUIT" );
	
}


bool CHeaderQuitButton::onLeftClick()
{
	GetGameClass()->SetGameType ( QUIT );
	return true;
}


} /* namespace gui */
