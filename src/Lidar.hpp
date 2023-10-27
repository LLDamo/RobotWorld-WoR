/*
 * Lidar.hpp
 *
 *  Created on: Oct 4, 2023
 *      Author: Damo
 */

#ifndef LIDAR_HPP_
#define LIDAR_HPP_


#include "Config.hpp"

#include "AbstractSensor.hpp"
#include "DistancePercepts.hpp"
#include "DistanceStimuli.hpp"

#include <math.h>

namespace Model
{
	/**
	 * Compile time configurable length of the laser beam
	 */
	const short int lidarLaserBeamLength = 2048;

	const short int lidarBeams = 180;

	const double angleStep = (2 * M_PI / (double)lidarBeams);

	class Robot;
	typedef std::shared_ptr<Robot> RobotPtr;

	class Particle;
	typedef std::shared_ptr<Particle> ParticlePtr;

	/**
	 *
	 */
	class Lidar : public AbstractSensor
	{
		public:
			/**
			 *
			 */
			explicit Lidar( Robot& aRobot);
			/**
			 *
			 */
			virtual std::shared_ptr< AbstractStimulus > getStimulus() const override;
			/**
			 *
			 */
			virtual std::shared_ptr< AbstractPercept > getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const override;
			/**
			 *
			 */
			static std::shared_ptr< DistanceStimuli > getStimulus( const wxPoint& origin);
			/**
			 *
			 */
			static 	void getPerceptFor( const wxPoint& origin, PointCloud& resultPointCloud);
			/**
			 *
			 */
			static wxPoint getDifferencePoint( const wxPoint& origin);
			/**
			 *
			 */
			static double getTotalDistance( const wxPoint& origin);
			/**
			 *
			 */
			static void setStdDev(double aStdDev) {Lidar::stddev = aStdDev;}
			/**
			 *
			 */
			static double getStdDev(){ return stddev;}
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const override;
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override;
			//@}
		protected:
		private:
			/**
			 * Standard deviation of the odometer per 10 pixels
			 */
			static double stddev;
	};
} // namespace Model


#endif /* LIDAR_HPP_ */
