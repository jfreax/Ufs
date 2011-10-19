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

#include "main_menu.hpp"
#include "../../engine/ui/window.hpp"

#include "../../game.hpp"

namespace gui
{


CSingleplayerButton::CSingleplayerButton()
{
	/* Neuen Hintergrundsprite anlegen */
	sf::Image* buttonImg = GetGameClass()->GetImgResource()->Get( "themes/default/buttons/main_button1.png" );
// 	sf::Sprite buttonSprite ( *buttonImg );
	
	/* Verhältnis der Originalhintergrundgröße mit der jetzigen */
	double verh = settings::GetWidth() / 1024.f;
	
	/* Name des Buttons */
	this->SetName ( "singleplayer" );
	
	/* Größe anpassen */
	this->SetSize ( sf::Vector2f ( buttonImg->GetWidth() * verh, buttonImg->GetHeight() * verh ) );
	
	/* Sprite als Hintergrund zuweisen */
	this->SetBackground ( buttonImg );
	
	/* Position setzen */
	this->SetPosition( sf::Vector2f ( 482.f * verh, 139.f * verh ) );
	
	/* Text setzen */
	this->SetText ( "Singleplayer" );
	this->SetTextPosition ( sf::Vector2f ( 35.f * verh, 14.f * verh ) );
	this->SetTextSize ( 16 );
	this->SetFont ( "main_menu" );
	
	/* Textposition speichern */
	endTextPos_ = sf::Vector2f ( 35.f * verh, 14.f * verh );
	
	/* Mousehover Effekt deaktivieren */
	hoverEffect_ = unHoverEffect_ = false;
	
	sf::Vector2f testSize ( 100.f, 100.f );
	
	testShape.AddPoint( 0,0 );
	testShape.AddPoint( 100,0 );
	testShape.AddPoint( 100,100 );
	testShape.AddPoint( 0,100 );
	
}


bool CSingleplayerButton::Call()
{
// GetGameClass()->GetApp()->Draw( testShape );

glBegin( GL_QUADS );

  glColor3f(0.3, 0.9, 0.3);
    glVertex3f( 1.5,  0.5,  -1);
    glVertex3f(-1.5,  0.5,  -1);
  glColor3f(0.9, 0.3, 0.3);
    glVertex3f(-1.5, -0.5,  -1);
    glVertex3f( 1.5, -0.5,  -1);

glEnd();

	/* MouseHover-Effekt */
	if ( hoverEffect_ )
	{
		sf::Vector2f textPos = this->GetTextPosition();
		textPos.y -= diff_;
		
		/* Textposition anpassen */
		if ( textPos.y >= 0 )
		{	
			this->SetTextPosition ( textPos );
		}
		/* Endposition erreicht, Effekt beenden */
		else
		{
			this->SetTextPosition ( sf::Vector2f ( textPos.x, 0 ) );
			hoverEffect_ = false;
		}
	
	}
	/* MouseUnHover-Effekt */
	else if ( unHoverEffect_ )
	{
		sf::Vector2f textPos = this->GetTextPosition();
		textPos.y += diff_;
		
		/* Textposition anpassen */
		if ( textPos.y <= endTextPos_.y )
		{	
			this->SetTextPosition ( textPos );
		}
		/* Endposition erreicht, Effekt beenden */
		else
		{
			this->SetTextPosition ( endTextPos_ );
			hoverEffect_ = false;
		}

	}

}


bool CSingleplayerButton::onHoverMouse()
{
	isMouseHere_ = wasMouseHere_ = true;
	background_.SetColor( sf::Color ( 255, 255, 255, 230 ) );
	
	/* Effekt aktivieren */
	hoverEffect_ = true;
	unHoverEffect_ = false;
	
	/* Für Fps-unabhängige Berechnungen */
	diff_ = 20.f * GetGameClass()->GetApp()->GetFrameTime();
}

bool CSingleplayerButton::onUnHoverMouse()
{
	background_.SetColor( sf::Color ( 255, 255, 255, 255 ) );
	
	/* Effekt aktivieren */
	hoverEffect_ = false;
	unHoverEffect_ = true;
	
	/* Für Fps-unabhängige Berechnungen */
	diff_ = 10.f * GetGameClass()->GetApp()->GetFrameTime();
}


} /* namespace gui */
