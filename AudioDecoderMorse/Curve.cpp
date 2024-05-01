#include "Curve.h"

namespace sf
{
	namespace Plot
	{
		Curve::Curve() : _name(""), _color(sf::Color::Black), _thickness(1) 
		{
			_vertices.setPrimitiveType(sf::LinesStrip);
		};

		Curve::Curve(const std::string& name, const sf::Vector2f& size, const sf::Color color, float thickness, sf::PrimitiveType type)
			: _name(name), _color(color), _thickness(thickness), _size(size)
		{
			_vertices.setPrimitiveType(type);
		};

		void Curve::setSize(const sf::Vector2f& size) {
			_size = size;
		}
				
		void Curve::setName(const std::string& name) {
			_name = name;
		}
			
		void Curve::setColor(const sf::Color& color) {
			_color = color;
		}

		void Curve::setThickness(const float& thickness) {
			_thickness = thickness;
		}

		const sf::VertexArray& Curve::getVertices() const {
			return _vertices;
		}

		void Curve::addVertex(sf::Vector2f position) {
			_vertices.append(sf::Vertex(position, _color));
		}

		void Curve::draw(sf::RenderTarget& target, sf::RenderStates states) const {
			states.transform *= getTransform();
			target.draw(_vertices, states);
		}
	}
}