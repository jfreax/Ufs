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

#include "tooltip.hpp"

#include "../../engine/ui/window.hpp"
#include "../other/label.hpp"
#include "../other/spacer.hpp"
#include "../../engine/ui/button.hpp"
#include "../button/quit.hpp"
#include "../../game.hpp"

namespace gui
{
	
	
CTooltip::CTooltip ( CWidget* motherWidget, std::string text ) :
motherWidget_ ( motherWidget ),
CWindow ( false )
{
	this->SetLayout ( HORIZONTAL, 5 );
		
	/* Text properties */
	label_ = this->AddWidget ( new CLabel ( text ) );
	label_->SetTextSize ( 12 );
	label_->AdjustSize();
		
	textColor_ = label_->GetText()->GetColor();
		
	this->SetTitlebar ( 0 );
// 	backgroundColor_ = sf::Color ( 200,220,0,0 ); /* TODO find a nice color */
	
	this->AdjustSize();
	
	this->AddWidget( new CSpacer ( VERTICAL, label_->GetText()->GetRect().GetHeight() + 3 ) );
	
	this->SetLayout ( HORIZONTAL, 3 );
}


CTooltip::~CTooltip()
{
	std::vector< gui::CWidget* >::iterator endIter = widgetList_.end();
	std::vector< gui::CWidget* >::iterator iter = widgetList_.begin();

	for ( ; iter != endIter; ++iter ) {
		delete ( *iter );
	}
	
	delete label_;
}
	
	
void CTooltip::Update()
{
	if ( motherWidget_ != NULL )
		this->SetPosition ( sf::Vector2f ( motherWidget_->GetDimensionInScreen().Left - ( motherWidget_->GetDimension().GetWidth() * 0.5f ),
						   motherWidget_->GetPosition().y + motherWidget_->GetDimension().GetHeight() + 2 ) );
}


void CTooltip::CalcBackground()
{
//     gui::CWindow::CalcBackground();

	/* alte Shapes löschen */
	delete formWin_;
	formWin_ = new sf::Shape();
	
	delete formWinBorder_;
	formWinBorder_ = new sf::Shape();
	
	delete formTitlebar_;
	formTitlebar_ = new sf::Shape();
	
	int x = 0;
	int y = 0;
	int height = this->GetSize().y;
	int length = this->GetSize().x;
	
	
	borderColor_ = sf::Color ( 180, 180, 180, 80 );
	formWin_->AddPoint( sf::Vector2f ( x              , y          ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x + length - 8, y          ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x + length     , y + 8     ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x + length     , y + height ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x + 8          , y + height ), borderColor_ );
	formWin_->AddPoint( sf::Vector2f ( x              , y + height - 8 ), borderColor_ );
	
	formWinBorder_->EnableFill( false );
	formWinBorder_->EnableOutline ( true );
	formWinBorder_->SetOutlineWidth( 1 );
	
	sf::Color yellow ( 120, 120, 0 );
	
	formWinBorder_->AddPoint( sf::Vector2f ( x              , y          ), borderColor_, yellow );
	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 8 , y          ), borderColor_, yellow );
	formWinBorder_->AddPoint( sf::Vector2f ( x + length     , y + 8      ), borderColor_, yellow );
	formWinBorder_->AddPoint( sf::Vector2f ( x + length     , y + height ), borderColor_, yellow );
	formWinBorder_->AddPoint( sf::Vector2f ( x + 8          , y + height ), borderColor_, yellow );
	formWinBorder_->AddPoint( sf::Vector2f ( x              , y + height - 8 ), borderColor_, yellow );
	
// 	formWinBorder_->AddPoint( sf::Vector2f ( x + 5          , y + 5           ), sf::Color ( 0, 0, 0, 190 ) );
// 	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 22, y + 5           ), sf::Color ( 0, 0, 0, 180 ) );
// 	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 5 , y + 22          ), sf::Color ( 0, 0, 0, 160 ) );
// 	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 5 , y + height - 20 ), sf::Color ( 0, 0, 0, 170 ) );
// 	formWinBorder_->AddPoint( sf::Vector2f ( x + length - 5 , y + height - 5  ), sf::Color ( 0, 0, 0, 160 ) );
// 	formWinBorder_->AddPoint( sf::Vector2f ( x + 20         , y + height - 5  ), sf::Color ( 0, 0, 0, 160 ) );
// 	formWinBorder_->AddPoint( sf::Vector2f ( x + 5          , y + height - 20 ), sf::Color ( 0, 0, 0, 180 ) );
	
	/* Positionen anpassen */
	formWin_->SetPosition ( GetPosition() );
	formWinBorder_->SetPosition ( GetPosition() );
}

	
	
void CTooltip::ChangeTransparency ( unsigned int alpha )
{
	CWindow::ChangeTransparency ( alpha );
	// 	backgroundColor_.a = alpha;
	textColor_.a = alpha;
	
	// 	for ( int i = formWin_->GetNbPoints()-1; i >= 0; --i ) {
		// 		sf::Color nColor = formWin_->GetPointColor ( i );
			// 		nColor.a = alpha;
			// 
			// 		formWin_->SetPointColor ( i, nColor );
			// 	}
			
		label_->GetText()->SetColor ( textColor_ );
}


void CTooltip::SetText ( std::string string )
{
	label_->SetText ( string );
}


} /* namespace gui */