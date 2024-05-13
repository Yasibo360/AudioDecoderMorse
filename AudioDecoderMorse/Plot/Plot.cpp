#include "Plot.h"

namespace sf
{
	namespace Plot
	{
		Plot2D::Plot2D()
		{
		}

		Plot2D::Plot2D(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& title, const std::string& font) :
			_indentation(45),
			_curves()
		{
			// Set the font for the plot
			setFont(font);

			// Set the size of the plot
			setSize(size);

			// Set the position of the plot
			setPosition(position);

			// Set the title of the plot. Note: Title can be set after setting sizes and positions
			setTitle(title);

			// Setup the background for the plot
			setupBackground();
		}

		void Plot2D::setSize(const sf::Vector2f& size)
		{
			// Default minimum and maximum values for the axes.
			int defMin = 0;
			int defMax = 1;

			// Set the size of the plot.
			_size = size;

			// Calculate the size of the graph area.
			float graphWidth = _size.x - 2 * _indentation;
			float graphHeight = _size.y - 3 * _indentation;

			_graphSize = sf::Vector2f(graphWidth, graphHeight);

			// Set up the grid with the new graph size.
			_grid.setup(_graphSize);

			// Set the size of the x-axis to the width of the graph area.
			_xaxis.setSize(_graphSize.x);

			// Set up the x-axis with default minimum and maximum values.
			_xaxis.setup(sf::Vector2f(defMin, defMax));

			// Set the size of the y-axis to the height of the graph area.
			_yaxis.setSize(_graphSize.y);

			// Set up the y-axis with default minimum and maximum values.
			_yaxis.setup(sf::Vector2f(defMin, defMax));

			// Set the size of the background to the size of the plot.
			_bkgrg.setSize(_size);

			// Set the size of the graph background to the size of the graph area.
			_graphBkgrg.setSize(_graphSize);
		}

		sf::Vector2f Plot2D::getGraphSize() const {
			return _graphSize; 
		}

		void Plot2D::setPosition(const sf::Vector2f& position) {
			// Set the position of the plot
			this->sf::Transformable::setPosition(position);

			// Set the position of the grid
			_graphPos = sf::Vector2f(_indentation, _indentation);
			_grid.setPosition(_graphPos);

			_legend.setPosition(_graphPos.x, _graphPos.y + _graphSize.y + _indentation);

			// Set the position of the x-axis
			_xaxis.setPosition(_graphPos.x, _graphPos.y + _graphSize.y + 5);

			// Set the position of the y-axis
			_yaxis.setPosition(_graphPos.x - 20, _graphPos.y + _graphSize.y);

			// Rotate the y-axis by -90 degrees
			_yaxis.setRotation(-90);

			// Set the position of the background
			_bkgrg.setPosition(0, 0);

			// Set the position of the graph background
			_graphBkgrg.setPosition(sf::Vector2f(_indentation, _indentation));
		}

		sf::Vector2f Plot2D::getGraphPosition() const
		{
			return _graphPos;
		}

		void Plot2D::setupBackground(
			const sf::Color& colorBkgrg,
			const sf::Color& colorOutlineBkgrg, 
			const sf::Color& colorGraphBkgrg,
			const sf::Color& colorOutlineGraphBkgrg
		) 
		{
			// Set the background color and outline
			_bkgrg.setFillColor(sf::Color::White);
			_bkgrg.setOutlineColor(sf::Color(128, 128, 128)); // Grey
			_bkgrg.setOutlineThickness(1);

			// Set the graph background color and outline
			_graphBkgrg.setFillColor(sf::Color::White);
			_graphBkgrg.setOutlineColor(sf::Color::Black);
			_graphBkgrg.setOutlineThickness(1);
		}

		void Plot2D::setFont(const std::string& filename) {
			static sf::Font sharedFont;

			// Загружаем шрифт только один раз
			if (sharedFont.getInfo().family.empty()) {
				if (!sharedFont.loadFromFile(filename)) {
					throw std::string("Failed to load font");
				}
			}

			// Set the font for the title, x-axis, and y-axis
			_title.setFont(sharedFont);
			_legend.setFont(&sharedFont);
			_xaxis.setFont(&sharedFont);
			_yaxis.setFont(&sharedFont);
		}

		void Plot2D::setTitle(const std::string& title) {
			// Set the string of the title
			_title.setString(title);

			// Set the character size of the title
			_title.setCharacterSize(16);

			// Set the position of the title
			_title.setPosition(sf::Vector2f((_size.x - _title.getGlobalBounds().width) / 2, _indentation / 3));
		}

		void Plot2D::setTitleColor(const sf::Color& color) {
			_title.setFillColor(color);
		}

		void Plot2D::setXTitle(const std::string& name) {
			_xaxis.setName(name);
		}

		void Plot2D::setYTitle(const std::string& name) {
			_yaxis.setName(name);
		}

		void Plot2D::setupAxes(const sf::Vector2f axisX, const sf::Vector2f axisY)
		{
			_xaxis.setup(axisX);
			_yaxis.setup(axisY);
		}

		void Plot2D::setupGrid(const sf::Vector2f gridX, const sf::Vector2f gridY)
		{
			std::pair<sf::Vector2f, sf::Vector2f> pair;
			pair.first = gridX;
			pair.second = gridY;
			_grid.setup(_graphSize, pair);
		}

		Curve& Plot2D::createCurve(const std::string& name, const sf::Color& color, float thickness)
		{
			// Create a new curve
			_curves[name] = Curve();
			Curve& newCurve = _curves[name];

			// Set the name, size, position, color, and thickness of the curve
			newCurve.setName(name);
			newCurve.setSize(_graphSize);
			newCurve.setPosition(_graphPos);
			newCurve.setColor(color);
			newCurve.setThickness(thickness);

			_legend.addLegend(newCurve);

			return newCurve;
		}

		int Plot2D::getCurveCount()
		{
			return _curves.size();
		}

		Curve& Plot2D::getCurve(const std::string& name) {
			auto it = _curves.find(name);
			if (it == _curves.end()) {
				throw std::out_of_range("Curve not found");
			}
			return it->second;
		}

		bool Plot2D::haveCurve(const std::string& name) const {
			return _curves.find(name) != _curves.end();
		}

		void Plot2D::draw(sf::RenderTarget& target, sf::RenderStates states) const {
			states.transform *= getTransform();

			// Draw the background, graph background, grid, title, x-axis, y-axis, and curves
			target.draw(_bkgrg, states);
			target.draw(_graphBkgrg, states);
			target.draw(_grid, states);
			target.draw(_title, states);
			target.draw(_xaxis, states);
			target.draw(_yaxis, states);
			target.draw(_legend, states);

			for (const auto& pair : _curves) {
				target.draw(pair.second, states);
			}
		}
	}
}