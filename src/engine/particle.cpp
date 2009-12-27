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

#include "particle.hpp"
#include "../game.hpp"
#include "../util/util.hpp"


#define sin_d(x)  (sin((x)*M_PI/180))
#define cos_d(x)  (cos((x)*M_PI/180))


////////////////////////////////////////////////////////////
/// Konstruktor
////////////////////////////////////////////////////////////
CParticleManager::CParticleManager ( int Type )
{
	type = Type;
	material = NULL;
	dimension = sf::Vector2i ( 400, 400 );

	// Variablen für kurzzeitige Berechnungen
	sf::FloatRect texCoords ( 0, 0, 0, 1 );
	int halfWidth = this->GetDimension().x * 0.5;
	int halfHight = this->GetDimension().y * 0.5;

	// Displaylist erstellen
	glParticle = glGenLists ( 1 );
	glNewList ( glParticle, GL_COMPILE );

	glBegin ( GL_QUADS );
	{
		glTexCoord2f ( texCoords.Left,  texCoords.Top );
		glVertex2f ( -halfWidth, -halfHight );
		
		glTexCoord2f ( texCoords.Left,  texCoords.Bottom );
		glVertex2f ( -halfWidth,  halfHight );
		
		glTexCoord2f ( texCoords.Right, texCoords.Bottom );
		glVertex2f ( halfWidth,  halfHight );
		
		glTexCoord2f ( texCoords.Right, texCoords.Top );
		glVertex2f ( halfWidth, -halfHight );
	}
	glEnd();

	glEndList();

// 	glDeleteLists();
}


////////////////////////////////////////////////////////////
/// Neue Positionen berechnen
////////////////////////////////////////////////////////////
bool CParticleManager::Calc ( std::list <class CEmitter>::iterator EmIter )
{
	CParticle *PartTmp = EmIter->GetParticle();

	sf::Vector3f partDirection;

	while ( PartTmp ) {
		/* "Leben" abziehen */
		if ( !PartTmp->setDelta_Life ( -GetGameClass()->GetApp()->GetFrameTime() ) )
			PartTmp->Reset ( &*EmIter );

		/* Farbe anpassen */
		double fade = PartTmp->GetLife() / EmIter->GetLife();
		switch ( type ) {
			case 1:
// 				PartTmp->SetColor ( sf::Color ( fade * 150.f, fade * 100.f, 0, fade * 250.f ) );
				break;
			case 2:
// 				PartTmp->SetColor ( sf::Color ( fade * 100.f, fade * 10.f, fade * 250.f, fade * 255.f ) );
				break;
			default:
				break;
		}

		/* Position und Geschwindigkeit bestimmen */
		partDirection = PartTmp->GetDirection();
		PartTmp->SetDelta_Position ( sf::Vector3f ( partDirection.x, partDirection.y, PartTmp->GetDirection().z ) * PartTmp->GetVelocity() * GetGameClass()->GetApp()->GetFrameTime() );
		
		PartTmp = PartTmp->Next;
	}

	return true;
}


void CParticleManager::Update ( void )
{
	for ( std::list <class CEmitter>::iterator iter = emitter.begin(); iter != emitter.end(); ++iter )
		Calc ( iter );
}


////////////////////////////////////////////////////////////
/// All particle draw them self
////////////////////////////////////////////////////////////
bool CParticleManager::Draw ( ) const
{
	if ( this->GetMaterial() )
		this->GetMaterial()->Bind();

	glEnable ( GL_BLEND );
	glDisable ( GL_DEPTH_TEST );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE );

	for ( std::list <class CEmitter>::const_iterator iter = emitter.begin(); iter != emitter.end(); ++iter ) {
		// Partikel zeichnen
		CParticle *PartTmp = iter->GetParticle();
		while ( PartTmp ) {
			glPushMatrix();
			{
				sf::Vector3f pos = PartTmp->GetPosition();
				sf::Color col = PartTmp->GetColor();

				glTranslatef ( pos.x, pos.y, 0 );
				glRotatef ( 0, 0, 0, 1 );
// 				glScalef ( pos.z, pos.z, pos.z );
				glColor4ub ( col.r, col.g, col.b, col.a );

				glCallList ( glParticle );
			}
			glPopMatrix();
			
			PartTmp = PartTmp->Next;
		}

	}

}

////////////////////////////////////////////////////////////
/// Einen Emitter ( Punkt an dem die Partikel ausgestoßen werden ) hinzufügen
////////////////////////////////////////////////////////////
bool CParticleManager::AddEmitter ( class CEmitter nEmit )
{
	emitter.push_back ( nEmit );

	return true;
}


std::list< class CEmitter >* CParticleManager::GetEmitter()
{
		return &emitter;
}


sf::Image* CParticleManager::GetMaterial() const
{
	return material;
}


void CParticleManager::SetMaterial ( sf::Image* value )
{
	material = value;
}


sf::Vector2i CParticleManager::GetDimension() const
{
	return dimension;
}


void CParticleManager::SetDimension ( const sf::Vector2i& value )
{
	dimension = value;
}


unsigned int CParticleManager::GetType() const
{
	return type;
}


void CParticleManager::SetType ( unsigned int value )
{
	type = value;
}



////////////////////////////////////////////////////////////
/// Konstruktor
////////////////////////////////////////////////////////////
CEmitter::CEmitter ( unsigned int numberOfParticles )
{
	particle = NULL;

	/* Standardwerte */
	this->SetPosition ( sf::Vector3f ( 0, 0, 0 ) );
	minVelocity = 5.f; // TODO
	maxVelocity = 10.f; // TODO
	this->SetDirection ( sf::Vector3f ( 10.f, 0.f, 0.f ) );
	this->SetEmitArea ( sf::Vector3i ( 0, 0, 0 ) );
	
	/* Emitter mit Partikeln "füttern" */
	this->Feed ( numberOfParticles );
}


void CEmitter::Feed ( unsigned int numberOfParticles )
{
	for ( unsigned int i = 0; i < numberOfParticles; ++i )
		new CParticle ( this );
}


sf::Vector3f CEmitter::GetPosition() const
{
	return position;
}


void CEmitter::SetPosition ( const sf::Vector3f& value )
{
	position = value;
}


float CEmitter::GetMinVelocity() const
{
	return minVelocity;
}


float CEmitter::GetMaxVelocity() const
{
	return maxVelocity;
}


void CEmitter::SetVelocity ( const float& valueMin, const float& valueMax )
{
	maxVelocity = valueMin;
	minVelocity = valueMax;
}


sf::Vector3f CEmitter::GetDirection() const
{
	return direction;
}


void CEmitter::SetDirection ( const int angle, const double& factor )
{
	direction = sf::Vector3f ( cos_d ( angle ) * factor, sin_d ( angle ) * factor, direction.z );
}


void CEmitter::SetDirection ( const sf::Vector3f& value )
{
	direction = value;
}


sf::Vector3f CEmitter::GetGrafity() const
{
	return grafity;
}


void CEmitter::SetGrafity ( const sf::Vector3f& value )
{
	grafity = value;
}


double CEmitter::GetLife() const
{
	return life;
}


void CEmitter::SetLife ( double value )
{
	life = value;
}


class CParticle* CEmitter::GetParticle() const
{
	return particle;
}


void CEmitter::SetParticle ( class CParticle* value )
{
	particle = value;
}


sf::Vector3i CEmitter::GetEmitArea() const
{
	return emitArea;
}


void CEmitter::SetEmitArea ( const sf::Vector3i& value )
{
	emitArea = value;
}



////////////////////////////////////////////////////////////
/// Konstruktor
////////////////////////////////////////////////////////////
CParticle::CParticle ( CEmitter *Anker )
{
	// In die Liste einreihen
	if ( Anker->GetParticle() != NULL )
		Next = Anker->GetParticle();
	else
		Next = NULL;

	Anker->SetParticle ( this );

	// Anfangseigenschaften setzen
	this->Reset ( Anker );
	this->SetLife( 0 );
}


void CParticle::Reset ( CEmitter *Anker )
{
	// Lebenszeit
	this->SetLife ( util::random ( Anker->GetLife() *.5f, Anker->GetLife() ) );

	// Geschwindigkeit
	this->SetVelocity ( util::random ( Anker->GetMinVelocity(), Anker->GetMaxVelocity() ) );

	// Richtung
	this->SetDirection ( Anker->GetDirection() );

	// Drehung der Texture
	this->SetRotation ( util::random ( 0, 360 ) );

	// Flächengröße an dem die Partikel entstehen
	sf::Vector3f area ( util::random ( Anker->GetEmitArea().x ), util::random ( Anker->GetEmitArea().y ), util::random ( Anker->GetEmitArea().z ) );
	this->SetPosition ( Anker->GetPosition() + area );

	color = sf::Color ( 200, 0, 150, 200 );
}


sf::Vector3f CParticle::GetPosition() const
{
	return position;
}


void CParticle::SetPosition ( const sf::Vector3f& value )
{
	position = value;
}


void CParticle::SetDelta_Position ( const sf::Vector3f& value )
{
	position += value;
}



sf::Color CParticle::GetColor() const
{
	return color;
}


void CParticle::SetColor ( const sf::Color& value )
{
	color = value;
}


double CParticle::GetLife() const
{
	return life;
}


bool CParticle::SetLife ( double value )
{
	life = value;

	if ( life <= 0.f )
		return false;

	return true;
}


bool CParticle::setDelta_Life ( double value )
{
	life += value;

	if ( life <= 0.f )
		return false;

	return true;
}


sf::Vector3f CParticle::GetDirection() const
{
	return direction;
}


void CParticle::SetDirection ( const sf::Vector3f& value )
{
	direction = value;
}


float CParticle::GetVelocity() const
{
	return velocity;
}


void CParticle::SetVelocity ( float value )
{
	velocity = value;
}



int CParticle::GetRotation() const
{
	return rotation;
}


void CParticle::SetRotation ( int value )
{
	rotation = value;
}
