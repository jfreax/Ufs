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

#include "../../collision.hpp"
#include "../../game.hpp"
#include "../../settings/settings.hpp"
#include "../../gui/window/terminal.hpp"

#include "manager.hpp"

namespace gui
{

void CManager::Initialize()
{
	theme_.Open ( settings::GetThemePath() + settings::GetTheme() + ".ini" );

	previousMouseScope_ = NONE;
	previousMousePos_.x = previousMousePos_.y = 0;
	
	terminal_ = NULL;
	textArea_ = NULL;
}


bool CManager::Render()
{
	std::vector<gui::CWindow*>::iterator iter = windowList_.begin();
	std::vector<gui::CWindow*>::iterator iterEnd = windowList_.end();

	for ( int i = 0; iter != iterEnd; ++iter ) {
		( *iter )->Render();
	}
	
	
	if ( GetGameClass()->GetGameType() != LOADING ) {
		GetGameClass()->GetMapManager()->GetCurrentSystem().GetTooltip()->Update();
		GetGameClass()->GetMapManager()->GetCurrentSystem().GetTooltip()->Render();
	}

	return true;
}


bool CManager::MouseClick ( const int x, const int y, const sf::Mouse::Button button )
{
	CGame* game = GetGameClass();
	MOUSESCOPE highestMouseScope = NONE;

	std::vector< CWindow* >::iterator clickedWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator lastWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator currentWindow = windowList_.begin();
	std::vector< CWindow* >::iterator iterBegin = windowList_.begin();


	if ( button == sf::Mouse::Left ) {
		for ( ; currentWindow != lastWindow+1; ++currentWindow ) {
			if ( ( *currentWindow )->GetShow() ) {
				/* Titlebar */
				if ( ( ( *currentWindow )->GetTitlebarDimension().Contains ( x, y ) &&
				                ( *currentWindow )->GetMoveAble() /*&&
			                previousMouseScope_ == NONE */) || previousMouseScope_ == TITLE ) {
					if ( currentWindow == lastWindow ) {
						
						previousMouseScope_ = TITLE;
						sf::Vector2f winPos = ( *currentWindow )->GetPosition();

						if ( !previousMousePos_.x && !previousMousePos_.y ) { // Maus wurde zuvor hier nicht geklickt
							previousMousePos_.x = x - winPos.x;
							previousMousePos_.y = y - winPos.y;
						}

						/* Move window */
						( *currentWindow )->SetPosition ( sf::Vector2f ( x - ( previousMousePos_.x ), y - ( previousMousePos_.y ) ) );
					}
				}

				/* Fenster vergrößern bzw verkleinern */
				else if ( ( *currentWindow )->GetResizeArea().Contains ( x, y ) && previousMouseScope_ == NONE || previousMouseScope_ == RESIZE ) {
					/* Nur letztes (im Vordergrundstehendes) Fenster */
					if ( currentWindow == lastWindow ) {
						previousMouseScope_ = RESIZE;
						sf::Vector2f winPos = ( *currentWindow )->GetPosition();

						if ( !previousMousePos_.x && !previousMousePos_.y ) {
							previousMousePos_.x = x - winPos.x;
							previousMousePos_.y = y - winPos.y;
						}

						( *currentWindow )->SetSize ( sf::Vector2f ( x - ( *currentWindow )->GetPosition().x, y - ( *currentWindow )->GetPosition().y ) );
					}
				}

				if ( ( *currentWindow )->GetWindowDimension().Contains ( x, y ) ||
				                ( *currentWindow )->GetTitlebarDimension().Contains ( x, y ) ) { // ## Fenster ##
					clickedWindow = currentWindow;

					if ( previousMouseScope_ == NONE ) {
						previousMouseScope_ = WINDOW;
					}
				}
			}
		}

	} else if ( button == sf::Mouse::Middle ) {
		GetGameClass()->GetMapManager()->Move ( sf::Vector2f ( x, y ) );
	} else if ( button == sf::Mouse::Right ) {

	}

	/* Change window render order */
	if ( clickedWindow != lastWindow && ( previousMouseScope_ == NONE || previousMouseScope_ == WINDOW ) ) {
		this->BringToFront ( clickedWindow );
		previousMouseScope_ = NONE;
		
		this->MouseClick ( x, y, button );
	}

	settings::SetMouseScope ( highestMouseScope );


	if ( previousMouseScope_ == NONE )
		return false;
	else
		return true;
}


bool CManager::MouseClickReleased ( const int x, const int y, const sf::Mouse::Button button )
{
	CGame* game = GetGameClass();
	MOUSESCOPE highestMouseScope = NONE;
	
	std::vector< CWindow* >::iterator clickedWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator lastWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator currentWindow = windowList_.begin();
	std::vector< CWindow* >::iterator iterBegin = windowList_.begin();

	if ( button == sf::Mouse::Left ) {
		for ( ;currentWindow != lastWindow+1; ++currentWindow ) {
			if ( ( *currentWindow )->GetShow() && ( *currentWindow )->GetWindowDimension().Contains ( x, y ) ||
			                ( *currentWindow )->GetTitlebarDimension().Contains ( x, y ) ) { // ## Fenster ##
				clickedWindow = currentWindow;

				if ( previousMouseScope_ == NONE ) {
					previousMouseScope_ = WINDOW;
				}


				if ( currentWindow == lastWindow ) {
					/* Widgets prüfen */
					std::vector< gui::CWidget* >* widgetList = ( *currentWindow )->GetWidgetList();
					gui::CWidget* currentWidget = NULL;

					for ( std::vector< gui::CWidget* >::size_type i = widgetList->size(); i; --i ) {
						currentWidget = widgetList->at ( i - 1 );

						/* Wenn Widget angezeigt werden soll, minimale Klickzeitabstand vorbei ist
						   und die Maus an der richtigen Position ist, dann Aktion ausführen */

						if ( currentWidget->GetShow() &&
// 						currentWidget->lastClickTime.GetElapsedTime() > .2f &&
						                currentWidget->GetDimensionInScreen().Contains ( x, y ) ) {
							previousMouseScope_ = NONE;

							currentWidget->lastClickTime.Reset();
							currentWidget->onLeftClick();

							return true;
						}
					}
				}
			}
		}
	} else if ( button == sf::Mouse::Middle ) {
		GetGameClass()->GetMapManager()->UnSetPos();
	} else if ( button == sf::Mouse::Right ) {

	}

	settings::SetMouseScope ( highestMouseScope );

	if ( previousMouseScope_ == NONE )
		return false;
	else
		return true;
}


bool CManager::MouseHover ( const int x, const int y )
{
	CGame* game = GetGameClass();

	previousMousePos_.x = previousMousePos_.y = 0;
	previousMouseScope_ = NONE;
	MOUSESCOPE highestMouseScope = NONE;


	std::vector< CWindow* >::iterator currentWindow = windowList_.end() - 1;
	std::vector< CWindow* >::iterator iterBegin = windowList_.begin();

	for ( ;currentWindow + 1 != iterBegin; --currentWindow ) {
		if ( ( *currentWindow )->GetWindowDimension().Contains ( x, y ) ||
		                ( *currentWindow )->GetTitlebarDimension().Contains ( x, y ) ) {
			highestMouseScope = WINDOW > highestMouseScope ? WINDOW : highestMouseScope;

			std::vector< gui::CWidget* >* widgetList = ( *currentWindow )->GetWidgetList();
			gui::CWidget* currentWidget = NULL;

			for ( std::vector< gui::CWidget* >::size_type i = widgetList->size(); i; --i ) {
				currentWidget = widgetList->at ( i - 1 );

				/* Wenn Widget angezeigt werden soll und die Maus an der richtigen Position ist, dann Aktion ausführen */

				if ( currentWidget->GetShow() && currentWidget->GetDimensionInScreen().Contains ( x, y ) ) {
					previousMouseScope_ = NONE;
					currentWidget->onHoverMouse();

				}
			}
		}
	}

	settings::SetMouseScope ( highestMouseScope );
}


CWindow* CManager::AddWindow ( CWindow* win )
{
	win->Initialize();
	windowList_.push_back ( win );
	return win;
}


bool CManager::CloseWindow ( gui::CWindow* window, bool DoNotFreeSpace )
{
	if ( !window && windowList_.size() ) {
		if ( this->GetActiveWindow()->GetCloseAble() ) {
			windowList_.pop_back();
			delete this->GetActiveWindow();
			return true;
		} else {
			this->CloseWindow ( this->GetPreviousWindow ( this->GetActiveWindow() ) );
		}
	} else {
		std::vector<gui::CWindow*>::iterator iter = windowList_.begin();

		for ( ; iter != windowList_.end(); ) {
			if ( ( *iter )->GetId() == window->GetId() ) {
				if ( window->GetCloseAble() ) {
					windowList_.erase ( iter );

					if ( !DoNotFreeSpace )
						delete window;

					return true;
				} else {
					return this->CloseWindow ( this->GetPreviousWindow ( window ) );
				}
			} else {
				++iter;
			}
		}
	}

	return false;
}


sf::Vector2f CManager::AddWindowToDock ( CWindow* win )
{
	sf::Vector2f positionOfLastWindow;

	/* Titelbargröße */
	int height = win->GetTitlebarHeight();
	int length = win->GetTitlebarLength() + height * 0.5f;

	/* Maximale Fenster pro Reihe */
	int max = ( settings::GetHeight() ) / length;

	/* Position des Fensters innerhalb des Docks ermitteln */
	int y = ( dockList_.size() % max ) * length + ( ( settings::GetHeight() ) % length * 0.5f );
	int x = ( dockList_.size() / max ) * height;

	/* Fenster ggf. ohne Rundung zeichnen */

	if ( x ) {
		std::list < CWindow* >::iterator endIter = dockList_.end();

		for ( int i = max; i; --i) {
			endIter--;
		}

		( *endIter )->SetNoRoundTitlebar( true );
		( *endIter )->Update();
	}

	/* Fenster ist nicht mehr verschiebbar! */
	win->SetMoveAble( false );

	/* Fenster der Docklist hinzufügen */
	dockList_.push_back ( win );

	/* Ergebnis zurückgeben */
	return sf::Vector2f ( x, y );

}


void CManager::RemoveWindowFromDock ( CWindow* win )
{
	/* Titelbargröße */
	int height = 15;
	int length = win->GetTitlebarDimension().GetWidth() - height * 0.5f;

	/* Maximale Fenster pro Reihe */
	int max = ( settings::GetHeight() ) / length;

	/* Fenster aus der Liste entfernen */
	dockList_.remove ( win );
	win->SetNoRoundTitlebar( false );
	win->SetMoveAble( true );

	/* Temporäre Liste erstellen und Daten übertragen */
	std::list < CWindow* > newList;
	newList.swap( dockList_ );

	/* Dockliste mit "neuen" Daten speisen und Positionen anpassen */
	sf::Vector2f newPosition;
	std::list < CWindow* >::iterator iter = newList.begin();
	std::list < CWindow* >::iterator iterEnd = newList.end();

	for ( ; iter != iterEnd; ++iter ) {
		( *iter )->SetNoRoundTitlebar( false );
		newPosition = AddWindowToDock ( ( *iter ) );

		/* Position */
		( *iter )->SetPosition ( newPosition );
	}


}


void CManager::BringToFront ( std::vector< CWindow* >::iterator iter )
{
	CWindow* tmp = *iter;

	windowList_.erase ( iter );
	windowList_.push_back ( tmp );
}


void CManager::BringToFront ( CWindow* win )
{
	if ( win == NULL )
		return;

	std::vector<gui::CWindow*>::iterator iter = windowList_.begin();

	std::vector<gui::CWindow*>::iterator iterEnd = windowList_.end();

	for ( int i = 0; iter != iterEnd; ++iter ) {
		if ( ( *iter )->GetId() == win->GetId() ) {
			windowList_.erase ( iter );
			windowList_.push_back ( win );
			return;
		}
	}
}



CTheme* CManager::GetTheme()
{
	return &theme_;
}


CWindow* CManager::GetActiveWindow()
{
	return windowList_[windowList_.size()-1];
}


CWindow* CManager::GetPreviousWindow ( CWindow* window )
{
	std::vector<gui::CWindow*>::iterator iter = ++windowList_.begin();

	for ( ; iter != windowList_.end(); ) {
		if ( ( *iter )->GetId() == window->GetId() ) {
			return *(--iter);
		} else {
			++iter;
		}
	}
}


void CManager::FocusTextArea ( CTextArea* textArea )
{
	textArea_ = textArea;
}


CTextArea* CManager::GetTextArea()
{
	return textArea_;
}



void CManager::RegisterTerminal ( CWindow* terminal )
{
	terminal_ = terminal;
}


void CManager::ToogleTerminal()
{
	if ( !terminal_ )
		return;
	
	dynamic_cast< CTerminalWindow* > ( terminal_ )->ToggleShow ();
}



} // namespace gui

