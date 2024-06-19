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

	void set_points(const sf::Vector2f& point_1, const sf::Vector2f& point_2);
	void set_points(float point_1_x, float point_1_y, float point_2_x, float point_2_y);

	void set_point_1(const sf::Vector2f& point_1);
	void set_point_1(float point_1_x, float point_1_y);

	void set_point_2(const sf::Vector2f& point_2);
	void set_point_2(float point_2_x, float point_2_y);

	void set_color(const sf::Color& color);
	void set_thickness(float thickness);

	const sf::Vector2f& get_point_1() const;
	const sf::Vector2f& get_point_2() const;
	float get_thickness() const;
	const sf::Color& get_color() const;
private:
	sf::Vector2f m_point_1{};
	sf::Vector2f m_point_2{};

	float m_thickness{ 1.0f };
	sf::Color m_color{};

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};