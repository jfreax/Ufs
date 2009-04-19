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

#include "theme.hpp"
#include "widget.hpp"

#include "../collision.hpp"
#include "../game.hpp"
#include "../settings.hpp"

#include "manager.hpp"

namespace gui
{

CManager::CManager ( void )
{
	theme_.Open ( settings::GetThemePath() + settings::GetTheme() + ".ini" );

	previousMouseScope_ = NONE;
	previousMousePos_.x = previousMousePos_.y = 0;
}


bool CManager::Render ( void )
{
	std::vector<gui::CWindow*>::iterator iter = windowList_.begin();
	std::vector<gui::CWindow*>::iterator iterEnd = windowList_.end();

	for ( int i = 0; iter != iterEnd; ++iter )
	{
		( *iter )->Render();
	}

	return true;
}


bool CManager::MouseClick ( const int x, const int y, const sf::Mouse::Button button )
{
	CGame* game = GetGameClass();
	MOUSESCOPE highestMouseScope = NONE;

	std::vector< CWindow* >::iterator clickedWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator lastWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator currentWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator iterBegin = windowList_.begin();

	for ( ;currentWindow + 1 != iterBegin; --currentWindow )
	{
		if ( ( ( *currentWindow )->GetTitlebarDimension().Contains ( x, y ) && previousMouseScope_ == NONE ) || previousMouseScope_ == TITLE )
		{ // ## Titelbar ##
			if ( currentWindow == lastWindow )
			{
				previousMouseScope_ = TITLE;
				sf::Vector2f winPos = ( *currentWindow )->GetPosition();

				if ( !previousMousePos_.x && !previousMousePos_.y )
				{ // Maus wurde zuvor hier nicht geklickt
					previousMousePos_.x = x - winPos.x;
					previousMousePos_.y = y - winPos.y;
				}

				// verschieben
				( *currentWindow )->SetPosition ( sf::Vector2f ( x - ( previousMousePos_.x ), y - ( previousMousePos_.y ) ) );
			}
		}
		else if ( ( *currentWindow )->GetResizeArea().Contains ( x, y ) && previousMouseScope_ == NONE || previousMouseScope_ == RESIZE )
		{ // ## vergrößern/verkleinern ##
			if ( currentWindow == lastWindow )
			{ // nur letztes (im Vordergrundstehendes) Fenster
				previousMouseScope_ = RESIZE;
				sf::Vector2f winPos = ( *currentWindow )->GetPosition();

				if ( !previousMousePos_.x && !previousMousePos_.y )
				{
					previousMousePos_.x = x - winPos.x;
					previousMousePos_.y = y - winPos.y;
				}

				( *currentWindow )->SetSize ( sf::Vector2f ( x - ( *currentWindow )->GetPosition().x, y - ( *currentWindow )->GetPosition().y ) );
			}
		}

		if ( ( *currentWindow )->GetWindowDimension().Contains ( x, y ) )
		{ // ## Fenster ##
			clickedWindow = currentWindow;
			if ( previousMouseScope_ == NONE )
				previousMouseScope_ = WINDOW;

			if ( currentWindow == lastWindow )
			{ // Widgets prüfen
				std::vector< gui::CWidget* >* widgetList = ( *currentWindow )->GetWidgetList();
				gui::CWidget* currentWidget = NULL;

				for ( std::vector< gui::CWidget* >::size_type i = widgetList->size(); i; --i )
				{
					currentWidget = widgetList->at ( i - 1 );

					if ( currentWidget->GetDimensionInScreen().Contains ( x, y ) )
					{
						previousMouseScope_ = NONE;
						currentWidget->Mouse ( MOUSE::sfMouseToMouse ( button ) );
					}
				}
			}
		}
		else
		{
		}
	}

	if ( clickedWindow != lastWindow && ( previousMouseScope_ == NONE || previousMouseScope_ == WINDOW ) )
	{
		this->BringToFront ( clickedWindow );
		previousMouseScope_ = NONE;
	}

	settings::SetMouseScope ( highestMouseScope );

	return false;
}


bool CManager::MouseHover ( const int x, const int y )
{
	CGame* game = GetGameClass();

	previousMousePos_.x = previousMousePos_.y = 0;
	previousMouseScope_ = NONE;
	MOUSESCOPE highestMouseScope = NONE;


	std::vector< CWindow* >::iterator currentWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator iterBegin = windowList_.begin();

	for ( ;currentWindow + 1 != iterBegin; --currentWindow )
	{
		if ( ( *currentWindow )->GetWindowDimension().Contains ( x, y ) )
		{
			highestMouseScope = WINDOW > highestMouseScope ? WINDOW : highestMouseScope;

			std::vector< gui::CWidget* >* widgetList = ( *currentWindow )->GetWidgetList();
			gui::CWidget* currentWidget = NULL;

			for ( std::vector< gui::CWidget* >::size_type i = widgetList->size(); i; --i )
			{
				currentWidget = widgetList->at ( i - 1 );

				if ( currentWidget->GetDimensionInScreen().Contains ( x, y ) )
				{
					previousMouseScope_ = NONE;
					currentWidget->Mouse ( MOUSE::HOVER );
				}
			}
		}
	}


	settings::SetMouseScope ( highestMouseScope );
}


gui::CWindow* CManager::NewWindow ( sf::Vector2f position, sf::Vector2f size )
{
	gui::CWindow* win = new gui::CWindow ( &theme_, position, size );
	windowList_.push_back ( win );

	return win;
}


bool CManager::CloseWindow ( gui::CWindow* window )
{
	if ( !window && windowList_.size() )
	{
		delete this->GetActiveWindow();

		windowList_.pop_back();
	}
	else
	{
		std::vector<gui::CWindow*>::iterator iter = windowList_.begin();

		for ( ; iter != windowList_.end(); )
		{
			if ( ( *iter )->GetId() == window->GetId() )
			{
				gui::CWindow* winDelete = *iter;
				windowList_.erase ( iter );
				delete winDelete;
			}
			else
			{
				++iter;
			}
		}
	}
}


bool CManager::BringToFront ( std::vector< CWindow* >::iterator iter )
{
	CWindow* tmp = *iter;

	windowList_.erase ( iter );
	windowList_.push_back ( ( tmp ) );
}


CTheme* CManager::GetTheme ( void )
{
	return &theme_;
}


gui::CWindow* CManager::GetActiveWindow ( void )
{
	return windowList_[windowList_.size()-1];
}



} // namespace gui

