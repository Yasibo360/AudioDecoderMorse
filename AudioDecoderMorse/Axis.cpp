#include "Axis.h"

namespace sf
{
	namespace Plot
	{
		Axis::Axis() : _font(0), _size(0), _numPart(5) {}

		void Axis::setSize(float size)
		{
			_size = size;
		}

		void Axis::setName(const std::string& name) {
			_name = name;
		}

		void Axis::setFont(sf::Font* font) {
			_font = font;
		}

		void Axis::defineLabel()
		{
			if (!_font) return;

			_label.setFont(*_font);
			_label.setCharacterSize(12);
			_label.setFillColor(sf::Color::Black);
			_label.setString(_name);

			if (getRotation() != 0)
				_label.setPosition(_size / 2 - _name.size() * 5, -20);
			else
				_label.setPosition(_size / 2 - _name.size() * 5, 20);
		}

		void Axis::setup(const Vector2f& range)
		{
			if (!_font) return;
			_numbers.clear();

			defineLabel();

			float distance = fabs(range.y - range.x);
			if (distance == 0) distance = 1;

			float decx = _size / _numPart;

			float offset = distance / _numPart;
			for (int i = 0; i <= _numPart; i++)
			{
				sf::Text text;
				text.setFont(*_font);
				text.setCharacterSize(10);
				text.setFillColor(sf::Color::Black);
				std::stringstream ss;
				float value = range.x + offset * i;
				if (fabs(value) < 10000)
				{
					ss << std::setprecision(4) << value;
				}
				else
				{
					ss << std::scientific << std::setprecision(3) << value;
				}
				text.setString(ss.str());
				text.setPosition(i * decx - ss.str().size() * 2, 0);
				_numbers.push_back(text);
			}
		}

		void Axis::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			if (!_font) return;
			states.transform *= getTransform();

			for (std::vector<sf::Text>::const_iterator it = _numbers.begin(); it != _numbers.end(); ++it)
				target.draw(*it, states);
			target.draw(_label, states);
		}
	}
}