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

#include "../../game.hpp"
#include "select.hpp"


namespace gui
{


CSelectWidget::CSelectWidget()
{
	this->SetSize( sf::Vector2f ( 200, 200 ) );
	
	spriteList_ = &GetGameClass()->GetMapManager()->GetSelectedSprites();
	wasInitialized_ = false;
	needUpdate_ = true;
}


CSelectWidget::~CSelectWidget()
{
	delete player_;
	
	/* Deregister widget */
	GetGameClass()->GetMapManager()->SetSpecialWidget ( "MINI_OBJECT", NULL );
}



void CSelectWidget::Initialize()
{
	wasInitialized_ = true;
	
	/* Register widget */
	GetGameClass()->GetMapManager()->SetSpecialWidget ( "MINI_OBJECT", this );

	/* Prepare the positions */
	player_ = dynamic_cast < CLabel* > ( GetMotherWin()->AddWidget ( new CLabel ) );
	player_->SetPosition ( sf::Vector2f ( 10, -50 ) );
	player_->SetShow ( false );
}



void CSelectWidget::Render()
{
	/* Call one time! */
	if ( !wasInitialized_ )
		this->Initialize();
	
	sf::RenderWindow* app = GetGameClass()->GetApp();
	
	gui::CWidget::Render();
	this->Calc();
	
// 	std::vector < sf::Sprite >::iterator iter = toDraw_.begin();
// 	std::vector < sf::Sprite >::iterator iterEnd = toDraw_.end();
// 	
// 	for ( ; iter != iterEnd ; ++iter ) {
// 		app->Draw ( *iter );
// 	}
	
}


bool CSelectWidget::Call()
{
	needUpdate_ = true;
	
	return true;
}


/* ------- PRIVATE METHODS ------- */


void CSelectWidget::Calc()
{
	gui::CWidget::Calc();
	
	if ( needUpdate_ ) {
		needUpdate_ = false;
	
		/* Clear old list */
// 		toDraw_.clear();
		images_.clear();
		
		/* No sprite is marked */
		if ( spriteList_->empty() ) {
			player_->SetShow ( false );
			
		/* If one object is marked */
		} else if ( spriteList_->size() == 1 ) {
// 			if ( spriteList_->at ( 0 )->GetMiniImage() != NULL ) {
				sprite::CSprite* sprite = *spriteList_->begin();
				
// 				sf::Sprite miniImg ( *sprite->GetMiniImage(), sf::Vector2f ( GetPositionInScreen().x+6, GetPositionInScreen().y ) );
				
// 				float verh = (float)miniImg.GetImage()->GetWidth() / (float)miniImg.GetImage()->GetWidth();	
// 				float size = this->GetMotherWin()->GetWindowDimension().GetWidth() / ( miniImg.GetImage()->GetWidth() + 20 );	
// 				miniImg.SetScale ( size, size*verh );
				
				/* Set infos about the object */
				player_->SetShow();
				player_->SetText ( "Player: " + util::lCast< std::string > ( sprite->GetPlayer() ) );
// 				Data daTmp;
// 				daTmp.player.SetPosition ( sf::Vector2f ( GetPositionInScreen().x, GetPositionInScreen(). ) );
// 				daTmp.playerID = sprite->GetPlayer();
// 				daTmp.player = sf::String ( util::lCast< std::string > ( sprite->GetPlayer() ) );
// // 				daTmp.health
// 			
// 				data_.push_back ( daTmp );
// 				toDraw_.push_back ( miniImg );
// 			}
		
		/* Or more than one are marked */
		} else if ( !spriteList_->empty() ) {
			
		}
	}
}





} /* namespace gui */