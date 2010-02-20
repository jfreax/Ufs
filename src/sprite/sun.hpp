/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SUN_HPP
#define SUN_HPP

#include "../engine/sprite/sprite.hpp"
#include "../particle.hpp"
#include "../engine/ui/window.hpp"
#include "../gui/window/quit.hpp"
#include "../map/system.hpp"


namespace sprite
{


class CSun : public CSprite
{
	public:
		CSun();

		void Render ( sf::RenderTarget& Target ) const;

		void Update();
		
		void CalcGFX();
		void ShowGlow();
		
		sf::Color GetColor();
		void SetColor ( sf::Color color );
		
	private:
		sf::Sprite glow_;
		sf::Sprite brightness_;
		
		double showGlow_;
		sf::Shape* galaxyGlow_;

		uint8_t alpha_;
		
		/* STATIC */
// 	public:
// 		static void CalcGFX();
		
};



} /* namespace sprite */

#endif // SUN_HPP
