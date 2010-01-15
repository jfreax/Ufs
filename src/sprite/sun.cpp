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


#include "../game.hpp"
#include "sun.hpp"


namespace sprite
{


CSun::CSun ()
{
	/* Imageresourcen manager */
	CImageResource* imageResource = GetGameClass()->GetImgResource();

	/* load picture of a sun */
	background_ = new CAnimation ( imageResource->Get( "images/sun/sun.png" ), 0, 0.05f );
	background_->SetCenter ( GetCenter() );
	
	/* Set blendmode */
	background_->SetBlendMode ( sf::Blend::Multiply );

	/* Bild wird nur auf diesem Bereich gezeichnet */
	mask_ = sf::Shape::Circle ( 250, 250, 250, sf::Color ( 255, 255, 255 ) );
	
	/* picture of sun glow */
	glow_.SetImage( *imageResource->Get ( "images/sun/sun_glow.png" ) );
	glow_.SetPosition( -83, -83 );
	
	/* storm picture */
	brightness_.SetImage( *imageResource->Get ( "images/sun/sun_storm.png" ) );
	brightness_.SetPosition( 10, 10 );	

	brightness_.SetColor( sf::Color ( 255, 255, 255, 100 ) );
	brightness_.SetBlendMode( sf::Blend::Alpha );
	
	/* Particlesystem */
// 	particle.set_Material ( imageResource->Get ( "images/sun/fire.png"  ) );
// 	particle.set_Dimension ( sf::Vector2i ( 8, 8 ) );
// 	
// 
// 	
// 	CEmitter nEmit ( 500, 6.2f, sf::Vector3f ( 0, 0, 0.f ) );
// 	nEmit.set_Velocity ( 2.f, 22.f );
// 	nEmit.set_Direction ( 90, 5.f );
// 	nEmit.set_Position( sf::Vector3f ( 50,50,1) );
// 	
// 	particle.AddEmitter ( nEmit );

// 	particle.set_Material ( imageResource->Get ( "images/star.png" ) );
// 	particle.set_Dimension ( sf::Vector2i ( 500, 500 ) );

	/* Neuen Emitter erstellen */
	CEmitter emitter ( 100 );
	
	/* Emittereigenschaften setzen */
	emitter.SetLife ( 10.f );
	emitter.SetPosition ( sf::Vector3f ( 0, 0, 2 ) );	
	emitter.SetVelocity ( 0.f, 2.f );
	emitter.SetDirection ( 45, 50.f );
	
	/* Dem Partikelsystem den Emitter hinzufügen */
	particle.AddEmitter ( emitter );

// 	particle.Initialization ( 100, sf::Vector2f ( 10, 20 ), sf::Vector3f ( 100, 100, 0 ), sf::Vector2f ( 100000,300000 ), sf::Vector2f ( 40, 90 ), sf::Vector2f ( 10, 20 ), sf::Color::White );


	/* Set properties */
	this->SetZoomFactor( 0.3 );
	this->SetZoomLevel ( 0.02 );
}




void CSun::Render ( sf::RenderTarget& Target ) const
{
	/* run sprite renderer */
	CSprite::Render( Target );
	
	/* draw sun glow */
	Target.Draw ( glow_ );
	
	/* render particlesystem */
// 	particle.Draw();
// 
// 	Target.Draw ( brightness_ );
// 	particle.Draw();
}


void CSun::Update ( void )
{
	/* run updater from sprite-class */
	CSprite::Update();
	
	/* Helligkeitsflimmern reinbringen */
	alpha_++;
// 	brightness_.SetColor ( sf::Color ( 255, 255, 255, alpha_ ) );

	/* Partikelsystem akualisieren */
// 	particle.Draw ( );
	particle.Update();
	
}


} /* namespace sprite */

