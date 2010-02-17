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

#ifndef SHIP_HPP
#define SHIP_HPP

#include "../engine/sprite/sprite.hpp"
#include "../engine/sprite/moveable.hpp"

namespace sprite
{


class CShip : public CSprite, public CMoveAble
{
	public:
		CShip();
		
		void Render ( sf::RenderTarget& Target ) const;
		
		void Update();
		
		void SetRotation ( float Rotation );
	private:

};

} /* namespace sprite */

#endif // SHIP_HPP
