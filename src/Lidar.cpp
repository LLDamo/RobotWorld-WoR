/*
 * Lidar.cpp
 *
 *  Created on: Oct 4, 2023
 *      Author: Damo
 */

#include "Lidar.hpp"

#include "Logger.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"
#include "MainApplication.hpp"

#include <random>

namespace Model
{
	/**
	 *
	 */
	/* static */ double Lidar::stddev = 10.0;
	/**
	 *
	 */
	Lidar::Lidar( Robot& aRobot) :
								AbstractSensor( aRobot)
	{
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractStimulus > Lidar::getStimulus() const
	{
		Robot* robot = dynamic_cast<Robot*>(agent);
		Stimuli stimuli;
		if(robot)
		{
			Application::MainSettings& mainSettings = Application::MainApplication::getSettings();
			std::random_device rd{};
			std::mt19937 gen{rd()};
		    std::normal_distribution<> noise{0,mainSettings.getLidarStdDev()};
		    for (std::size_t beam = 0; beam < lidarBeams; ++beam)
		    {
				double angle = (double)beam * angleStep;

				DistanceStimulus distanceStimulus(angle,noDistance);

				std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();
				for (std::shared_ptr< Wall > wall : walls)
				{
					wxPoint wallPoint1 = wall->getPoint1();
					wxPoint wallPoint2 = wall->getPoint2();
					wxPoint robotLocation = robot->getPosition();
					wxPoint laserEndpoint{static_cast<int>(robotLocation.x + std::cos( angle) * lidarLaserBeamLength + noise(gen)) ,
										static_cast<int>(robotLocation.y + std::sin( angle) * lidarLaserBeamLength + noise(gen))};

					wxPoint interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, robotLocation, laserEndpoint);

					if(interSection != wxDefaultPosition)
					{
						double distance = Utils::Shape2DUtils::distance(robotLocation,interSection);
						if(distance < distanceStimulus.distance || distanceStimulus.distance == noDistance)
						{
							distanceStimulus.distance = distance;
						}
					}
				}
				stimuli.push_back( distanceStimulus);
		    }
		}
		return std::make_shared< DistanceStimuli >( stimuli);
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractPercept > Lidar::getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const
	{
		Robot* robot = dynamic_cast< Robot* >( agent);
		std::vector<DistancePercept> pointCloud;
		if (robot)
		{
			wxPoint robotLocation = robot->getPosition();

			DistanceStimuli* distanceStimuli = dynamic_cast< DistanceStimuli* >( anAbstractStimulus.get());

			if(distanceStimuli)
			{
				for(std::size_t beam = 0; beam < distanceStimuli->stimuli.size(); ++beam)
				{
					if(distanceStimuli->stimuli.at(beam).distance == noDistance)
					{
//						wxPoint endpoint{	static_cast< int >( robotLocation.x + std::cos( distanceStimuli->stimuli.at(beam).angle)*lidarLaserBeamLength),
//										static_cast< int >( robotLocation.y + std::sin( distanceStimuli->stimuli.at(beam).angle)*lidarLaserBeamLength)};
//
//						pointCloud.push_back(DistancePercept( endpoint));
						pointCloud.push_back( DistancePercept(wxPoint(noObject,noObject)));
					}
					else
					{
						wxPoint endpoint{	static_cast< int >( robotLocation.x + std::cos( distanceStimuli->stimuli.at(beam).angle)*distanceStimuli->stimuli.at(beam).distance),
										static_cast< int >( robotLocation.y + std::sin( distanceStimuli->stimuli.at(beam).angle)*distanceStimuli->stimuli.at(beam).distance)};

						pointCloud.push_back(DistancePercept( endpoint));
					}
				}
			}
		}

		return std::make_shared<DistancePercepts>( pointCloud);
	}
	/**
	 *
	 */
	std::shared_ptr< DistanceStimuli > Lidar::getStimulus(const wxPoint& origin)
	{
		Stimuli stimuli;
		stimuli.reserve(lidarBeams);

		Application::MainSettings& mainSettings = Application::MainApplication::getSettings();
		std::random_device rd{};
		std::mt19937 gen{rd()};
		std::normal_distribution<> noise{0,mainSettings.getLidarStdDev()};
		for (std::size_t beam = 0; beam < lidarBeams; ++beam)
		{
			double angle = (double)beam * angleStep; //6.2832 is aproximately 2pi

			DistanceStimulus distanceStimulus(angle,noDistance);

			std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();
			for (std::shared_ptr< Wall > wall : walls)
			{
				wxPoint wallPoint1 = wall->getPoint1();
				wxPoint wallPoint2 = wall->getPoint2();
				wxPoint laserEndpoint{static_cast<int>(origin.x + std::cos( angle) * lidarLaserBeamLength + noise(gen)) ,
									static_cast<int>(origin.y + std::sin( angle) * lidarLaserBeamLength + noise(gen))};

				wxPoint interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, origin, laserEndpoint);

				if(interSection != wxDefaultPosition)
				{
					double distance = Utils::Shape2DUtils::distance(origin,interSection);
					if(distance < distanceStimulus.distance || distanceStimulus.distance == noDistance)
					{
						distanceStimulus.distance = distance;
					}
				}
			}
			stimuli.push_back( distanceStimulus);
		}
		return std::make_shared< DistanceStimuli >( stimuli);
	}
	/**
	 *
	 */
	void Lidar::getPerceptFor( const wxPoint& origin, PointCloud& resultPointCloud)
	{
		resultPointCloud.clear();
		resultPointCloud.reserve(lidarBeams);

		std::shared_ptr<DistanceStimuli> distanceStimuli = Lidar::getStimulus(origin);

		if(distanceStimuli)
		{
			for(std::size_t beam = 0; beam < distanceStimuli->stimuli.size(); ++beam)
			{
				if(distanceStimuli->stimuli.at(beam).distance == noDistance)
				{
					resultPointCloud.push_back( DistancePercept(wxPoint(noObject,noObject)));
				}
				else
				{
					wxPoint endpoint{	static_cast< int >( origin.x + std::cos( distanceStimuli->stimuli.at(beam).angle)*distanceStimuli->stimuli.at(beam).distance),
									static_cast< int >( origin.y + std::sin( distanceStimuli->stimuli.at(beam).angle)*distanceStimuli->stimuli.at(beam).distance)};

					resultPointCloud.push_back(DistancePercept( endpoint));
				}
			}
		}
	}
	/**
	 *
	 */
	wxPoint Lidar::getDifferencePoint( const wxPoint& origin)
	{
		wxPoint differencePoint(0, 0);

		int64_t differencePointx = 0;
		int64_t differencePointy = 0;

		Application::MainSettings& mainSettings = Application::MainApplication::getSettings();
		std::random_device rd{};
		std::mt19937 gen{rd()};
		std::normal_distribution<> noise{0,mainSettings.getLidarStdDev()};

		for(std::size_t beam = 0; beam < lidarBeams; ++beam)
		{
			double angle = (double)beam * angleStep;

			DistanceStimulus distanceStimulus(angle,noDistance);

			std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();
			for (std::shared_ptr< Wall > wall : walls)
			{
				wxPoint wallPoint1 = wall->getPoint1();
				wxPoint wallPoint2 = wall->getPoint2();
				wxPoint laserEndpoint{static_cast<int>(origin.x + std::cos( angle) * lidarLaserBeamLength + noise(gen)) ,
									static_cast<int>(origin.y + std::sin( angle) * lidarLaserBeamLength + noise(gen))};

				wxPoint interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, origin, laserEndpoint);

				if(interSection != wxDefaultPosition)
				{
					double distance = Utils::Shape2DUtils::distance(origin,interSection);
					if(distance < distanceStimulus.distance || distanceStimulus.distance == noDistance)
					{
						distanceStimulus.distance = distance;
					}
				}
			}


			if(distanceStimulus.distance != noDistance)
			{
				wxPoint endpoint{	static_cast< int >( std::cos( distanceStimulus.angle)*distanceStimulus.distance),
								static_cast< int >( std::sin( distanceStimulus.angle)*distanceStimulus.distance)};

				differencePointx += endpoint.x;
				differencePointy += endpoint.y;
				differencePoint += endpoint;
			}
//			else
//			{
//				wxPoint endpoint{	static_cast< int >( std::cos( distanceStimulus.angle)*lidarLaserBeamLength),
//								static_cast< int >( std::sin( distanceStimulus.angle)*lidarLaserBeamLength)};
//
//				differencePoint += endpoint;
//			}
		}
//		return differencePoint;
//		std::cout << "diff point: " << differencePointx << ", " << differencePointy << std::endl;
		wxPoint result(differencePointx, differencePointy);

		if(differencePoint.x != differencePointx || differencePoint.y != differencePointy)
		{
			std::cout << "Aardappel" << std::endl;
		}

		return result;
	}
	/**
	 *
	 */
	double Lidar::getTotalDistance( const wxPoint& origin)
	{
		double totalDistance = 0.0;

		Application::MainSettings& mainSettings = Application::MainApplication::getSettings();
		std::random_device rd{};
		std::mt19937 gen{rd()};
		std::normal_distribution<> noise{0,mainSettings.getLidarStdDev()};

		for(std::size_t beam = 0; beam < lidarBeams; ++beam)
		{
			double angle = (double)beam * angleStep;

			DistanceStimulus distanceStimulus(angle,noDistance);

			std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();
			for (std::shared_ptr< Wall > wall : walls)
			{
				wxPoint wallPoint1 = wall->getPoint1();
				wxPoint wallPoint2 = wall->getPoint2();
				wxPoint laserEndpoint{static_cast<int>(origin.x + std::cos( angle) * lidarLaserBeamLength + noise(gen)) ,
									static_cast<int>(origin.y + std::sin( angle) * lidarLaserBeamLength + noise(gen))};

				wxPoint interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, origin, laserEndpoint);

				if(interSection != wxDefaultPosition)
				{
					double distance = Utils::Shape2DUtils::distance(origin,interSection);
					if(distance < distanceStimulus.distance || distanceStimulus.distance == noDistance)
					{
						distanceStimulus.distance = distance;
					}
				}
			}
			if(distanceStimulus.distance != noDistance)
			{
				totalDistance += distanceStimulus.distance;
			}
			else
			{
				totalDistance += lidarLaserBeamLength;
			}
		}
		return totalDistance;
	}
	/**
	 *
	 */
	std::string Lidar::asString() const
	{
		return "Lidar";
	}
	/**
	 *
	 */
	std::string Lidar::asDebugString() const
	{
		return asString();
	}
} // namespace Model





