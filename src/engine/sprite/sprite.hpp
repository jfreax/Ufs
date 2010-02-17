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

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../animation.hpp"


namespace sprite
{


class CSprite : public sf::Drawable
{
	public:
		CSprite();
		CSprite ( const CSprite& instance );
		
		~CSprite();
		
		virtual void Render ( sf::RenderTarget& Target ) const;
		virtual void DrawMarker();
		
		virtual void Update();
		virtual void UpdateMarker();
		
		virtual void CalcGFX();
		
		float GetPositionX() const;
		void SetPositionX ( float x );
		
		float GetPositionY() const;
		void SetPositionY ( float y );
		
		unsigned int GetId() const;
		
		unsigned int GetPlayer() const;
		void SetPlayer ( unsigned int player );
		
		sf::Rect<float> GetDimension() const;
		sf::Image* GetMiniImage() const;
		
		void Scale ( double scale );
		
		void SetZoomLevel ( float zLevel );
		float GetZoomLevel() const;
		
		void SetZoomFactor ( float factor );
		float GetZoomFactor() const;

	protected:
		unsigned int id_;
		unsigned int player_;
		
		bool initialized;
		
		sf::Sprite backgroundStatic_;
		CAnimation* background_;
		
		sf::Image* miniImage_;
		sf::Shape mask_;
		
		sf::Shape* gfxMarker_;
		float markerAPos_;
		
		float zoomLevel_;
		float zoomFactor_;
};


} /* namespace sprite */

#endif /* SPRITE_HPP */
