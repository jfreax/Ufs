/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "load.hpp"

#include "../other/label.hpp"
#include "../../game.hpp"

namespace gui
{
	
CLoadWindow::CLoadWindow()
{
	formWin_ = NULL;
	formLoadTop_ = NULL;
	formLoadBottom_ = NULL;
	
	/* No titlebar */
	this->SetTitlebar ( 0 );
	
	/* Change window properties */
	this->SetCloseAble ( false );
	this->SetResizeAble ( false );
	this->SetLayout ( VERTICAL, 5 );
	this->ShowBackground ( false );
	
	/* Add widgets */
	this->AddWidget ( new CLabel ( "Loading... please wait!", 18 ) );
	
	/* Set size and position (depend on buttons) */
	this->AdjustSize();
	this->SetPosition ( CENTER, CENTER );
	
	CalcBackground();
}


void CLoadWindow::Render()
{
	if ( !this->GetShow() )
		return;
	
	GetGameClass()->GetApp()->Draw( *formWin_ );
	GetGameClass()->GetApp()->Draw( *formLoadTop_ );
	GetGameClass()->GetApp()->Draw( *formLoadBottom_ );
	CWindow::Render();
	
	/* Adjust progressbar */
	formLoadTop_->SetScaleX( script::GetInitProgress()/100.f );
	formLoadBottom_->SetScaleX( script::GetInitProgress()/100.f );
	

}


void CLoadWindow::CalcBackground()
{
	delete formWin_;
	formWin_ = new sf::Shape();
	delete formLoadTop_;
	formLoadTop_ = new sf::Shape();
	delete formLoadBottom_;
	formLoadBottom_ = new sf::Shape();
	
	/* Variablen deklarieren */
	float Angle;
	float formRound = GetSize().x - titlebarLength_ > formRound_ ? formRound_ : GetSize().x - titlebarLength_;
	
	/* BACKGROUND 
	------------- */
	/* Top left corner */
	sf::Vector2f Center ( formRound_, formRound_ );
	for ( int i = 40; i < 60; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formWin_->AddPoint ( Center + Offset * formRound_, backgroundColor_ );
	}
	
	/* Top right */
	Center = sf::Vector2f ( GetSize().x - formRound, formRound );
	for ( int i = 60; i < 80; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formWin_->AddPoint ( Center + Offset * formRound, backgroundColor_ );
	}
	
	/* Bottom right */
	Center = sf::Vector2f ( GetSize().x - formRound_, GetSize().y - formRound_ );
	for ( int i = 0; i < 20; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formWin_->AddPoint ( Center + Offset * formRound_, backgroundColor_ );
	}
	
	/* Bottom left */
	Center = sf::Vector2f ( formRound_, GetSize().y - formRound_ );
	for ( int i = 20; i < 40; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formWin_->AddPoint ( Center + Offset * formRound_, backgroundColor_ );
	}
	
	/* BLUE B 
	--------- */
	sf::Color blue ( 0, 0, 200, 110 );
	
	/* Top left corner */
	formLoadTop_->AddPoint ( sf::Vector2f ( 0, GetSize().y * 0.5f ), blue );
	Center = sf::Vector2f ( formRound_, formRound_ );
	for ( int i = 40; i < 60; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formLoadTop_->AddPoint ( Center + Offset * formRound_, blue );
	}
	
	/* Top right */
	Center = sf::Vector2f ( GetSize().x - formRound, formRound );
	for ( int i = 60; i < 80; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formLoadTop_->AddPoint ( Center + Offset * formRound, blue );
	}
	formLoadTop_->AddPoint ( sf::Vector2f ( GetSize().x, GetSize().y * 0.5f ), blue );
	
	
	blue = sf::Color ( 0, 0, 200, 150 );
	
	/* Bottom right */
	formLoadBottom_->AddPoint ( sf::Vector2f ( GetSize().x, GetSize().y * 0.5f ), blue );
	Center = sf::Vector2f ( GetSize().x - formRound_, GetSize().y - formRound_ );
	for ( int i = 0; i < 20; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formLoadBottom_->AddPoint ( Center + Offset * formRound_, blue );
	}
	
	/* Bottom left */
	Center = sf::Vector2f ( formRound_, GetSize().y - formRound_ );
	for ( int i = 20; i < 40; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		formLoadBottom_->AddPoint ( Center + Offset * formRound_, blue );
	}
	formLoadBottom_->AddPoint ( sf::Vector2f ( 0, GetSize().y * 0.5f ), blue );
	
	
	formWin_->SetPosition ( GetPosition().x, GetPosition().y + GetSize().y );
	formLoadTop_->SetPosition ( GetPosition().x, GetPosition().y + GetSize().y );
	formLoadBottom_->SetPosition ( GetPosition().x, GetPosition().y + GetSize().y );
	
}

	
} /* namespace gui */
