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

#include <list>

#include "../game.hpp"
#include "../action.hpp"

#include "window.hpp"
#include "widget.hpp"

namespace gui
{


CWidget::CWidget ( CWindow* motherWin, sf::Vector2f position, sf::Vector2f size )
{
	static unsigned int globalId = 0;
	id_ = ++globalId;

	CTheme* theme = GetGameClass()->GetGuiManager()->GetTheme();

	motherWin_ = motherWin;
	motherWin_->AddWidget ( this );

	this->NoUpdate ( true );
	{
		this->SetPosition ( position );
		this->SetSize ( size );
	}

	this->NoUpdate ( false );

	isMouseHere = wasMouseHere = false;
}


bool CWidget::Update ( void )
{
	if ( noUpdate_ )
	{
		return false;
	}

	position_.x = fakePosition_.x < 0 ? fakePosition_.x + motherWin_->GetSize().x : fakePosition_.x;
	position_.y = fakePosition_.y + motherWin_->GetTitlebarDimension().GetHeight() < 0 ? fakePosition_.y + motherWin_->GetSize().y : fakePosition_.y;

	if ( background_.GetSize().x != 1.f )
	{
		background_.SetPosition ( position_ + motherWin_->GetPosition() );
		background_.Resize ( curSize_ );
	}
	else
	{
		form_ = sf::Shape::Rectangle ( position_, position_ + curSize_, backgroundColor_, border_, borderColor_ );
	}

	sf::Vector2f textPos ( ( curSize_.x - text_.GetRect().GetWidth() ) * 0.5f, ( curSize_.y - text_.GetRect().GetHeight() ) * 0.5f );

	text_.SetPosition ( position_ + motherWin_->GetPosition() + textPos );

	return true;
}


void CWidget::Calc ( void )
{
	// Funktionsanrufe t�tigen
	this->Call();

	//
	if ( !isMouseHere && wasMouseHere )
	{
		this->Mouse ( MOUSE::UNHOVER );
		wasMouseHere = false;
	}

	isMouseHere = false;
}



void CWidget::NoUpdate ( bool ison )
{
	noUpdate_ = ison;
	if ( !ison )
	{
		this->Update();
	}
}


bool CWidget::Call ( void )
{
	bool ret = true;

	std::list< Arguments_* >::iterator iter = callThis_.begin();
	for ( ; iter != callThis_.end(); ++iter )
	{
		if ( ( *iter )->delNow )
		{ // Dieses Objekt soll hier nicht mehr sein. Weg damit!
			Arguments_* delPtr = NULL;
			if ( ( *iter )->shouldDelete )
			{
				delPtr = ( *iter );
			}
			iter = callThis_.erase ( iter );
			delete delPtr;
		}
		else if ( ( *iter )->wait <= ( *iter )->timer.GetElapsedTime() )
		{  // Nur nach bestimmter Zeit starten!
			( *iter )->timer.Reset();
			if ( ! ( *iter )->function ( this, ( *iter )->args ) )
			{ // Die Anweisung sagt ich soll dich l�schen - schade f�r dich , aber was sein muss...
				Arguments_* delPtr = NULL;
				if ( ( *iter )->shouldDelete )
				{
					delPtr = ( *iter );
				}
				iter = callThis_.erase ( iter );
				delete delPtr;
			}
		}
	}

	return ret;
}


bool CWidget::Mouse ( MOUSE::TYPES type )
{
	std::vector< Arguments_* >* mouseArgs = NULL;

	switch ( type )
	{
		case MOUSE::LEFT:
			mouseArgs = &mouseLClick_;
			break;
		case MOUSE::RIGHT:
			mouseArgs = &mouseRClick_;
			break;
		case MOUSE::MIDDLE:
			mouseArgs = &mouseMClick_;
			break;
		case MOUSE::HOVER:
			mouseArgs = &mouseHover_;
			isMouseHere = wasMouseHere = true;
			break;
		case MOUSE::UNHOVER:
			mouseArgs = &mouseUnHover_;
			for ( std::vector< Arguments_* >::size_type i = mouseHover_.size(); i; --i )
			{ // Alle MouseHovers stoppen
				mouseHover_.at ( i - 1 )->delNow = true;
			}
			break;
	}

	if ( !mouseArgs )
	{
		return false;
	}


	bool ret = true;

	for ( std::vector< Arguments_* >::size_type i = mouseArgs->size(); i; --i )
	{
		if ( mouseArgs->at ( i - 1 )->wait <= mouseArgs->at ( i - 1 )->timer.GetElapsedTime() )
		{  // Nur nach bestimmter Zeit starten!
			mouseArgs->at ( i - 1 )->timer.Reset();

			if ( ! mouseArgs->at ( i - 1 )->function ( this, mouseArgs->at ( i - 1 )->args ) )
			{
				ret = false;
			}
			else
			{ // Aktion auch nach dem Klick weiter ausf�hren!
				callThis_.push_back ( mouseArgs->at ( i - 1 ) );
				ret = true;
			}
		}
	}

	return ret;
}


util::DataHolder* CWidget::AddCall ( bool ( *mouseClick ) ( CWidget*, util::DataHolder& ), float wait )
{
	Arguments_* newCall_ = new Arguments_;
	newCall_->function = mouseClick;
	newCall_->wait = wait;
	newCall_->shouldDelete = true;

	callThis_.push_back ( newCall_ );

	return &newCall_->args;
}


util::DataHolder* CWidget::AddMouseEvent ( bool ( *mouseClick ) ( CWidget*, util::DataHolder& ), MOUSE::TYPES type, float wait )
{
	Arguments_* newMouseAction = new Arguments_;
	newMouseAction->function = mouseClick;
	newMouseAction->wait = wait;
	newMouseAction->shouldDelete = false;

	switch ( type )
	{
		default:
		case MOUSE::LEFT:
			mouseLClick_.push_back ( newMouseAction );
			break;
		case MOUSE::RIGHT:
			mouseRClick_.push_back ( newMouseAction );
			break;
		case MOUSE::MIDDLE:
			mouseMClick_.push_back ( newMouseAction );
			break;
		case MOUSE::HOVER:
			mouseHover_.push_back ( newMouseAction );
			break;
		case MOUSE::UNHOVER:
			mouseUnHover_.push_back ( newMouseAction );
			break;
	}

	return &newMouseAction->args;
}


void CWidget::SetPosition ( sf::Vector2f position )
{
	fakePosition_ = position;
	this->Update();
}


void CWidget::SetSize ( sf::Vector2f size )
{
	curSize_ = size;
	this->Update();
}


void CWidget::SetName ( std::string name )
{
	name_ = name;
	text_.SetText ( name );

	this->Update();
}


void CWidget::SetFontSize ( int size )
{
	text_.SetSize ( size );
}


void CWidget::SetBackground ( sf::Sprite background )
{
	background_ = background;
}


sf::Sprite* CWidget::GetBackground ( void )
{
	return &background_;
}


void CWidget::SetBackground ( sf::Image* background )
{
	background_.SetImage ( *background );
}


void CWidget::SetBackgroundColor ( sf::Color color )
{
	backgroundColor_ = color;
}


sf::Color CWidget::GetBackgroundColor ( void )
{
	return backgroundColor_;
}


CWindow* CWidget::GetMotherWin ( void )
{
	return motherWin_;
}


sf::Rect< float > CWidget::GetDimensionInScreen ( void )
{
	sf::Vector2f motherPosition = motherWin_->GetPosition();
	return sf::Rect< float > ( motherPosition.x + position_.x, motherPosition.y + position_.y, motherPosition.x + position_.x + curSize_.x, motherPosition.y + position_.y + curSize_.y );
}


} // namespace gui
