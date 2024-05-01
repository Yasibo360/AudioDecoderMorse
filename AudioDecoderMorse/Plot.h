#pragma once

#include "Curve.h"
#include "Grid.h"
#include "Axis.h"
#include <unordered_map>

namespace sf
{
	namespace Plot
	{
		class Plot2D final 
			: public sf::Drawable
			, public sf::Transformable
		{
		public:
			Plot2D();
			
			/**
			 * @brief Construct a new Plot2D object with the specified parameters.
			 *
			 * This constructor initializes a new Plot2D object with the given size, position, title, and font.
			 *
			 * @param size The size of the plot.
			 * @param position The position of the plot.
			 * @param title The title of the plot.
			 * @param font The font to be used in the plot.
			 */
			Plot2D(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& title, const std::string& font);

			/**
			 * @brief Sets the size of the plot.
			 *
			 * This function sets the size of the plot, including the grid, axes, and background.
			 *
			 * @param size The new size of the plot.
			 */
			void setSize(const sf::Vector2f& size);

			sf::Vector2f getGraphSize() const;

			/**
			 * @brief Sets the position of the plot in the 2D space.
			 *
			 * This function sets the position of the plot in the 2D space. It also updates the positions of the grid, axes, and backgrounds.
			 *
			 * @param position The new position of the plot.
			 */
			void setPosition(const sf::Vector2f& position);

			sf::Vector2f getGraphPosition() const;

			/**
			 * @brief Set up the background for the 2D plot.
			 * @param colorBkgrg - The color of the background.
			 * @param colorOutlineBkgrg - The color of the background outline.
			 * @param colorGraphBkgrg- The color of the graph background.
			 * @param colorOutlineGraphBkgrg - The color of the graph background outline.
			 */
			void setupBackground(const sf::Color& colorBkgrg = sf::Color::White,
				const sf::Color& colorOutlineBkgrg = sf::Color(128, 128, 128),
				const sf::Color& colorGraphBkgrg = sf::Color::White,
				const sf::Color& colorOutlineGraphBkgrg = sf::Color::Black
			);

			/**
			 * @brief Sets the font of the plot.
			 *
			 * This function sets the font of the plot. It loads the font from the specified file and sets it for the title, x-axis, and y-axis.
			 *
			 * @param filename The path to the font file.
			 * @throws std::string If the font file cannot be loaded.
			 */
			void setFont(const std::string& filename);

			/**
			 * @brief Sets the title of the plot.
			 *
			 * This function sets the title of the plot. It also sets the character size and position of the title.
			 *
			 * @param title The new title of the plot.
			 */
			void setTitle(const std::string& title);

			/**
			 * @brief Sets the color of the plot title.
			 *
			 * This function sets the color of the plot title.
			 *
			 * @param color The new color of the plot title.
			 */
			void setTitleColor(const sf::Color& color = sf::Color::Black);

			/**
			 * @brief Set the name of the x-axis.
			 * @param name The name of the x-axis.
			 */
			void setXTitle(const std::string& name);

			/**
			 * @brief Set the name of the y-axis.
			 * @param name The name of the y-axis.
			 */
			void setYTitle(const std::string& name);
		
			void setupAxes(const sf::Vector2f axisX, const sf::Vector2f axisY);

			void setupGrid(const sf::Vector2f gridX, const sf::Vector2f gridY);

			/**
			 * @brief Create a new curve and add it to the plot.
			 *
			 * This function creates a new curve with the specified parameters and adds it to the plot.
			 *
			 * @param name The name of the new curve.
			 * @param color The color of the curve.
			 * @param thickness The thickness of the curve.
			 * @return A reference to the newly created curve.
			*/
			Curve& createCurve(const std::string& name, const sf::Color& color = sf::Color::Black, float thickness = 1);
			
			Curve& getCurve(const std::string& name);

			/**
			 * @brief Check if a curve with the given name exists.
			 * @param name The name of the curve to check.
			 * @return True if the curve exists, False otherwise.
			 */
			bool haveCurve(const std::string& name) const;		

			

		private:
			// Size of the plot
			sf::Vector2f _size;

			// Indentation for the plot elements
			int _indentation;

			// Size of the graph area
			sf::Vector2f _graphSize;

			// Position of the graph
			sf::Vector2f _graphPos;

			// Text for the title of the plot
			sf::Text _title;

			// Background rectangle shape
			sf::RectangleShape _bkgrg;

			// Graph background rectangle shape
			sf::RectangleShape _graphBkgrg;

			// X-axis of the plot
			Axis _xaxis;

			// Y-axis of the plot
			Axis _yaxis;

			// Grid for the plot
			sf::Plot::Grid _grid;

			// Map of curves in the plot
			std::unordered_map<std::string, Curve> _curves;

			/**
			 * @brief Draw the plot on the given target.
			 * @param target The target to draw the plot on.
			 * @param states The states to apply to the plot.
			 */
			void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};		
	}
}