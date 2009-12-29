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

#ifndef SELECT_WIDGET_HPP
#define SELECT_WIDGET_HPP

#include "../../engine/ui/widget.hpp"
#include "../../gui/other/label.hpp"

namespace gui
{
	
	
class CSelectWidget : public CWidget
{
	public:
		CSelectWidget();
		~CSelectWidget();
		
		void Initialize();
		void Render();
		
		bool Call();
		
	private:
		void Calc();
		
		struct Data {
			int playerID;
			
			sf::Color color;
			
			sf::String player;
			sf::String health;
		};
		
	private:
		bool wasInitialized_;
		bool needUpdate_;
		
		std::vector < sf::Sprite > toDraw_;
		std::vector < sprite::CSprite* >* spriteList_;
		
		CLabel* player;
		
		std::vector < Data > data_;
		
};



} /* namespace gui */

#endif // SELECT_WIDGET_HPP
