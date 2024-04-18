#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#include "Plot/plot.h"


class AudioGraph : public sf::Drawable
{
public:
    AudioGraph(const std::vector<short>& audioData, const sf::Vector2i& location)
        : audioData_(audioData)
    {
        plot_.setSize(sf::Vector2f(800, 500));
        plot_.setTitle("Amplitude vs Time");
        plot_.setFont("font.ttf");
        plot_.setXLabel("Time (seconds)");
        plot_.setYLabel("Amplitude");
        plot_.setBackgroundColor(sf::Color(255, 255, 255));
        plot_.setTitleColor(sf::Color::Black);
        plot_.setPosition(sf::Vector2f(600 * location.x, 400 * location.y));
        sf::plot::Curve& curve = plot_.createCurve("audio", sf::Color::Red);
        curve.setFill(rand() % 2);
        curve.setThickness(2);
        curve.setColor(sf::Color::Red);
        curve.setLimit(audioData_.size() / 1000);
    }

    void update()
    {
        auto maxElement = std::max_element(audioData_.begin(), audioData_.end());
        sf::plot::Curve& curve = plot_.getCurve("audio");
        for (size_t i = 0; i < audioData_.size(); ++i)
        {
            float amplitude = static_cast<float>(audioData_[i]);
            curve.addValue(amplitude);
        }
        

        plot_.prepare();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(plot_, states);
    }

private:
    std::vector<short> audioData_;
    sf::plot::Plot plot_;
};
