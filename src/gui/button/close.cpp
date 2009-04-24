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

#include "close.hpp"
#include "../../ui/window.hpp"
#include "../../ui/theme.hpp"
#include "../../game.hpp"

namespace gui
{

/* CLOSE BUTTON 
   ------------ */

CCloseButton::CCloseButton ( void )
{
	CGame* game = GetGameClass();
	CTheme* theme = game->GetGuiManager()->GetTheme();

	sf::Vector2f closeButtonPosition (
		theme->window_.iconClose.x,
		theme->window_.iconClose.y - theme->window_.titlebar );
	sf::Vector2f closeButtonSize ( theme->window_.iconClose.z, theme->window_.iconClose.z );

	this->SetPosition ( closeButtonPosition );
	this->SetSize ( closeButtonSize );
	this->SetBackground ( *game->GetImgResource()->Get ( settings::GetThemePath() + "icons/close.png" ) );

}

bool CCloseButton::onLeftClick ( void )
{
	motherWin_->Close();
	return true;
}



/* MINIMIZE BUTTON 
   --------------- */
   
CMinimizeButton::CMinimizeButton ( void )
{
	CGame* game = GetGameClass();
	CTheme* theme = game->GetGuiManager()->GetTheme();

	sf::Vector2f minimizeButtonPosition ( theme->window_.iconMinimize.x, theme->window_.iconMinimize.y - theme->window_.titlebar );
	sf::Vector2f minimizeButtonSize ( theme->window_.iconClose.z, theme->window_.iconClose.z );
	
	this->SetSize ( minimizeButtonSize );
	this->SetPosition ( minimizeButtonPosition );
	this->SetBackground ( *game->GetImgResource()->Get ( settings::GetThemePath() + "icons/minimize.png" ) );

	doMinimize_ = doMaximize_ = false;
	originalSize_ = 0;
}


bool CMinimizeButton::Call ( void )
{
	/* Fenster minimieren */
	if ( doMinimize_ )
	{
		sf::Vector2f size = motherWin_->GetSize();
		motherWin_->SetSize ( sf::Vector2f ( size.x, size.y - diff_ ), true );

		/* Icon 90 Grad drehen */
		if ( background_.GetRotation() + diffRotate_ <= 90 )
		{
			background_.Rotate ( diffRotate_ );
		}
		else
		{
			background_.SetRotation ( 90 );
		}

		/* Einfahren stoppen */
		if ( size.y <= 0 )
		{
			doMinimize_ = false;
		}
	}
	else if ( doMaximize_ )
	{
		sf::Vector2f size = motherWin_->GetSize();
		motherWin_->SetSize ( sf::Vector2f ( size.x, size.y + diff_ ), true );

		/* Icon 90 Grad zurückdrehen */
		if ( background_.GetRotation() - diffRotate_ >= 0 )
		{
			background_.Rotate ( -diffRotate_ );
		}
		else
		{
			background_.SetRotation ( 0 );
		}

		/* Ausfahren stoppen */
		if ( size.y >= originalSize_ )
		{
			/* Zur Sicherheit nochmal die exakte Größe setzen (Rundungsfehler) */
			motherWin_->SetSize ( sf::Vector2f ( size.x, originalSize_ ) );
			doMaximize_ = false;
			originalSize_ = 0;
		}	
	}
}


bool CMinimizeButton::onLeftClick ( void )
{
	diff_ = 1000 * GetGameClass()->GetApp()->GetFrameTime();
	
	/* Das Fenster beim klicken "einfahren" */
	if ( motherWin_->GetSize().y > 0 && !doMaximize_ && !originalSize_ )
	{
		diffRotate_ = ( 90 * diff_ ) / (motherWin_->GetSize().y+1);
		originalSize_ = motherWin_->GetSize().y;
		doMinimize_ = true;
	}
	/* Das Fenster ist minimiert, also "ausfahren" */
	else
	{
		diffRotate_ = ( 90 * diff_ ) / ( originalSize_ );
		doMaximize_ = true;
	}
	
	return true;
}


} /* namespace gui */

