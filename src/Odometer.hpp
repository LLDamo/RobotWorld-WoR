/*
 * Odometer.hpp
 *
 *  Created on: Oct 23, 2023
 *      Author: Damo
 */

#ifndef ODOMETER_HPP_
#define ODOMETER_HPP_

#include "Config.hpp"
#include "Point.hpp"

#include <memory>
#include <iostream>

namespace Model
{


	class Robot;
	typedef std::shared_ptr<Robot> RobotPtr;

	class Odometer
	{
	public:
		/**
		 *
		 */
		Odometer(const wxPoint& initialRobotPosition);
		/**
		 *
		 */
		double getStimulus(const wxPoint& robotPosition);
		/**
		 *
		 */
		double getPerceptFor(const wxPoint& robotPosition);
		/**
		 *
		 */
		double getDistance();
		/**
		 *
		 */
		void setPrevPosition(const wxPoint& prevPosition_);
		/**
		 *
		 */
		static void setStdDev(double aStdDev) {Odometer::stddev = aStdDev;}
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
		std::string asString() const;
		/**
		 * Returns a description of the object with all data of the object usable for debugging
		 */
		std::string asDebugString() const;
		//@}
	protected:
	private:
		/**
		 * Standard deviation of the odometer per 10 pixels
		 */
		static double stddev;

		wxPoint prevPosition;

		double distance;
	};
} // namespace Model
#endif /* ODOMETER_HPP_ */
