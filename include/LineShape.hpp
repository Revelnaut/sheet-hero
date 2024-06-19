#pragma once

#include <SFML/Graphics.hpp>

class LineShape : public sf::Drawable, public sf::Transformable {
public:
	LineShape();
	LineShape(const LineShape& source);
	LineShape(const sf::Vector2f& point_1, const sf::Vector2f& point_2);
	LineShape(float point_1_x, float point_1_y, float point_2_x, float point_2_y);
	~LineShape();

	LineShape& operator=(const LineShape& source);

	void setPoints(const sf::Vector2f& point_1, const sf::Vector2f& point_2);
	void setPoints(float point_1_x, float point_1_y, float point_2_x, float point_2_y);

	void setPoint1(const sf::Vector2f& point_1);
	void setPoint1(float point_1_x, float point_1_y);

	void setPoint2(const sf::Vector2f& point_2);
	void setPoint2(float point_2_x, float point_2_y);

	void setColor(const sf::Color& color);
	void setThickness(float thickness);

	const sf::Vector2f& getPoint1() const;
	const sf::Vector2f& getPoint2() const;
	float getThickness() const;
	const sf::Color& getColor() const;
private:
	sf::Vector2f m_point_1{};
	sf::Vector2f m_point_2{};

	float m_thickness{ 1.0f };
	sf::Color m_color{};

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};