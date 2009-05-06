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
#include "../settings.hpp"


namespace sprite
{


CSprite::CSprite ( void )
{
	background_ = NULL;
}


CSprite::~CSprite(void )
{
	delete background_;
}



void CSprite::Render ( sf::RenderTarget& Target ) const
{
	Target.Draw ( mask_ );
	Target.Draw ( *background_ );
	

}


void CSprite::Update ( void )
{
	/* Prüfen ob dieses Sprite überhaupt richtig initialisiert wurde */
	if ( !background_ )
	{
		return;
	}
	
	/* Animation berechnen */
	background_->Update();
}


void CSprite::Scale ( double scale )
{
	float verh = (float)settings::GetHeight() / (float)settings::GetWidth();
	this->SetScale ( scale * verh, scale );
	
}


} /* namespace sprite */
