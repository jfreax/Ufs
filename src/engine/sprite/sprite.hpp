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

#include "../../animation.hpp"

class CSystem;

namespace sprite
{
	
	
enum SPRITETYPE
{
	PLANET,
	SUN,
	SHIP
};


class CSprite : public sf::Drawable
{
	public:
		CSprite();
		CSprite ( const CSprite& instance );
		
		~CSprite();
		
		virtual void Render ( sf::RenderTarget& Target ) const;
		virtual void DrawMarker();
		
		virtual void Update();
		virtual void UpdateLogic();
		virtual void UpdateMarker();
		
		void SetMotherSystem ( CSystem* sys );
		
		float GetPositionX() const;
		void SetPositionX ( float x );
		
		float GetPositionY() const;
		void SetPositionY ( float y );
		
		unsigned int GetId() const;
		SPRITETYPE GetType() const;
		
		unsigned int GetPlayer() const;
		void SetPlayer ( unsigned int player );
		
		sf::Rect<float> GetDimension() const;
		sf::Rect<float> GetDimensionInGalaxy() const;
		
		sf::Sprite* GetBackground();
		sf::Image* GetMiniImage() const;
		
		void Scale ( double scale );
		
		void SetZoomLevel ( float zLevel );
		float GetZoomLevel() const;
		
		void SetZoomFactor ( float factor );
		float GetZoomFactor() const;

	protected:
		unsigned int id_;
		SPRITETYPE spriteType_;
		bool initialized_;
		
		CSystem* motherSystem_;
		
		unsigned int player_;
		
		sf::Sprite backgroundStatic_;
		CAnimation* background_;
		
		sf::Image* miniImage_;
		sf::Shape mask_;
		
		float zoomLevel_;
		float zoomFactor_;
		
		double oldZoom_;
		
		int alpha_; /* Overall opacity */
		
		double markerRotation_;
		
		/* STATIC */
// 	public:
// 		static void CalcGFX();
		
// 	protected:
// 		static sf::Shape* gfxMarker_;
// 		float markerWidth_;
};


void Initialize();
sf::Shape& GetGfxMarker();


} /* namespace sprite */

#endif /* SPRITE_HPP */
