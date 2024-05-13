#pragma once

#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace sf
{
	namespace Plot
	{
		class Axis
			: public sf::Drawable
			, public sf::Transformable
		{
		public:
			/**
			 * @brief Default constructor for the Axis class.
			 */
			Axis();

			/**
			 * @brief Sets the size.
			 *
			 * This method sets the size.
			 *
			 * @param size The size.
			 */
			void setSize(float size);

			/**
			 * @brief Sets the name.
			 *
			 * This method sets the name.
			 *
			 * @param name The name.
			 */
			void setName(const std::string& name);

			/**
			 * @brief Sets the range.
			 *
			 * This method sets the range.
			 *
			 * @param range The range.
			 */
			void setup(const Vector2f& range);

			/**
			 * @brief Sets the font.
			 *
			 * This method sets the font.
			 *
			 * @param font A pointer to the font.
			 */
			void setFont(sf::Font* font);

		private:
			// Split the axis in numPart part
			int _numPart;

			// Axis name
			std::string _name;

			// Global font
			sf::Font* _font;

			// Axis size
			float _size;

			// Axis label
			sf::Text _label;

			// Axis number
			std::vector<sf::Text> _numbers;

			void defineLabel();

			void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};
	}
}