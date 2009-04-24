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

#include "widget.hpp"
#include "manager.hpp"

#include "../game.hpp"
#include "../resource.hpp"
#include "../settings.hpp"
#include "../action.hpp"

#include "button.hpp"
#include "window.hpp"

#include "../gui/button/close.hpp"

namespace gui
{

CWindow::CWindow ( CTheme *theme, sf::Vector2f position, sf::Vector2f size )
{
	static unsigned int globalId = 0;
	id_ = ++globalId;

	this->NoUpdate ( true );
	{
		formWin_ = NULL;
		formWinBorder_ = NULL;
		formTitlebar_ = NULL;

		titlebarImage_ = NULL; /* TODO thememanager! */


		CGame* game = GetGameClass();

		minSize_ = theme->window_.minSize;
		maxSize_ = theme->window_.maxSize;

		formRound_ = 10;
		backgroundColor_ = theme->window_.backgroundColor;

		border_ = theme->window_.border;
		borderColor_ = theme->window_.borderColor;


		{ /* Titlebar */
			titlebar_ = theme->window_.titlebar;
			titlebarLength_ = theme->window_.titlebarLength;
			titlebarColor_ = theme->window_.titlebarColor;

			this->AddWidget ( new gui::CCloseButton );
			this->AddWidget ( new gui::CMinimizeButton );
		} /* End Titlebar */


		closeAble_ = true;
		moveAble_ = true;
		resizeAble_ = true;

		this->SetPosition ( position );
		this->SetSize ( size );

	}

	this->NoUpdate ( false );
}


const unsigned int CWindow::GetId ( void ) const
{
	return id_;
}


void CWindow::NoUpdate ( bool ison )
{
	noUpdate_ = ison;
	if ( !ison )
	{
		this->Update();
	}
}


void CWindow::Update ( void )
{
	/* Keine Updates durchführen! */
	if ( noUpdate_ )
	{
		return;
	}

	/* Hintergrundbild / -Shape */
	if ( background_.GetSize().x != 1.f )
	{
		background_.SetPosition ( position_ );
		background_.Resize ( curSize_ );
	}
	else
	{
		calcBackground();
	}

	/* Inhalte ebenfalls aktualisieren */
	for ( std::vector<gui::CWidget*>::size_type i = widgetList_.size(); i; --i )
	{
		widgetList_.at ( i - 1 )->Update();
	}
}


bool CWindow::Render ( void )
{
	sf::RenderWindow* app = GetGameClass()->GetApp();

	/* Titlebar */
	app->Draw ( *formTitlebar_ );


	/* Hintergrund */
	if ( background_.GetSize().x && background_.GetSize().x != 1.f )
	{
		app->Draw ( background_ );
	}
	else
	{
		app->Draw ( *formWin_ );
		app->Draw ( *formWinBorder_ );
	}

	for ( std::vector<gui::CWidget*>::size_type i = widgetList_.size(); i; --i )
	{
		widgetList_[i-1]->Render();
	}
}


void CWindow::Close ( void )
{
	GetGameClass()->GetGuiManager()->CloseWindow ( this );
}


void CWindow::AddWidget ( CWidget* widget_ )
{
	widgetList_.push_back ( widget_ );

	widget_->SetMotherWin ( this );
	widget_->Update();
}


void CWindow::calcBackground ( void )
{
	/* alte Shapes löschen */
	delete formWin_;
	formWin_ = new sf::Shape();

	delete formWinBorder_;
	formWinBorder_ = new sf::Shape();

	delete formTitlebar_;
	formTitlebar_ = new sf::Shape();


	/* Variablen deklarieren */
	float Angle;
	sf::Vector2f Center;

	/* Titelleiste
	   ------------------ */
	if ( !titlebarImage_ )
	{
		int length = GetSize().x;
		if ( titlebarLength_ )
		{
			length = titlebarLength_;
		}
	
		/* linke obere Rundung */
		sf::Vector2f Center ( formRound_, formRound_ );
		for ( int i = 40; i < 60; ++i )
		{
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formTitlebar_->AddPoint ( Center + Offset * formRound_, titlebarColor_ );
		}

		/* rechte obere Rundung */
		Center = sf::Vector2f ( length - formRound_, formRound_ );
		for ( int i = 60; i < 80; ++i )
		{
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formTitlebar_->AddPoint ( Center + Offset * formRound_, titlebarColor_ );
		}

		/* untere rechte Ecke */
		formTitlebar_->AddPoint ( length, titlebar_, titlebarColor_ );

		/* untere linke Ecke */
		formTitlebar_->AddPoint ( 0, titlebar_, titlebarColor_ );
	}

	/* Fensterhintergrund
	   ------------------ */

	/* Nur Hintergrund zeichnen, wenn auch Fenster groß genug ist */
	if ( GetSize().y > titlebar_ )
	{
		/* Temp. Variable falls obere rechte Ecke keine Rundung haben soll */
		float formRound = GetSize().x - titlebarLength_ > formRound_ ? formRound_ : GetSize().x - titlebarLength_;
	
		/* obere linke Ecke */
		formWin_->AddPoint ( 0, 0, backgroundColor_ );
		
		/* obere rechte Ecke
		   Wenn Titelbar kürzer als Fenster, dann Ecke abrunden */
		if ( titlebarLength_ && titlebarLength_ < GetSize().x )
		{
			Center = sf::Vector2f ( GetSize().x - formRound, formRound );
			for ( int i = 60; i < 80; ++i )
			{
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formWin_->AddPoint ( Center + Offset * formRound, backgroundColor_ );
			}
		}
		else
		{
			formWin_->AddPoint ( GetSize().x, 0, backgroundColor_ );
		}

		/* rechte untere Rundung */
		Center = sf::Vector2f ( GetSize().x - formRound_, GetSize().y - formRound_ );
		for ( int i = 0; i < 20; ++i )
		{
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWin_->AddPoint ( Center + Offset * formRound_, backgroundColor_ );
		}

		/* Farbanpassungen */
		sf::Color lastColor = backgroundColor_;
		lastColor.a = 160;

		/* linke untere Rundung */
		Center = sf::Vector2f ( formRound_, GetSize().y - formRound_ );
		for ( int i = 20; i < 40; ++i )
		{
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWin_->AddPoint ( Center + Offset * formRound_, lastColor );
		}
	}


	/* Rand des Fensters
	   -----------------*/
	if ( border_ )
	{
		/* Allgemeine Einstellungen
		   Stärke der Umrandung */
		formWinBorder_->SetOutlineWidth ( border_ );

		/* Nur Rand zeichnen, keinen Inhalt */
		formWinBorder_->EnableFill ( false );

		/* Farbe des Randes */
		sf::Color borderColor = sf::Color ( 80, 80, 80, backgroundColor_.a );

		/* Wenn Titelleiste vorhanden, oben keine Rundungen zeichnen */
		if ( GetTitlebarDimension().GetWidth() )
		{
			/* obere linke Ecke */
			formWinBorder_->AddPoint ( 0, 0, borderColor, borderColor );

			/* obere rechte Ecke */
			formWinBorder_->AddPoint ( GetSize().x, 0, borderColor, borderColor );
		}
		else
		{
			/* linke obere Rundung */
			Center = sf::Vector2f ( formRound_, formRound_ );
			for ( int i = 40; i < 60; ++i )
			{
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
			}

			/* rechte obere Rundung */
			Center = sf::Vector2f ( GetSize().x - formRound_, formRound_ );
			for ( int i = 60; i < 80; ++i )
			{
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
			}
		}

		/* rechte untere Rundung */
		sf::Vector2f Center ( GetSize().x - formRound_, GetSize().y - formRound_ );
		for ( int i = 0; i < 20; ++i )
		{
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
		}

		/* linke untere Rundung */
		Center = sf::Vector2f ( formRound_, GetSize().y - formRound_ );
		for ( int i = 20; i < 40; ++i )
		{
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
		}
	}


	formWin_->SetPosition ( GetPosition() );
	formWinBorder_->SetPosition ( GetPosition() );
	formTitlebar_->SetPosition ( GetPosition().x, GetPosition().y - titlebar_ );
}



std::vector< gui::CWidget* >* CWindow::GetWidgetList ( void )
{
	return &widgetList_;
}


void CWindow::SetSize ( sf::Vector2f size_, bool force )
{
	curSize_ = size_;

	sf::Vector2f minSize = minSize_;

	/* Egal welche min. Einstellungen es gibt,
	   ich soll sie ignorieren.. nagut... */
	if ( force )
	{
		minSize = sf::Vector2f ( 0, 0 );
	}

	if ( curSize_.x < minSize.x )
	{
		curSize_.x = minSize.x;
	}
	if ( curSize_.y < minSize.y )
	{
		curSize_.y = minSize.y;
	}
	if ( curSize_.x > maxSize_.x )
	{
		curSize_.x = maxSize_.x;
	}
	if ( curSize_.y > maxSize_.y )
	{
		curSize_.y = maxSize_.y;
	}

	this->Update();
}


void CWindow::SetSizeInPercent ( sf::Vector2f sizePercent )
{
	this->SetSize ( sf::Vector2f ( settings::GetWidth() * sizePercent.x * 0.01, settings::GetHeight() * sizePercent.y * 0.01 ) );
}


sf::Vector2f CWindow::GetSize ( void ) const
{
	return curSize_;
}


void CWindow::SetPosition ( sf::Vector2f position )
{
	position_ = position;

	if ( position_.x + curSize_.x < 0 )
		position_.x = - curSize_.x + 10;

	if ( position_.x > settings::GetWidth() - 10 )
		position_.x = settings::GetWidth() - 10;

	if ( position_.y - titlebar_ < 0 )
		position_.y = titlebar_ + 1;

	if ( position_.y > settings::GetHeight() )
		position_.y = settings::GetHeight();

	this->Update();
}


sf::Vector2f CWindow::GetPosition ( void ) const
{
	return position_;
}


sf::Rect<float> CWindow::GetWindowDimension ( void ) const
{
	return sf::Rect<float> ( position_.x, position_.y - titlebar_, position_.x + curSize_.x, position_.y + curSize_.y );
}


sf::Rect<float> CWindow::GetTitlebarDimension ( void ) const
{
	sf::Vector2f titlePos ( position_.x, position_.y - titlebar_ );
	sf::Vector2f titleEndPos ( position_.x + curSize_.x, position_.y );
	return sf::Rect<float> ( titlePos.x, titlePos.y, titleEndPos.x, titleEndPos.y );
}


sf::Rect<float> CWindow::GetResizeArea ( void ) const
{
	sf::Vector2f point = GetPosition() + GetSize() - sf::Vector2f ( 2, 2 );

	return sf::Rect<float> ( point.x, point.y, point.x + 3, point.y + 3 );
}


void CWindow::SetTitlebar ( unsigned int titlebar )
{
	titlebar_ = titlebar;
	this->Update();
}


void CWindow::SetMoveWindow ( bool ison )
{
	moveWindow_ = ison;
}


bool CWindow::GetMoveWindow ( void ) const
{
	return moveWindow_;
}


} /* namespace gui */
