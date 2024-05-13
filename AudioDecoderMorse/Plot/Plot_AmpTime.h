#pragma once

#include "Plot.h"

class Plot_AmpTime 
    : public sf::Drawable
    , public sf::Transformable
{
    private:
        sf::Plot::Plot2D _plot;
        std::vector<sf::RectangleShape> peakIntervals;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:   
        Plot_AmpTime(
            const sf::Vector2f& size = sf::Vector2f(100, 100), 
            const sf::Vector2f& position = sf::Vector2f(0, 0), 
            const std::string& nameFile = "", 
            const std::string& font = ""
        );

        void addCurve(const std::string& name,
            std::function<void(sf::Plot::Curve&, const std::vector<float>&, const std::vector<float>&, const sf::Vector2f&)> function,
            const std::vector<float>& axisX, 
            const std::vector<float>& axisY, 
            const sf::Color& color = sf::Color::Blue,
            float thickness = 1
        );

        void addWidePeaks(std::vector<std::pair<float, float>> merged_peak_ranges, float minTime, float maxTime);

        void graphAmplitudeFromTime(sf::Plot::Curve& curve, const std::vector<float>& time, const std::vector<float>& samples, const sf::Vector2f& graphSize);
};