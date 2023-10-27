/*
 * Odometer.cpp
 *
 *  Created on: Oct 23, 2023
 *      Author: Damo
 */

#include "Odometer.hpp"

#include "Shape2DUtils.hpp"
#include "MainApplication.hpp"

#include <random>

namespace Model
{

	/**
	 *
	 */
	/* static */ double Odometer::stddev = 1.0;

	Odometer::Odometer(const wxPoint& initialRobotPosition) :
										prevPosition(initialRobotPosition), distance(0)
	{
	}

	double Odometer::getStimulus(const wxPoint& robotPosition)
	{
		Application::MainSettings& mainSettings = Application::MainApplication::getSettings();
		std::random_device rd{};
		std::mt19937 gen{rd()};
		std::normal_distribution<> noise{0,mainSettings.getOdometerStdDev()};
		double result = Utils::Shape2DUtils::distance(robotPosition, prevPosition) + noise(gen);
		prevPosition = robotPosition;
		return result;
	}

	double Odometer::getPerceptFor(const wxPoint& robotPosition)
	{
		distance += getStimulus(robotPosition);
		return distance;
	}

	double Odometer::getDistance()
	{
		return distance;
	}

	void Odometer::setPrevPosition(const wxPoint& prevPosition_)
	{
		prevPosition = prevPosition_;
	}

	std::string Odometer::asString() const
	{
		return "Odometer";
	}

	std::string Odometer::asDebugString() const
	{
		return asString();
	}

};
