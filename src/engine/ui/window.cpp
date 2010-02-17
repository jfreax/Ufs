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

#include "../../game.hpp"
#include "../../gui/button/titlebar.hpp"
#include "../../gui/other/spacer.hpp"


namespace gui
{

CWindow::CWindow ( bool withTitlebarPossible )
{
	static unsigned int globalId = 0;
	id_ = ++globalId;

	CGame* game = GetGameClass();
	CTheme* theme = game->GetGuiManager()->GetTheme();
	
	this->SetShow ( true );
	this->NoUpdate ( true );
	{
		formWin_ = NULL;
		formWinBorder_ = NULL;
		formTitlebar_ = NULL;

		titlebarImage_ = NULL; /* TODO thememanager! */

		minSize_ = theme->window_.minSize;
		maxSize_ = theme->window_.maxSize;

		showBackground_ = true;
		formRound_ = 10;
		backgroundColor_ = theme->window_.backgroundColor;

		border_ = theme->window_.border;
		borderColor_ = theme->window_.borderColor;
		
		freeToSet_ = false;

		if ( withTitlebarPossible ) { /* Titlebar */
			this->SetTitlebar ( theme->window_.titlebar );
			this->AddWidget ( new gui::CCloseButton );
			this->AddWidget ( new gui::CHideButton ( (CButton*)this->AddWidget ( new gui::CMinimizeButton ) ) );

			this->SetNoRoundTitlebar ( false );
			this->SetName ( "WINDOW ID: " + util::lCast<std::string>( id_ ) );

			titlebarLength_ = theme->window_.titlebarLength;
			titlebarColor_ = theme->window_.titlebarColor;
		} else {
			titlebar_ = false;
		} /* End Titlebar */


		closeAble_ = true;
		moveAble_ = true;
		resizeAble_ = true;

		SetAngle( 0 );

		this->SetPosition ( sf::Vector2f ( 0, 0 ) );
		this->SetSize ( minSize_ );

		this->SetLayout ( gui::NOTHING );

	}

	this->NoUpdate ( false );
}


CWindow::~CWindow()
{
	delete formWin_;
	delete formWinBorder_;
	delete formTitlebar_;
	delete titlebarImage_;
	
	std::vector< gui::CWidget* >::iterator endIter = widgetList_.end();
	std::vector< gui::CWidget* >::iterator iter = widgetList_.begin();
	for ( ; iter != endIter; ++iter ) {
		delete ( *iter );
	}
}



const unsigned int CWindow::GetId() const
{
	return id_;
}


void CWindow::NoUpdate ( bool ison )
{
	noUpdate_ = ison;

	if ( !ison ) {
		this->Update();
	}
}


void CWindow::Call()
{
	return;
}


void CWindow::Update()
{
	if ( !this->GetShow() )
		return;
	
	/* Keine Updates durchführen! */
	if ( noUpdate_ )
		return;

	/* Hintergrundbild (falls vorhanden) berechnen, ... */
	if ( background_.GetSize().x != 1.f ) {
		background_.SetPosition ( position_ );
		background_.Resize ( curSize_ );
	}

	/* ... ansonsten Backgroundshape berechnen lassen */
	else {
		this->CalcBackground();
	}

	/* Inhalte ebenfalls aktualisieren */
	for ( std::vector<gui::CWidget*>::size_type i = widgetList_.size(); i; --i ) {
		widgetList_.at ( i - 1 )->Update();
	}
}


void CWindow::UpdateWidgets()
{
	std::vector< gui::CWidget* >::iterator endIter = widgetList_.end();
	std::vector< gui::CWidget* >::iterator iter = widgetList_.begin();

	for ( ; iter != endIter; ++iter ) {
		( *iter )->Update ( true );

	}
}


void CWindow::Render()
{
	if ( !this->GetShow() )
		return;
	
	/* Call all special window features */
	this->Call();
	
	/* Save variables */
	sf::RenderWindow* app = GetGameClass()->GetApp();

	/* Titlebar */
	if ( titlebar_ )
		app->Draw ( *formTitlebar_ );

	/* Background */
	if ( showBackground_ ) {
		if ( background_.GetSize().x && background_.GetSize().x != 1.f ) {
			app->Draw ( background_ );
		} else {
			app->Draw ( *formWin_ );
			app->Draw ( *formWinBorder_ );
		}
	}

	/* Draw all widgets */
	for ( std::vector<gui::CWidget*>::size_type i = widgetList_.size(); i; --i )
		widgetList_[i-1]->Render();
}


bool CWindow::Close ( bool DoNotFreeSpace )
{
	if ( closeAble_ )
		return GetGameClass()->GetGuiManager()->CloseWindow ( this, DoNotFreeSpace );
	else
		return false;
}


CWidget* CWindow::AddWidget ( CWidget* widget, bool newLine )
{
	/* Calc position with the help of layout */
	if ( this->GetWidget() && !newLine ) {
		if ( layout_ == gui::HORIZONTAL ) {
			widget->SetPosition ( sf::Vector2f ( this->GetWidget ( -1 )->GetPosition().x + this->GetWidget ( -1 )->GetDimension().GetWidth() + layoutBorder_, -1 ) );
			widget->MovePosition ( VERTICAL, this->GetWidget( -1 )->GetDimension().Top + ( this->GetWidget( -1 )->GetDimension().GetHeight() * 0.5f ) - ( widget->GetDimension().GetHeight() * 0.5f ) );
		
		} else if ( layout_ == gui::VERTICAL ) {
			widget->SetPosition ( sf::Vector2f ( 1, this->GetWidget ( -1 )->GetPosition().y + this->GetWidget ( -1 )->GetDimension().GetHeight() + layoutBorder_ ) );
			widget->MovePosition ( HORIZONTAL, this->GetWidget( -1 )->GetDimension().Left + ( this->GetWidget( -1 )->GetDimension().GetWidth() * 0.5f ) - ( widget->GetDimension().GetWidth() * 0.5f ) );
			
		} else if ( layout_ == gui::GRID ) {
			/* TODO */
		}
	}
	if ( newLine ) {
		int endY = 0;
		std::vector< gui::CWidget* >::iterator endIter = widgetList_.end();
		std::vector< gui::CWidget* >::iterator iter = widgetList_.begin();
		for ( ; iter != endIter; ++iter )
			if ( ( *iter )->GetDimension().Bottom > endY )
				endY = ( *iter )->GetDimension().Bottom;
			
		widget->MovePosition ( VERTICAL, endY + layoutBorder_ );
	}

	/* Move widget outside the spacer's */
	std::vector< gui::CWidget* >::iterator endIter = spacerList_.end();
	std::vector< gui::CWidget* >::iterator iter = spacerList_.begin();

	for ( ; iter != endIter; ++iter ) {
		if ( ( *iter )->GetDimension().Intersects ( widget->GetDimension() ) ) {
			if ( ( *iter )->GetName() == "spacer_horizontal" ) {
				widget->SetPosition ( sf::Vector2f ( ( *iter )->GetDimension().GetWidth() + ( *iter )->GetPosition().x, -1 ) );
			} else {
				widget->SetPosition ( sf::Vector2f ( -1, ( *iter )->GetDimension().GetHeight() + ( *iter )->GetPosition().y ) );
			}
		}
	}
	
	/* Add a border */
	if ( widget->GetPosition().x == 0 ) {
		widget->MovePosition ( HORIZONTAL, layoutBorder_);
	}
	if ( widget->GetPosition().y == 0 ) {
		widget->MovePosition ( VERTICAL, layoutBorder_);
	}

	/* Add to right widget list */
	if (  widget->GetName() == "spacer_horizontal" || widget->GetName() == "spacer_vertical" ) {
		spacerList_.push_back ( widget );
	} else {
		widgetList_.push_back ( widget );
	}

	widget->SetMotherWin ( this );
	widget->Update();

	return widget;
}


void CWindow::CalcBackground()
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

	if ( titlebar_ ) {
		int length = GetSize().x;

		if ( titlebarLength_ ) {
			length = titlebarLength_;
		}

		/* Titlebar OHNE Rundungen */
		if ( noRoundTitlebar_ ) {
			/* linke obere Rundung */
			formTitlebar_->AddPoint ( 0, 0, titlebarColor_ );

			/* rechte obere Rundung */
			formTitlebar_->AddPoint ( length, 0, titlebarColor_ );
		}

		/* Titelbar mit Rundung */
		else {
			/* linke obere Rundung */
			sf::Vector2f Center ( formRound_, formRound_ );

			for ( int i = 40; i < 60; ++i ) {
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formTitlebar_->AddPoint ( Center + Offset * formRound_, titlebarColor_ );
			}

			/* rechte obere Rundung */
			Center = sf::Vector2f ( length - formRound_, formRound_ );

			for ( int i = 60; i < 80; ++i ) {
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formTitlebar_->AddPoint ( Center + Offset * formRound_, titlebarColor_ );
			}
		}

		/* untere rechte Ecke */
		formTitlebar_->AddPoint ( length, titlebar_, titlebarColor_ );

		/* untere linke Ecke */
		formTitlebar_->AddPoint ( 0, titlebar_, titlebarColor_ );
	}

	/* Fensterhintergrund
	   ------------------ */

	/* Nur Hintergrund zeichnen, wenn auch Fenster groß genug ist */
	if ( GetSize().y > titlebar_ ) {
		/* Temp. Variable falls obere rechte Ecke keine Rundung haben soll */
		float formRound = GetSize().x - titlebarLength_ > formRound_ ? formRound_ : GetSize().x - titlebarLength_;

		/* obere linke Ecke */
		if ( titlebar_ || GetPosition().y == 0 ) {
			formWin_->AddPoint ( 0, 0, backgroundColor_ );
		} else {
			sf::Vector2f Center ( formRound_, formRound_ );
			
			for ( int i = 40; i < 60; ++i ) {
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formWin_->AddPoint ( Center + Offset * formRound_, backgroundColor_ );
			}	
		}

		/* obere rechte Ecke
		   Wenn Titelbar kürzer als Fenster und Fenster nicht am oberen Rand, dann Ecke abrunden */
		if ( ( !titlebar_ || titlebarLength_ < GetSize().x ) && GetPosition().y != 0 ) {
			Center = sf::Vector2f ( GetSize().x - formRound, formRound );

			for ( int i = 60; i < 80; ++i ) {
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formWin_->AddPoint ( Center + Offset * formRound, backgroundColor_ );
			}
		} else {
			formWin_->AddPoint ( GetSize().x, 0, backgroundColor_ );
		}

		/* rechte untere Rundung */
		Center = sf::Vector2f ( GetSize().x - formRound_, GetSize().y - formRound_ );

		for ( int i = 0; i < 20; ++i ) {
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWin_->AddPoint ( Center + Offset * formRound_, backgroundColor_ );
		}

		/* Farbanpassungen */
		sf::Color lastColor = backgroundColor_;

		lastColor.a = 160;

		/* linke untere Rundung */
		Center = sf::Vector2f ( formRound_, GetSize().y - formRound_ );

		for ( int i = 20; i < 40; ++i ) {
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWin_->AddPoint ( Center + Offset * formRound_, lastColor );
		}
	}


	/* Rand des Fensters
	   -----------------*/
	if ( border_ ) {
		/* Allgemeine Einstellungen
		   Stärke der Umrandung */
		formWinBorder_->SetOutlineWidth ( border_ );

		/* Nur Rand zeichnen, keinen Inhalt */
		formWinBorder_->EnableFill ( false );

		/* Farbe des Randes */
		sf::Color borderColor = sf::Color ( 80, 80, 80, backgroundColor_.a );

		/* Wenn Titelleiste vorhanden, oben keine Rundungen zeichnen */

		if ( GetTitlebarDimension().GetWidth() ) {
			/* obere linke Ecke */
			formWinBorder_->AddPoint ( 0, 0, borderColor, borderColor );

			/* obere rechte Ecke */
			formWinBorder_->AddPoint ( GetSize().x, 0, borderColor, borderColor );
		} else {
			/* linke obere Rundung */
			Center = sf::Vector2f ( formRound_, formRound_ );

			for ( int i = 40; i < 60; ++i ) {
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
			}

			/* rechte obere Rundung */
			Center = sf::Vector2f ( GetSize().x - formRound_, formRound_ );

			for ( int i = 60; i < 80; ++i ) {
				Angle = i * 2 * 3.141592654f / 80;
				sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
				formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
			}
		}

		/* rechte untere Rundung */
		sf::Vector2f Center ( GetSize().x - formRound_, GetSize().y - formRound_ );

		for ( int i = 0; i < 20; ++i ) {
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
		}

		/* linke untere Rundung */
		Center = sf::Vector2f ( formRound_, GetSize().y - formRound_ );

		for ( int i = 20; i < 40; ++i ) {
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			formWinBorder_->AddPoint ( Center + Offset * formRound_, borderColor, borderColor );
		}
	}

	/* Positionen anpassen */
	formWin_->SetPosition ( GetPosition() );
	formWinBorder_->SetPosition ( GetPosition() );
	formTitlebar_->SetPosition ( GetPosition().x, GetPosition().y );

	/* Mittelpunkt setzen */
	formTitlebar_->SetCenter( sf::Vector2f ( 0, titlebar_ ) );

	/* Rotation setzen */
	formWin_->SetRotation ( angle_ );
	formWinBorder_->SetRotation ( angle_ );
	formTitlebar_->SetRotation ( angle_ );
}


void CWindow::SetBackgroundImage ( sf::Image* img )
{
	if ( img == NULL ) {
		sf::Image* emptyImg = new sf::Image;
		backgroundImage_ = emptyImg;
		background_.SetImage( *emptyImg );	
	} else {	
		backgroundImage_ = img;
		background_.SetImage( *img );
	}

	this->Update();
}


void CWindow::ShowBackground ( bool ison )
{
	showBackground_ = ison;
}


void CWindow::SetName ( std::string str )
{
	name_.SetText( str );
}


void CWindow::Rotate ( double angle )
{
	angle_ += angle;

	if ( angle_ >= 360 ) {
		angle_ = 0;
	} else if ( angle_ < 0 ) {
		angle_ = 359;
	}

	this->Update();
}


void CWindow::SetAngle ( double angle )
{
	angle_ = angle;
}


double CWindow::GetAngle()
{
	return angle_;
}


std::vector< gui::CWidget* >* CWindow::GetWidgetList()
{
	return &widgetList_;
}


gui::CWidget* CWindow::GetWidget ( int i )
{
	if ( widgetList_.size() <= 3 ) /* Ignore the titlebar buttons */
		return NULL;

	if ( i == -1 ) { /* last one */
		return *( widgetList_.end() - 1 );
	}

	if ( i < widgetList_.size() ) {
		return widgetList_[i];
	} else {
		return NULL;
	}
}


void CWindow::AdjustSize()
{
	int endX = 0;
	int endY = 0;
	
	std::vector< gui::CWidget* >::iterator endIter = widgetList_.end();
	std::vector< gui::CWidget* >::iterator iter = widgetList_.begin();
	for ( ; iter != endIter; ++iter ) {
		if ( ( *iter )->GetDimension().Right > endX )
			endX = ( *iter )->GetDimension().Right;
		
		if ( ( *iter )->GetDimension().Bottom > endY )
			endY = ( *iter )->GetDimension().Bottom;
	}
	
	this->SetSize ( sf::Vector2f ( endX + layoutBorder_, endY + layoutBorder_ ) );
}


void CWindow::SetSize ( sf::Vector2f size_, bool force )
{
	curSize_ = size_;
	sf::Vector2f minSize = minSize_;

	/* Egal welche min. Einstellungen es gibt,
	   ich soll sie ignorieren.. nagut... */
	if ( force ) {
		minSize = sf::Vector2f ( 0, 0 );
	}
	
	if ( !resizeAble_ )
		return;

	if ( curSize_.x < minSize.x ) {
		curSize_.x = minSize.x;
	}

	if ( curSize_.y < minSize.y ) {
		curSize_.y = minSize.y;
	}

	if ( curSize_.x > maxSize_.x ) {
		curSize_.x = maxSize_.x;
	}

	if ( curSize_.y > maxSize_.y ) {
		curSize_.y = maxSize_.y;
	}

	this->Update();
}


void CWindow::SetSizeInPercent ( sf::Vector2f sizePercent )
{
	this->SetSize ( sf::Vector2f ( settings::GetWidth() * sizePercent.x * 0.01, settings::GetHeight() * sizePercent.y * 0.01 ) );
}


sf::Vector2f CWindow::GetSize() const
{
	return curSize_;
}


void CWindow::SetPosition ( sf::Vector2f position )
{
	if ( position.x != -1 )
		position_.x = position.x;
	if ( position.y != -1 )
		position_.y = position.y;
	
	if ( !freeToSet_ ) {
		if ( position_.x + curSize_.x < 0 )
			position_.x = - curSize_.x + 10;

		if ( position_.x > settings::GetWidth() - 10 )
			position_.x = settings::GetWidth() - 10;
	
		if ( position_.y - titlebar_ < 0 )
			position_.y = titlebar_ + 1;

		if ( position_.y > settings::GetHeight() )
			position_.y = settings::GetHeight();
	}

	this->Update();
	this->UpdateWidgets();
}


void CWindow::SetPosition ( POSITION posX, POSITION posY )
{
	position_.x = 0; position_.y = 0; /* FIXME why I used "MovePosition"??? */
	
	switch ( posX ) {
		case LEFT:
			MovePosition ( HORIZONTAL, 0 );
			break;
		case CENTER:
			MovePosition ( HORIZONTAL, ( settings::GetWidth() - GetSize().x ) * 0.5f );
			break;
		case RIGHT:
			MovePosition ( HORIZONTAL, settings::GetWidth() - GetSize().x );
			break;
	}
	
	switch ( posY ) {
		case TOP:
			MovePosition ( VERTICAL, 0 );
			break;
		case CENTER:
			MovePosition ( VERTICAL, (settings::GetHeight() - GetSize().y) * 0.5f );
			break;
		case BOTTOM:
			MovePosition ( VERTICAL, settings::GetHeight() - GetSize().y );
			break;
	}
	
	this->Update();
}


void CWindow::MovePosition ( LAYOUT direction, int distance )
{
	if ( direction == HORIZONTAL ) {
		this->SetPosition ( sf::Vector2f ( GetPosition().x + distance, -1 ) );
	} else {
		this->SetPosition ( sf::Vector2f ( -1, GetPosition().y + distance ) );
	}
}


sf::Vector2f CWindow::GetPosition() const
{
	return position_;
}


int CWindow::GetTitlebarHeight()
{
	return titlebar_;
}


int CWindow::GetTitlebarLength()
{
	return titlebarLength_;
}


sf::Rect<float> CWindow::GetWindowDimension() const
{
	if ( formWin_ == NULL ) {
		return sf::Rect<float> ( 0, 0, 0, 0 );
	} else {
		sf::Vector2f pos = formWin_->TransformToGlobal( sf::Vector2f ( 0, 0 ) );
		sf::Vector2f posEnd = formWin_->TransformToGlobal( sf::Vector2f ( curSize_.x, curSize_.y ) );

		return sf::Rect<float> ( pos.x, pos.y - titlebar_, posEnd.x, posEnd.y );
	}

// 	return sf::Rect<float> ( position_.x, position_.y - titlebar_, position_.x + curSize_.x, position_.y + curSize_.y );
}


sf::Rect<float> CWindow::GetTitlebarDimension() const
{
	if ( formTitlebar_ ) {
		sf::Vector2f titlePos ( formTitlebar_->GetPosition().x, formTitlebar_->GetPosition().y - titlebar_ );
		sf::Vector2f titleEndPos = formTitlebar_->TransformToGlobal (sf::Vector2f(titlebarLength_,titlebar_) );

		return sf::Rect<float> ( titlePos.x, titlePos.y, titleEndPos.x + titlebar_, titleEndPos.y );
	}
}


sf::Rect<float> CWindow::GetResizeArea() const
{
	sf::Vector2f point = GetPosition() + GetSize() - sf::Vector2f ( 2, 2 );

	return sf::Rect<float> ( point.x, point.y, point.x + 3, point.y + 3 );
}


void CWindow::SetShow ( bool show )
{
	show_ = show;
	
	if ( show_ ) {
		GetGameClass()->GetGuiManager()->BringToFront ( this );
	}
}


bool CWindow::GetShow()
{
	return show_;
}



void CWindow::SetTitlebar ( unsigned int titlebar )
{
	/* TitlebarIcons-Anzeige nur ändern, falls nötig */
	if ( titlebar_ != !titlebar ) {
		bool showTitlebarIcons = true;

		/* Wenn Titlebar angezeigt werden soll, dann auch Icons anzeigen lassen */

		if ( titlebar ) {
			showTitlebarIcons = true;
		}

		/* Icons nicht anzeigen */
		else {
			showTitlebarIcons = false;
		}

		std::vector< gui::CWidget* >::iterator endIter = widgetList_.end();
		std::vector< gui::CWidget* >::iterator iter = widgetList_.begin();

		for ( ; iter != endIter; ++iter ) {
			if ( ( *iter )->GetName() == "close" || ( *iter )->GetName() == "minimize" ) {
				( *iter )->SetShow ( false );
			}
		}
	}

	titlebar_ = titlebar;
	
	if ( !titlebar_ )
		titlebarLength_ = 0;

	this->Update();
}


void CWindow::SetNoRoundTitlebar ( bool ison )
{
	noRoundTitlebar_ = ison;
}


void CWindow::SetMoveWindow( bool ison )
{
	moveWindow_ = ison;
}


bool CWindow::GetMoveWindow() const
{
	return moveWindow_;
}


void CWindow::SetMoveAble ( bool ison )
{
	moveAble_ = ison;
}


bool CWindow::GetMoveAble()
{
	return moveAble_;
}


void CWindow::SetCloseAble ( bool ison )
{
	closeAble_ = ison;
}


bool CWindow::GetCloseAble()
{
	return closeAble_;
}


void CWindow::SetResizeAble ( bool ison )
{
	resizeAble_ = ison;
}


bool CWindow::GetResizeAble()
{
	return resizeAble_;
}


void CWindow::SetLayout ( gui::LAYOUT layout, int layoutBorder )
{
	layout_ = layout;
	layoutBorder_ = layoutBorder; /* TODO Test if value is okay */
}


unsigned int CWindow::GetLayoutBorder()
{
	return layoutBorder_;
}


void CWindow::SetColor ( sf::Color color )
{
	backgroundColor_ = color;
	
	for ( int i = formWin_->GetNbPoints()-1; i >= 0; --i ) {		
		formWin_->SetPointColor ( i, color );
	}
}


void CWindow::ChangeTransparency ( unsigned int alpha )
{
	backgroundColor_.a = alpha;
	
	for ( int i = formWin_->GetNbPoints()-1; i >= 0; --i ) {
		sf::Color nColor = formWin_->GetPointColor ( i );
		nColor.a = alpha;
		
		formWin_->SetPointColor ( i, nColor );
	}
}


void CWindow::SetFreeToSet ( bool freeToSet )
{
	freeToSet_ = freeToSet;
}




} /* namespace gui */
