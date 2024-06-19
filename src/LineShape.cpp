#include "LineShape.hpp"
#include <cmath>

LineShape::LineShape() {}

LineShape::LineShape(const LineShape& source) :
	m_point_1{ source.m_point_1 },
	m_point_2{ source.m_point_2 },
	m_thickness{ source.m_thickness },
	m_color{ source.m_color }
{}

LineShape::LineShape(const sf::Vector2f& point_1, const sf::Vector2f& point_2) :
	m_point_1{ point_1 }, m_point_2{ point_2 }
{}

LineShape::LineShape(float point_1_x, float point_1_y, float point_2_x, float point_2_y) :
	m_point_1{ point_1_x, point_1_y }, m_point_2{ point_2_x, point_2_y }
{}

LineShape::~LineShape() {}

LineShape& LineShape::operator=(const LineShape& source) {
	m_point_1 = source.m_point_1;
	m_point_2 = source.m_point_2;
	m_thickness = source.m_thickness;
	m_color = source.m_color;
	return *this;
}

void LineShape::setPoints(const sf::Vector2f& point_1, const sf::Vector2f& point_2) {
	m_point_1 = point_1;
	m_point_2 = point_2;
}

void LineShape::setPoints(float point_1_x, float point_1_y, float point_2_x, float point_2_y) {
	m_point_1 = sf::Vector2f(point_1_x, point_1_y);
	m_point_2 = sf::Vector2f(point_2_x, point_2_y);
}

void LineShape::setPoint1(const sf::Vector2f& point_1) {
	m_point_1 = point_1;
}

void LineShape::setPoint1(float point_1_x, float point_1_y) {
	m_point_1 = sf::Vector2f(point_1_x, point_1_y);
}

void LineShape::setPoint2(const sf::Vector2f& point_2) {
	m_point_2 = point_2;
}

void LineShape::setPoint2(float point_2_x, float point_2_y) {
	m_point_2 = sf::Vector2f(point_2_x, point_2_y);
}

const sf::Vector2f& LineShape::getPoint1() const {
	return m_point_1;
}

const sf::Vector2f& LineShape::getPoint2() const {
	return m_point_2;
}

float LineShape::getThickness() const {
	return m_thickness;
}

const sf::Color& LineShape::getColor() const
{
	return m_color;
}

#include <iostream>

void LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto angle = atan2f(m_point_2.y - m_point_1.y, m_point_2.x - m_point_1.x);
	auto distance = std::sqrt(std::pow((m_point_2.x - m_point_1.x), 2) + std::pow((m_point_2.y - m_point_1.y), 2));

	sf::RectangleShape rectangle{ sf::Vector2f{distance + m_thickness, m_thickness} };
	rectangle.setOrigin(m_thickness / 2, m_thickness / 2);
	rectangle.rotate(angle);
	rectangle.setPosition(m_point_1);
	rectangle.setFillColor(m_color);
	target.draw(rectangle, states);

	std::cout << "Angle : " << angle << " - Distance : " << distance << std::endl;
}