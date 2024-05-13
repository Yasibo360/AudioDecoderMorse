#include "Legend.h"

namespace sf
{
	namespace Plot
	{
		Legend::Legend() : _font(0) {}

		void Legend::setFont(sf::Font* font)
		{
			_font = font;
		}

		void Legend::addLegend(const sf::Plot::Curve& curve)
		{
			if (!_font) return;

			sf::Color color = curve.getColor();
			sf::Text name;

			name.setCharacterSize(12);
			name.setFont(*_font);
			name.setFillColor(sf::Color::Black);
			name.setString(curve.getName());

			_legend.push_back(std::make_pair(color, name));
		}

		void Legend::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			if (!_font) return;
			states.transform *= getTransform();

			float offset = 0.f; // начальное смещение

			for (const auto& pair : _legend)
			{
				if (pair.first.a != 0) {
					sf::RectangleShape line(sf::Vector2f(20.f, 4.f));
					line.setFillColor(pair.first);
					line.setPosition(offset, 0.f); // устанавливаем позицию линии с учетом смещения
					target.draw(line, states);

					sf::Text name = pair.second;
					name.setPosition(offset + line.getSize().x + 5.f, -line.getSize().y * 1.5); // устанавливаем позицию текста с учетом смещения
					target.draw(name, states);

					offset += line.getSize().x + name.getLocalBounds().getSize().x + 20.f; // увеличиваем смещение на ширину линии и небольшой отступ
				}								
			}
		}
	}
}