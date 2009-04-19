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

namespace gui
{

CWindow::CWindow ( CTheme *theme, sf::Vector2f position, sf::Vector2f size )
{
	static unsigned int globalId = 0;
	id_ = ++globalId;

	CGame* game = GetGameClass();

	minSize_ = theme->window_.minSize;
	maxSize_ = theme->window_.maxSize;

	background_.SetImage ( *game->GetImgResource()->Get ( settings::GetThemePath() + theme->window_.background ) );
	backgroundColor_ = theme->window_.backgroundColor;

	border_ = theme->window_.border;
	borderColor_ = theme->window_.borderColor;

	{
		titlebar_ = theme->window_.titlebar;
		titlebarColor_ = theme->window_.titlebarColor;

		sf::Vector2f closeButtonPosition ( theme->window_.iconClose.x, theme->window_.iconClose.y - titlebar_ );
		sf::Vector2f closeButtonSize ( theme->window_.iconClose.z, theme->window_.iconClose.z );

		CButton* closeButton = new CButton ( this, closeButtonPosition, closeButtonSize );
		closeButton->SetBackground ( *game->GetImgResource()->Get ( settings::GetThemePath() + "close.png" ) );
		closeButton->AddMouseEvent ( &action::closeWindow, MOUSE::LEFT );
	} // End Titlebar


	closeAble_ = true;
	moveAble_ = true;
	resizeAble_ = true;

	this->SetPosition ( position );
	this->SetSize ( size );

	this->Update();
}


const unsigned int CWindow::GetId ( void ) const
{
	return id_;
}


void CWindow::Update ( void )
{
	sf::Vector2f titlebarPosition ( position_.x, position_.y - titlebar_ );
	sf::Vector2f titlebarEndPosition ( position_.x + curSize_.x, position_.y );
	formTitlebar_ = sf::Shape::Rectangle ( titlebarPosition, titlebarEndPosition, titlebarColor_ );

	// Hintergrundbild / -Shape
	if ( background_.GetSize().x != 1.f )
	{
		background_.SetPosition ( position_ );
		background_.Resize ( curSize_ );
	}
	else
	{
		formWin_ = sf::Shape::Rectangle ( position_, position_ + curSize_, backgroundColor_, border_, borderColor_ );
	}

	// Inhalte ebenfalls aktualisieren
	for ( std::vector<gui::CWidget*>::size_type i = widgetList_.size(); i; --i )
	{
		widgetList_.at ( i - 1 )->Update();
	}
}


bool CWindow::Render ( void )
{
	sf::RenderWindow* app = GetGameClass()->GetApp();

	// Titlebar
	app->Draw ( formTitlebar_ );


	if ( background_.GetSize().x && background_.GetSize().x != 1.f )
	{
		app->Draw ( background_ );
	}
	else
	{
		app->Draw ( formWin_ );
	}

	for ( std::vector<gui::CWidget*>::size_type i = widgetList_.size(); i; --i )
	{
		widgetList_[i-1]->Render();
	}
}


void CWindow::AddWidget ( CWidget* widget_ )
{
	widgetList_.push_back ( widget_ );
}


std::vector< gui::CWidget* >* CWindow::GetWidgetList ( void )
{
	return &widgetList_;
}


void CWindow::SetSize ( sf::Vector2f size_ )
{
	curSize_ = size_;

	if ( curSize_.x < minSize_.x )
	{
		curSize_.x = minSize_.x;
	}
	if ( curSize_.y < minSize_.y )
	{
		curSize_.y = minSize_.y;
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
// 


} // namespace gui
