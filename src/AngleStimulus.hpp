/*
 * AngleStimulus.hpp
 *
 *  Created on: Oct 23, 2023
 *      Author: Damo
 */

#ifndef ANGLESTIMULUS_HPP_
#define ANGLESTIMULUS_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"

#include <limits>

namespace Model
{
	/**
	 *
	 */
	class AngleStimulus : public AbstractStimulus
	{
		public:
			/**
			 *
			 */
			AngleStimulus( 	double anAngle) :
				angle(anAngle)
			{
			}
			double angle;
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const override
			{
				return "AngleStimulus: " + std::to_string(angle);
			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override
			{
				return asString();
			}
	}; // class AngleStimulus
} // namespace Model

#endif /* ANGLESTIMULUS_HPP_ */
