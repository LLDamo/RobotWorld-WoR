/*
 * Compas.cpp
 *
 *  Created on: Oct 23, 2023
 *      Author: Damo
 */

#include "Compas.hpp"

#include "Logger.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"
#include "MainApplication.hpp"

#include <random>

namespace Model
{
	/**
	 *
	 */
	/* static */ double Compas::stddev = 2.0;
	/**
	 *
	 */
	Compas::Compas( Robot& aRobot) :
								AbstractSensor( aRobot)
	{
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractStimulus > Compas::getStimulus() const
	{
		Robot* robot = dynamic_cast<Robot*>(agent);
		if(robot)
		{
			Application::MainSettings& mainSettings = Application::MainApplication::getSettings();
			std::random_device rd{};
			std::mt19937 gen{rd()};
		    std::normal_distribution<> noise{0,mainSettings.getCompasStdDev()};

		    double angle = Utils::Shape2DUtils::getAngle( robot->getFront()); // + 0.5 * Utils::PI;
		    angle += Utils::MathUtils::toRadians(noise(gen));
		    return std::make_shared< AngleStimulus >( angle);
		}
		return std::make_shared< AngleStimulus >( 0.0);
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractPercept > Compas::getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const
	{
		Robot* robot = dynamic_cast< Robot* >( agent);
		if (robot)
		{
			AngleStimulus* angleStimulus = dynamic_cast< AngleStimulus* >( anAbstractStimulus.get());
			if(angleStimulus)
			{
				return std::make_shared<AnglePercept>( angleStimulus->angle);
			}
		}

		return std::make_shared<AnglePercept>( 0.0);
	}
	/**
	 *
	 */
	std::string Compas::asString() const
	{
		return "LaserDistanceSensor";
	}
	/**
	 *
	 */
	std::string Compas::asDebugString() const
	{
		return asString();
	}
} // namespace Model

