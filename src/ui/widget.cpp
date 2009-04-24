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


CWidget::CWidget ( )
{
	static unsigned int globalId = 0;
	id_ = ++globalId;

	CTheme* theme = GetGameClass()->GetGuiManager()->GetTheme();

	motherWin_ = NULL;

	this->NoUpdate ( false );
	isMouseHere = wasMouseHere = false;
}


bool CWidget::Update ( void )
{
	if ( noUpdate_ || !motherWin_ )
	{
		return false;
	}

	position_.x = fakePosition_.x < 0 ? fakePosition_.x + motherWin_->GetSize().x : fakePosition_.x;
	position_.y = fakePosition_.y + motherWin_->GetTitlebarDimension().GetHeight() < 0 ? fakePosition_.y + motherWin_->GetSize().y : fakePosition_.y;

	/* Bild so setzen, das der Mittelpunkt auch wirklich in der Mitte ist */
	background_.SetCenter ( background_.GetSize() * 0.5f );
// 	position_ -= background_.GetCenter();
	

	if ( background_.GetSize().x != 1.f )
	{
		background_.SetPosition ( position_ + motherWin_->GetPosition() + background_.GetCenter() );
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
	// Funktionsanrufe tätigen
	this->Call();

	//
	if ( !isMouseHere && wasMouseHere )
	{
		this->onUnHoverMouse();
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


void CWidget::SetMotherWin ( gui::CWindow* win )
{
	motherWin_ = win;
}


void CWidget::SetPosition ( sf::Vector2f position )
{
	fakePosition_ = position;
	this->Update();
}


sf::Vector2f CWidget::GetPosition ( )
{
	return position_;
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
