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

#ifndef SYSTEMTOOLTIP_HPP
#define SYSTEMTOOLTIP_HPP


#include "../../engine/ui/window.hpp"
#include "../../engine/ui/widget.hpp"

#include "../../engine/sprite/sprite.hpp"

namespace gui
{
class CButton;

class CSystemTooltip : public CWindow
{
	public:
		CSystemTooltip ( CSystem* sys, std::string text );
		~CSystemTooltip();
		
		bool onUnHoverMouse();
		
		void Update();
		
		void AddPlanetButton ( sprite::CPlanet* planet );
		
		void Show ( const int x, const int y );
		
		void CalcBackground();
		void ChangeTransparency ( unsigned int alpha );
		
		void SetText ( std::string string );
		
	private:
		CSystem* system_;
		
		CWidget* label_;
		sf::Color textColor_;
		
		sf::Clock showClock_;
};


} /* namespace gui */

#endif // SYSTEMTOOLTIP_HPP
