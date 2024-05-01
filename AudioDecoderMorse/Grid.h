#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

// This class represents a grid that can be drawn on a sf::RenderTarget.
// It is a sf::Drawable and sf::Transformable.

namespace sf
{
	namespace Plot
	{		
		class Grid
			: public sf::Drawable
			, public sf::Transformable
		{
		private:
			// The number of parts the grid is divided into.
			int _numPart;

			// The vertices that make up the grid.
			sf::VertexArray _vertices;

			// The border of the grid.
			sf::RectangleShape _border;

			// The color of the grid.
			sf::Color _color;

			// The size of the grid.
			sf::Vector2f _size;

			/**
			 * @brief Draws the curve on the provided target.
			 *
			 * @param target The target to draw on.
			 * @param states The render states to use.
			 */
			void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		public:
			/**
			 * @brief Constructor for the Grid class with a specified number of parts.
			 *
			 * This constructor initializes a Grid object with the specified number of parts.
			 * If the number of parts is not specified, the default value of 5 is used.
			 *
			 * @param numPart The number of parts of the grid.
			 */
			Grid();
			
			/**
			 * @brief Sets the size and range of the grid.
			 *
			 * This method sets the size and range of the grid. If the range is not specified, default values (0, 0) are used for vertical lines and (0, 0) for horizontal lines.
			 *
			 * @param size The size of the grid.
			 * @param gridRange The range of the grid. A pair of values: the first value is for vertical lines, and the second value is for horizontal lines.
			 */
			void setup(const Vector2f& size, std::pair<sf::Vector2f, sf::Vector2f> gridRange = std::make_pair(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
		};
	}
}