/*
 * Particle.hpp
 *
 *  Created on: Oct 4, 2023
 *      Author: Damo
 */

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include "Config.hpp"

#include "AbstractAgent.hpp"
#include "DistancePercepts.hpp"
#include "Observer.hpp"
#include "Point.hpp"

#include <iostream>

namespace Model
{

	class Particle;
	typedef std::shared_ptr< Particle > ParticlePtr;

	class Particle
	{
	public:
		explicit Particle();

		Particle(wxPoint position_);

		virtual ~Particle();

		void setPosition(wxPoint newPosition);

		wxPoint getPosition() const;

//		void fillPointCloud();

		void getPointCloud(PointCloud& pointCloud);

		/**
		 *
		 */
//		PointCloud pointCloud;
	private:
		/**
		 *
		 */
		wxPoint position;

	};
} // namespace Model

#endif /* PARTICLE_HPP_ */
