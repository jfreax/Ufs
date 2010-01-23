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

#include "select.hpp"
#include "../../gui/other/select.hpp"


namespace gui
{


CSelectWindow::CSelectWindow()
{
	CWindow::CWindow();
	
	/* No titlebar */
	this->SetTitlebar ( 0 );

	/* Add widgets */
	this->AddWidget ( new gui::CSelectWidget )->GetDimension().GetHeight();
	
	/* Change window properties */
	this->AdjustSize();
	this->SetPosition ( LEFT, BOTTOM );
	this->SetMoveAble ( false );
	this->SetCloseAble ( false );
	this->SetResizeAble ( false );
	this->SetTitlebar( false );

}


void CSelectWindow::CalcBackground()
{
	/* alte Shapes löschen */
	delete formWin_;
	formWin_ = new sf::Shape();
	
	delete formWinBorder_;
	formWinBorder_ = new sf::Shape();
	
	delete formTitlebar_;
	formTitlebar_ = new sf::Shape();
	
	int x = this->GetPosition().x;
	int y = this->GetPosition().y;
	int height = this->GetSize().y;
	int length = this->GetSize().y;
	
	borderColor_ = sf::Color ( 180, 180, 180, 80 );
	formWin_->AddPoint( sf::Vector2f ( x              , y          ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x + length - 20, y          ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x + length     , y + 20     ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x + length     , y + height ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x              , y + height ), borderColor_ );
// 	formWin_->AddPoint( sf::Vector2f ( x              , y + height - 20 ), borderColor_ );
	
	formWinBorder_->AddPoint( sf::Vector2f ( x + 5          , y + 5           ), sf::Color ( 0, 0, 0, 190 ) );
	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 22, y + 5           ), sf::Color ( 0, 0, 0, 180 ) );
	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 5 , y + 22          ), sf::Color ( 0, 0, 0, 160 ) );
	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 5 , y + height - 20 ), sf::Color ( 0, 0, 0, 170 ) );
	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 5 , y + height - 5  ), sf::Color ( 0, 0, 0, 160 ) );
	formWinBorder_->AddPoint( sf::Vector2f ( x + 20         , y + height - 5  ), sf::Color ( 0, 0, 0, 160 ) );
	formWinBorder_->AddPoint( sf::Vector2f ( x + 5          , y + height - 20 ), sf::Color ( 0, 0, 0, 180 ) );
}


} /* namespace gui */
