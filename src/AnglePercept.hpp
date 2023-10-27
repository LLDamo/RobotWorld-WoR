/*
 * AnglePercept.hpp
 *
 *  Created on: Oct 23, 2023
 *      Author: Damo
 */

#ifndef ANGLEPERCEPT_HPP_
#define ANGLEPERCEPT_HPP_

#include "Config.hpp"

#include "AngleStimulus.hpp"
#include "Point.hpp"

#include <limits>

namespace Model
{
	/**
	 *
	 */
	class AnglePercept : public AbstractPercept
	{
		public:
			/**
			 *
			 */
			explicit AnglePercept( const double anAngle) :
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
				return "AnglePercept: " + std::to_string(angle);
			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override
			{
				return asString();
			}
	}; //	class AnglePercept
} // namespace Model

#endif /* ANGLEPERCEPT_HPP_ */
