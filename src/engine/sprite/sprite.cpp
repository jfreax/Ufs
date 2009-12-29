/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Publicturbine_1.3ds
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

#include "sprite.hpp"
#include "../../settings/settings.hpp"
#include "../../game.hpp"


namespace sprite
{

static unsigned int globalId = 0;

CSprite::CSprite()
{
	id_ = ++globalId;
	
	player_ = 0; /* "NATURE" */
	
	background_ = NULL;
	miniImage_  = NULL;
}


CSprite::CSprite ( const sprite::CSprite& instance ) : Drawable ( instance )
{ /* TODO */
	id_ = ++globalId;
	
// 	background_ = new CAnimation (  instance.background_->, 1, 0.05f );
	background_ = instance.background_;
	
	player_ = instance.player_;
	mask_ = instance.mask_;
	zoomLevel_ = instance.zoomLevel_;
}



CSprite::~CSprite()
{
	delete background_;
}



void CSprite::Render ( sf::RenderTarget& Target ) const
{
	if ( !background_ )
		return;
	
	Target.Draw ( mask_ );
	Target.Draw ( *background_ );
}


void CSprite::Update()
{
	if ( !background_ )
		return;
	
	/* Animation berechnen */
	background_->Update();
	
	/* Scale image to zoom level */
	this->Scale ( 1 / ( GetZoomLevel() + GetGameClass()->GetMapManager()->GetZoomLevel() ));
	
}


unsigned int CSprite::GetId()
{
	return id_;
}


unsigned int CSprite::GetPlayer()
{
	return player_;
}


void CSprite::SetPlayer ( unsigned int player )
{
	player_ = player;
}



sf::Rect<float> CSprite::GetDimension()
{
	return sf::Rect<float> ( GetPosition().x, GetPosition().y,
				 GetPosition().x + background_->GetSize().x * this->GetScale().x, GetPosition().y + background_->GetSize().y * this->GetScale().y );
}


sf::Image* CSprite::GetMiniImage()
{
	return miniImage_;
}



void CSprite::Scale ( double scale )
{
	float verh = (float)settings::GetHeight() / (float)settings::GetWidth();
	this->SetScale ( scale * verh, scale );
	
}


float CSprite::GetZoomLevel()
{
	return zoomLevel_;
}


void CSprite::SetZoomLevel ( float zLevel )
{
	zoomLevel_ = zLevel;
}




} /* namespace sprite */
