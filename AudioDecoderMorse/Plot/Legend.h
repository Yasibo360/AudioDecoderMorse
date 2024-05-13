#pragma once

#include "Curve.h"

namespace sf
{
	namespace Plot
	{
		class Legend
			: public sf::Drawable
			, public sf::Transformable
		{
		public:
			Legend();

			void setFont(sf::Font* font);

			void addLegend(const sf::Plot::Curve& curve);

		private:
			sf::Font* _font;

			std::vector<std::pair<sf::Color, sf::Text>> _legend;

			void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};
	}
}