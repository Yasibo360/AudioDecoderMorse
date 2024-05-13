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

bool Plot_AmpTime::haveCurve(const std::string& name)
{
    return _plot.haveCurve(name);
}

int Plot_AmpTime::getCurveCount()
{
    return _plot.getCurveCount();
}

void Plot_AmpTime::addWidePeaks(std::vector<std::pair<float, float>> merged_peak_ranges, float minTime, float maxTime)
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

void Plot_AmpTime::graphAmplitudeFromTime(sf::Plot::Curve& curve, const std::vector<float>& time, const std::vector<float>& samples, const sf::Vector2f& graphSize) {
    float x = 0.0f;
    float y = 0.0f;

    float minSample = *std::min_element(samples.begin(), samples.end());
    float maxSample = *std::max_element(samples.begin(), samples.end());
    float minTime = *std::min_element(time.begin(), time.end());
    float maxTime = *std::max_element(time.begin(), time.end());

    for (std::size_t i = 0; i < samples.size(); ++i) {
        x = (time[i] - minTime) / (maxTime - minTime) * graphSize.x;

        float N = std::abs(std::abs(maxSample) - std::abs(minSample));
        float k = N / (maxSample - minSample + 2 * N) * graphSize.y;

        y = graphSize.y - (samples[i] - minSample) / (maxSample - minSample) * (graphSize.y - 2 * k) - k / 2;

        curve.addVertex(sf::Vector2f(x, y));
    }

    return;
}

void Plot_AmpTime::graphHorizontalLine(sf::Plot::Curve& curve, const std::vector<float>& time, const std::vector<float>& samples, const sf::Vector2f& graphSize) {
    float x = 0.0f;
    float y = 0.0f;

    float minSample = *std::min_element(samples.begin(), samples.end());
    float maxSample = *std::max_element(samples.begin(), samples.end());
    float minTime = *std::min_element(time.begin(), time.end());
    float maxTime = *std::max_element(time.begin(), time.end());

    float max_sample = *std::max_element(samples.begin(), samples.end());
    float tresh = max_sample * 0.5;

    for (std::size_t i = 0; i < samples.size(); ++i) {
        x = (time[i] - minTime) / (maxTime - minTime) * graphSize.x;

        float N = std::abs(std::abs(maxSample) - std::abs(minSample));
        float k = N / (maxSample - minSample + 2 * N) * graphSize.y;

        y = graphSize.y - (tresh - minSample) / (maxSample - minSample) * (graphSize.y - 2 * k) - k / 2;

        curve.addVertex(sf::Vector2f(x, y));
    }

    return;
}

void Plot_AmpTime::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(_plot, states);

    for (const auto& interval : peakIntervals) {
        target.draw(interval);
    }
}
