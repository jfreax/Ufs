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

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <list>
#include <SFML/Graphics.hpp>


////////////////////////////////////////////////////////////
/// Einfaches Partikelsystem
////////////////////////////////////////////////////////////
class CParticleManager
{
	public:
		////////////////////////////////////////////////////////////
		/// Konstruktor
		////////////////////////////////////////////////////////////
		CParticleManager ( int Type = 2 );

		////////////////////////////////////////////////////////////
		/// Neue Positionen berechnen
		////////////////////////////////////////////////////////////
		bool Calc ( std::list <class CEmitter>::iterator EmIter );
		
		void Update ( void );

		////////////////////////////////////////////////////////////
		/// Alle Partikel zeichnen ( ruft this->Calc() selbständig auf! )
		////////////////////////////////////////////////////////////
		bool Draw ( ) const;

		////////////////////////////////////////////////////////////
		/// Einen Emitter ( Punkt an dem die Partikel ausgestoßen werden ) hinzufügen
		////////////////////////////////////////////////////////////
		bool AddEmitter ( class CEmitter nEmit );

		std::list< class CEmitter >* GetEmitter();

		void SetMaterial ( sf::Image* value );
		sf::Image* GetMaterial() const;

		void SetDimension ( const sf::Vector2i& value );
		sf::Vector2i GetDimension() const;

		void SetType ( unsigned int value );
		unsigned int GetType() const;
		
	private:
		std::list < class CEmitter > emitter;

		// Texture der Partikel (optional)
		sf::Image* material;

		// Größe ein einzelnen Partikels
		sf::Vector2i dimension;

		// 1 = Feuer
		unsigned int type;

		GLuint glParticle;
};


////////////////////////////////////////////////////////////
/// Ein Emitter
////////////////////////////////////////////////////////////
class CEmitter
{
	public:
		////////////////////////////////////////////////////////////
		/// Konstruktor
		////////////////////////////////////////////////////////////
		CEmitter ( unsigned int numberOfParticles = 0 );
		
		void Feed ( unsigned int numberOfParticles );

		void SetPosition ( const sf::Vector3f& value );
		sf::Vector3f GetPosition() const;

		void SetVelocity ( const float& valueMin, const float& valueMax );
		float GetMinVelocity() const;
		float GetMaxVelocity() const;

		void SetDirection ( const int angle, const double& factor );
		void SetDirection ( const sf::Vector3f& value );
		sf::Vector3f GetDirection() const;

		void SetGrafity ( const sf::Vector3f& value );
		sf::Vector3f GetGrafity() const;

		void SetLife ( double value );
		double GetLife() const;

		void SetParticle ( class CParticle* value );
		class CParticle* GetParticle() const;

		void SetEmitArea ( const sf::Vector3i& value );
		sf::Vector3i GetEmitArea() const;

	private:
		class CParticle *particle;

		sf::Vector3f position;
		sf::Vector3f direction;
		sf::Vector3i emitArea;
		sf::Vector3f grafity;

		float minVelocity;
		float maxVelocity;

		double life;
};


////////////////////////////////////////////////////////////
/// Ein einzelner Partikel
////////////////////////////////////////////////////////////
class CParticle
{
	public:
		CParticle ( CEmitter *Anker );
		CParticle *Next;

		void Reset ( CEmitter *Anker );

		void SetPosition ( const sf::Vector3f& value );
		void SetDelta_Position ( const sf::Vector3f& value );
		sf::Vector3f GetPosition() const;

		void SetColor ( const sf::Color& value );
		sf::Color GetColor() const;

		bool SetLife ( double value );
		bool setDelta_Life ( double value );
		double GetLife() const;

		void SetDirection ( const sf::Vector3f& value );
		sf::Vector3f GetDirection() const;

		void SetVelocity ( float value );
		float GetVelocity() const;

		void SetRotation ( int value );
		int GetRotation() const;
		
	private:
		sf::Vector3f position;
		sf::Vector3f direction;

		int rotation;

		float velocity;
		double life;

		sf::Color color;
};

#endif // PARTICLE_HPP
