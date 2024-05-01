#include "Plot_AmpTime.h"

Plot_AmpTime::Plot_AmpTime(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& nameFile, const std::string& font)
{
    std::string title = "Amplitude over Time in file: " + nameFile;
    _plot = sf::Plot::Plot2D(size, position, title, font);
    _plot.setTitleColor(sf::Color::Black);
    _plot.setXTitle("Time (s)");
    _plot.setYTitle("Amplitude");
}

void Plot_AmpTime::addCurve(
    const std::string& name, 
    std::function<void(sf::Plot::Curve&, const std::vector<float>&, const std::vector<float>&, const sf::Vector2f&)> function,
    const std::vector<float>& axisX, const std::vector<float>& axisY, const sf::Color& color, float thickness)
{
	_plot.createCurve(name, color, thickness);
    function(_plot.getCurve(name), axisX, axisY, _plot.getGraphSize());
    
    float axisXStart = axisX.front();
    float axisXEnd = axisX.back();
    float minValue = *std::min_element(axisY.begin(), axisY.end());
    float maxValue = *std::max_element(axisY.begin(), axisY.end());

    _plot.setupAxes(sf::Vector2f(axisXStart, axisXEnd), sf::Vector2f(minValue, maxValue));
    _plot.setupGrid(sf::Vector2f(5, 0), sf::Vector2f(0, 5)); // 5 is the number of grid lines
}

void Plot_AmpTime::findPeaks(std::vector<std::pair<float, float>> merged_peak_ranges, float minTime, float maxTime)
{
    for (const auto& range : merged_peak_ranges) {
        float startX = (range.first - minTime) / (maxTime - minTime) * _plot.getGraphSize().x;
        float endX = (range.second - minTime) / (maxTime - minTime) * _plot.getGraphSize().x;

        sf::RectangleShape interval(sf::Vector2f(endX - startX, _plot.getGraphSize().y));
        interval.setPosition(_plot.getPosition().x + _plot.getGraphPosition().x + startX, _plot.getPosition().y + _plot.getGraphPosition().y);
        interval.setFillColor(sf::Color(0, 255, 0, 64)); // Полупрозрачный зеленый цвет
        peakIntervals.push_back(interval);
    }
}

void Plot_AmpTime::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(_plot, states);

    for (const auto& interval : peakIntervals) {
        target.draw(interval);
    }
}
