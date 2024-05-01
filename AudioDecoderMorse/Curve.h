#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <functional>

// The Curve class is a part of the sf.Plot namespace and inherits from sf::Drawable and sf::Transformable.
// It represents a graph or curve that can be drawn on the screen.

namespace sf
{
	namespace Plot
	{
		/**
		 * @class Curve
		 * @brief A class representing a curve
		 *
		 * This class represents a curve and provides methods to set its properties and draw it.
		 */
		class Curve
			: public sf::Drawable
			, public sf::Transformable
		{
		private:
			// Graph name
			std::string _name;

			// Graph color
			sf::Color _color;

			// Graph thickness
			float _thickness;

			// Array of graph vertices
			sf::VertexArray _vertices;

			// Graph size
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
			 * @brief Default constructor. Initializes a black curve with default thickness.
			 */
			Curve();
			
			/**
			 * @brief Overloaded constructor. Initializes a curve with provided parameters.
			 *
			 * @param name The name of the curve.
			 * @param size The size of the curve.
			 * @param color The color of the curve (default is black).
			 * @param thickness The thickness of the curve (default is 1).
			 */
			Curve(
				const std::string& name, 
				const sf::Vector2f& size, 
				const sf::Color color = sf::Color::Blue, 
				float thickness = 1, 
				sf::PrimitiveType type = sf::PrimitiveType::LinesStrip
			);

			/**
			 * @brief Sets the size of the curve.
			 *
			 * @param size The size to set.
			 */
			void setSize(const sf::Vector2f& size);

			/**
			 * @brief Sets the name of the curve.
			 *
			 * @param name The name to set.
			 */
			void setName(const std::string& name);

			/**
			 * @brief Sets the color of the curve.
			 *
			 * @param color The color to set.
			 */
			void setColor(const sf::Color& color);

			/**
			 * @brief Sets the thickness of the curve.
			 *
			 * @param thickness The thickness to set.
			 */
			void setThickness(const float& thickness);

			/**
			 * @brief Returns the array of vertices that make up the curve.
			 *
			 * @return The array of vertices.
			 */
			const sf::VertexArray& getVertices() const;

			/**
			 * @brief Adds a vertex to the curve.
			 *
			 * @param position The position of the vertex to add.
			 */
			void addVertex(sf::Vector2f position);
		};
	}
}