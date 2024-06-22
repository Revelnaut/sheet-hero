#include "LineShape.hpp"
#include <cmath>
#include <numbers>

LineShape::LineShape() {}

LineShape::LineShape(const LineShape& source) :
	m_point_1{ source.m_point_1 },
	m_point_2{ source.m_point_2 },
	m_thickness{ source.m_thickness },
	m_color{ source.m_color } {}

LineShape::LineShape(const sf::Vector2f& point_1, const sf::Vector2f& point_2) :
	m_point_1{ point_1 }, m_point_2{ point_2 } {}

LineShape::LineShape(float point_1_x, float point_1_y, float point_2_x, float point_2_y) :
	m_point_1{ point_1_x, point_1_y }, m_point_2{ point_2_x, point_2_y } {}

LineShape::~LineShape() {}

LineShape& LineShape::operator=(const LineShape& source) {
	m_point_1 = source.m_point_1;
	m_point_2 = source.m_point_2;
	m_thickness = source.m_thickness;
	m_color = source.m_color;
	return *this;
}

void LineShape::set_points(const sf::Vector2f& point_1, const sf::Vector2f& point_2) {
	m_point_1 = point_1;
	m_point_2 = point_2;
}

void LineShape::set_points(float point_1_x, float point_1_y, float point_2_x, float point_2_y) {
	m_point_1 = sf::Vector2f(point_1_x, point_1_y);
	m_point_2 = sf::Vector2f(point_2_x, point_2_y);
}

void LineShape::set_point_1(const sf::Vector2f& point_1) {
	m_point_1 = point_1;
}

void LineShape::set_point_1(float point_1_x, float point_1_y) {
	m_point_1 = sf::Vector2f(point_1_x, point_1_y);
}

void LineShape::set_point_2(const sf::Vector2f& point_2) {
	m_point_2 = point_2;
}

void LineShape::set_point_2(float point_2_x, float point_2_y) {
	m_point_2 = sf::Vector2f(point_2_x, point_2_y);
}

void LineShape::set_color(const sf::Color& color) {
	m_color = color;
}

void LineShape::set_thickness(float thickness) {
	m_thickness = thickness;
	if ( m_thickness < 1.0f ) {
		m_thickness = 1.0f;
	}
}

const sf::Vector2f& LineShape::get_point_1() const {
	return m_point_1;
}

const sf::Vector2f& LineShape::get_point_2() const {
	return m_point_2;
}

float LineShape::get_thickness() const {
	return m_thickness;
}

const sf::Color& LineShape::get_color() const {
	return m_color;
}

void LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	float distance = std::sqrtf(std::powf(( m_point_2.x - m_point_1.x ), 2) + std::powf(( m_point_2.y - m_point_1.y ), 2));
	float angle = atan2f(m_point_2.y - m_point_1.y, m_point_2.x - m_point_1.x);
	angle *= 180.0f / std::numbers::pi; // Convert radians to degrees for SFML

	sf::RectangleShape rectangle{ sf::Vector2f{distance + m_thickness, m_thickness} };
	rectangle.setPosition(m_point_1);
	rectangle.setFillColor(m_color);
	rectangle.setOrigin(m_thickness / 2, m_thickness / 2);
	rectangle.rotate(angle);

	target.draw(rectangle, states);
}