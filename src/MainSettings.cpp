#include "MainSettings.hpp"

#include <wx/xml/xml.h>
#include <iostream>

namespace Application
{
	/**
	 *
	 */
	MainSettings::MainSettings() : drawOpenSet(true), speed(10), worldNumber(0), drawParticleFilter(false), drawKalmanFilter(false)
	{
		wxXmlDocument doc;
		if (doc.Load("sensorConfig.xml"))
		{
			if (doc.GetRoot()->GetName() == "root")
			{
				wxXmlNode *child = doc.GetRoot()->GetChildren();
				while (child)
				{
					if (child->GetName() == "Lidar")
					{
						wxString content = child->GetAttribute("stdDev");
						lidarStdDev = std::stod(content.ToStdString());
					}
					else if (child->GetName() == "Compas")
					{
						wxString content = child->GetAttribute("stdDev");
						compasStdDev = std::stod(content.ToStdString());
					}
					else if (child->GetName() == "Odometer")
					{
						wxString content = child->GetAttribute("stdDev");
						odometerStdDev = std::stod(content.ToStdString());
					}

					child = child->GetNext();
				}
			}
		}
		else
		{
			std::cout << "could not open file to initialize" << std::endl;
		}
	}
	/**
	 *
	 */
	MainSettings::~MainSettings()
	{
	}
	/**
	 *
	 */
	void MainSettings::saveConfiguration()
	{
		wxXmlDocument doc;
		if (doc.Load("sensorConfig.xml"))
		{
			if (doc.GetRoot()->GetName() == "root")
			{
				wxXmlNode *child = doc.GetRoot()->GetChildren();
				while (child)
				{
					if (child->GetName() == "Lidar")
					{
						if(child->DeleteAttribute("stdDev"))
						{
							child->AddAttribute("stdDev", std::to_string(lidarStdDev));
						}
					}
					else if (child->GetName() == "Compas")
					{
						if(child->DeleteAttribute("stdDev"))
						{
							child->AddAttribute("stdDev", std::to_string(compasStdDev));
						}
					}
					else if (child->GetName() == "Odometer")
					{
						if(child->DeleteAttribute("stdDev"))
						{
							child->AddAttribute("stdDev", std::to_string(odometerStdDev));
						}
					}

					child = child->GetNext();
				}
			}
			doc.Save("sensorConfig.xml");
		}
		else
		{
			std::cout << "could not open file to save" << std::endl;
		}
	}
	/**
	 *
	 */
	void MainSettings::loadConfiguration()
	{
		wxXmlDocument doc;
		if (doc.Load("sensorConfig.xml"))
		{
			if (doc.GetRoot()->GetName() == "root")
			{
				wxXmlNode *child = doc.GetRoot()->GetChildren();
				while (child)
				{
					if (child->GetName() == "Lidar")
					{
						wxString content = child->GetAttribute("stdDev");
						lidarStdDev = std::stod(content.ToStdString());
					}
					else if (child->GetName() == "Compas")
					{
						wxString content = child->GetAttribute("stdDev");
						compasStdDev = std::stod(content.ToStdString());
					}
					else if (child->GetName() == "Odometer")
					{
						wxString content = child->GetAttribute("stdDev");
						odometerStdDev = std::stod(content.ToStdString());
					}

					child = child->GetNext();
				}
			}
		}
		else
		{
			std::cout << "could not open file to load" << std::endl;
		}
	}
	/**
	 *
	 */
	bool MainSettings::getDrawOpenSet() const
	{
		return drawOpenSet;
	}
	/**
	 *
	 */
	void MainSettings::setDrawOpenSet( bool aDrawOpenSet)
	{
		drawOpenSet = aDrawOpenSet;
	}
	/**
	 *
	 */
	unsigned long MainSettings::getSpeed() const
	{
		return speed;
	}
	/**
	 *
	 */
	void MainSettings::setSpeed( unsigned long aSpeed)
	{
		speed = aSpeed;
	}
	/**
	 *
	 */
	unsigned long MainSettings::getWorldNumber() const
	{
		return worldNumber;
	}
	/**
	 *
	 */
	void MainSettings::setWorldNumber( unsigned long aWorldNumber)
	{
		worldNumber = aWorldNumber;
	}
	/**
	 *
	 */
	bool MainSettings::getDrawParticleFilter() const
	{
		return drawParticleFilter;
	}
	/**
	 *
	 */
	void MainSettings::setDrawParticleFilter( bool aDrawParticleFilter)
	{
		drawParticleFilter = aDrawParticleFilter;
	}
	/**
	 *
	 */
	bool MainSettings::getDrawKalmanFilter() const
	{
		return drawKalmanFilter;
	}
	/**
	 *
	 */
	void MainSettings::setDrawKalmanFilter( bool aDrawKalmanFilter)
	{
		drawKalmanFilter = aDrawKalmanFilter;
	}

	double MainSettings::getCompasStdDev() const {
		return compasStdDev;
	}

	void MainSettings::setCompasStdDev(double compasStdDev) {
		this->compasStdDev = compasStdDev;
	}

	double MainSettings::getLidarStdDev() const {
		return lidarStdDev;
	}

	void MainSettings::setLidarStdDev(double lidarStdDev) {
		this->lidarStdDev = lidarStdDev;
	}

	double MainSettings::getOdometerStdDev() const {
		return odometerStdDev;
	}

	void MainSettings::setOdometerStdDev(double odometerStdDev) {
		this->odometerStdDev = odometerStdDev;
	}

} /* namespace Application */
