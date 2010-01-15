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

#include "titlebar.hpp"
#include "../../engine/ui/window.hpp"
#include "../../engine/ui/theme.hpp"
#include "../../game.hpp"

namespace gui
{

/* CLOSE BUTTON 
   ------------ */

CCloseButton::CCloseButton ( void )
{
	CGame* game = GetGameClass();
	CTheme* theme = game->GetGuiManager()->GetTheme();
	
	/* Name des Buttons */
	name_ = "close";

	/* Position und Größe */
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
	/* Beim Klick -> Fenster schließen */
	motherWin_->Close();
	return true;
}



/* MINIMIZE BUTTON 
   --------------- */
   
CMinimizeButton::CMinimizeButton ( void )
{
	CGame* game = GetGameClass();
	CTheme* theme = game->GetGuiManager()->GetTheme();
	
	/* Name des Button */
	name_ = "minimize";
	
	/* Größe und Position */
	sf::Vector2f minimizeButtonPosition ( theme->window_.iconMinimize.x, theme->window_.iconMinimize.y - theme->window_.titlebar );
	sf::Vector2f minimizeButtonSize ( theme->window_.iconMinimize.z, theme->window_.iconMinimize.z );
	
	this->SetSize ( minimizeButtonSize );
	this->SetPosition ( minimizeButtonPosition );
	this->SetBackground ( *game->GetImgResource()->Get ( settings::GetThemePath() + "icons/minimize.png" ) );

	/* Aktionen alle erstmal AUS */
	doMinimize_ = doMaximize_ = false;
	originalSize_ = 0;
}


bool CMinimizeButton::onLeftClick ( void )
{
	diff_ = 2000 * GetGameClass()->GetApp()->GetFrameTime();
	
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


/* HIDE BUTTON 
   ----------- */

CHideButton::CHideButton ( CButton* minimizeButton ) : minimizeButton_ ( minimizeButton )
{
	CGame* game = GetGameClass();
	CTheme* theme = game->GetGuiManager()->GetTheme();
	
	/* Name des Button */
	name_ = "minimize";
	
	/* Größe und Position */
	sf::Vector2f minimizeButtonPosition ( theme->window_.iconHide.x, theme->window_.iconHide.y - theme->window_.titlebar );
	sf::Vector2f minimizeButtonSize ( theme->window_.iconHide.z, theme->window_.iconHide.z );
	
	this->SetSize ( minimizeButtonSize );
	this->SetPosition ( minimizeButtonPosition );
	this->SetBackground ( *game->GetImgResource()->Get ( settings::GetThemePath() + "icons/hide.png" ) );
	
	/* Aktionen alle erstmal AUS */
	doHide_ = doShow_ = finish_ = false;
}


bool CHideButton::onLeftClick ( void )
{
	/* Minimizebutton mit ausführen */
	minimizeButton_->onLeftClick();
	
	diff_ = 1500 * GetGameClass()->GetApp()->GetFrameTime();
	

	/* Fenster links andocken lassen (hide) */
	if ( !doHide_ && motherWin_->GetSize().y )
	{
		/* Originalposition des Fensters merken */
		originalPosition_ = motherWin_->GetPosition();
		
		/* Neue Position vom GUIManager zuweisen lassen */
		dockPosition_ = GetGameClass()->GetGuiManager()->AddWindowToDock ( motherWin_ );
		
		diffRotate_ = - ( 90 * diff_ ) / (motherWin_->GetSize().y + 1);
		doHide_ = true;
		finish_ = false;
	}
	else
	{
		/* Fenster aus dem Dockmanager entfernen */
		GetGameClass()->GetGuiManager()->RemoveWindowFromDock ( motherWin_ );
	
		doShow_ = true;
	}

}


bool CHideButton::Call ( void )
{
	/* Fenster ins Dock schieben */
	if ( doHide_ && !finish_ )
	{
		/* Fenster drehen */
		if ( !motherWin_->GetAngle() || motherWin_->GetAngle() >= 270 ) 
		{
			motherWin_->Rotate( diffRotate_ );
		}
		
		finish_ = true;
		
		/* Neue Fensterposition errechnen */
		sf::Vector2f newPosition = motherWin_->GetPosition();
		if ( dockPosition_.x + diff_ < motherWin_->GetPosition().x )
		{
			newPosition.x -= diff_;
			finish_ = false;
		}
		else if ( dockPosition_.x > motherWin_->GetPosition().x )
		{
			newPosition.x += diff_;
			finish_ = false;
		}
		if ( dockPosition_.y + diff_ < motherWin_->GetPosition().y )
		{
			newPosition.y -= diff_;
			finish_ = false;
		}
		else if ( dockPosition_.y > motherWin_->GetPosition().y )
		{
			newPosition.y += diff_;
			finish_ = false;
		}
		
		/* Und Position setzen */
		motherWin_->SetPosition ( newPosition );
	}
	else if ( doHide_ )
	{
		motherWin_->SetPosition ( dockPosition_ );
	
		motherWin_->SetAngle ( 270 );
		motherWin_->Update();
		doHide_ = false;
	}
	/* Fenster aus dem Dock holen und anzeigen */
	else if ( doShow_ && motherWin_->GetAngle() > -diffRotate_ )
	{
		/* Fenster drehen */
		motherWin_->Rotate( -diffRotate_ );
		
		/* Alte Position wieder herstellen TODO ! */
		motherWin_->SetPosition ( originalPosition_ );
	}
	else if ( doShow_ )
	{
		motherWin_->SetAngle ( 0 );
		doShow_ = false;
	}

}


} /* namespace gui */

