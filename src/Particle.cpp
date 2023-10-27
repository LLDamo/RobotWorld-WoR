/*
 * Particle.cpp
 *
 *  Created on: Oct 4, 2023
 *      Author: Damo
 */

#include "Particle.hpp"

#include "Lidar.hpp"

#include <sstream>

namespace Model
{

	Particle::Particle() : position(wxDefaultPosition)
	{
//		pointCloud = Lidar::getPerceptFor(wxDefaultPosition);
	}

	Particle::Particle(wxPoint position_): position(position_)
	{
//		pointCloud = Lidar::getPerceptFor(position_);
	}


	Particle::~Particle() {

	}

	void Particle::setPosition(wxPoint newPosition)
	{
		this->position = newPosition;
	}

	wxPoint Particle::getPosition() const
	{
		return this->position;
	}

//	void Particle::fillPointCloud()
//	{
//		this->pointCloud = Lidar::getPerceptFor(position);
//	}

	void Particle::getPointCloud(PointCloud& pointCloud)
	{
		Lidar::getPerceptFor(position, pointCloud);
	}

} // namespace Model
