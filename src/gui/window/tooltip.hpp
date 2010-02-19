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

#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP

#include "../../engine/ui/window.hpp"
#include "../../engine/ui/widget.hpp"

namespace gui
{


class CTooltip : public CWindow
{
	public:
		CTooltip ( CWidget* motherWidget, std::string text );
		~CTooltip();
		
		void Update();
		
		void ChangeTransparency ( unsigned int alpha );
		
		void SetText ( std::string string );

	private:
		CWidget* motherWidget_;
		
		CWidget* label_;
		sf::Color textColor_;
};

} /* namespace gui */



#endif // TOOLTIP_HPP
