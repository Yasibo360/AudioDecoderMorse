#include "Grid.h"

namespace sf
{
	namespace Plot
	{
		Grid::Grid() : _numPart(5) {}

		void Grid::setup(const Vector2f& size, std::pair <sf::Vector2f, sf::Vector2f> gridRange) {
			_size = size;

			_color = sf::Color(150, 150, 150);
			_vertices.clear();

			_border.setSize(_size);
			_border.setFillColor(sf::Color(0, 0, 0, 0));
			_border.setOutlineColor(sf::Color(0, 0, 0));
			_border.setOutlineThickness(1);

			// Set the primitive type of the vertices to lines
			_vertices.setPrimitiveType(sf::Lines);

			float offset_y, offset_x;

			// Calculate the offset for the vertical lines
			offset_x = (gridRange.first.x == 0) ? (offset_x = _size.x / _numPart) : (offset_x = _size.x / gridRange.first.x);

			// Calculate the offset for the horizontal lines
			offset_y = (gridRange.second.y == 0) ? (offset_y = _size.y / 2 / _numPart) : (offset_y = _size.y / 2 / gridRange.second.y);

			// Loop over the vertical lines
			for (int i = 1; i < _numPart; i++) {
				// Add the start and end points of the vertical line to the vertices
				_vertices.append(sf::Vertex(sf::Vector2f(i * offset_x, 0), _color));
				_vertices.append(sf::Vertex(sf::Vector2f(i * offset_x, _size.y), _color));
			}

			// Loop over the horizontal lines
			for (int i = 0; i < _numPart; i++) {
				// Add the start and end points of the horizontal line to the vertices
				_vertices.append(sf::Vertex(sf::Vector2f(0, _size.y / 2 - i * offset_y), _color));
				_vertices.append(sf::Vertex(sf::Vector2f(_size.x, _size.y / 2 - i * offset_y), _color));

				_vertices.append(sf::Vertex(sf::Vector2f(0, _size.y / 2 + i * offset_y), _color));
				_vertices.append(sf::Vertex(sf::Vector2f(_size.x, _size.y / 2 + i * offset_y), _color));
			}
		}

		void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
			states.transform *= getTransform();

			target.draw(_vertices, states);
			target.draw(_border, states);
		}
	}
}