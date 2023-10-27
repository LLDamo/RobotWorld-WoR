#ifndef MAINSETTINGS_HPP_
#define MAINSETTINGS_HPP_

#include "Config.hpp"

namespace Application
{

	/*
	 *
	 */
	class MainSettings
	{
		public:
			/**
			 *
			 */
			MainSettings();
			/**
			 *
			 */
			virtual ~MainSettings();
			/**
			 *
			 */
			bool getDrawOpenSet() const;
			/**
			 *
			 */
			void setDrawOpenSet( bool aDrawOpenSet);
			/**
			 *
			 */
			unsigned long getSpeed() const;
			/**
			 *
			 */
			void setSpeed( unsigned long aSpeed);
			/**
			 *
			 */
			unsigned long getWorldNumber() const;
			/**
			 *
			 */
			void setWorldNumber( unsigned long aWorldNumber);
			/**
			 *
			 */
			bool getDrawParticleFilter() const;
			/**
			 *
			 */
			void setDrawParticleFilter( bool aDrawParticleFilter);
			/**
			 *
			 */
			bool getDrawKalmanFilter() const;
			/**
			 *
			 */
			void setDrawKalmanFilter( bool aDrawKalmanFilter);
			/**
			 *
			 */
			double getCompasStdDev() const;
			/**
			 *
			 */
			void setCompasStdDev(double compasStdDev);
			/**
			 *
			 */
			double getLidarStdDev() const;
			/**
			 *
			 */
			void setLidarStdDev(double lidarStdDev);
			/**
			 *
			 */
			double getOdometerStdDev() const;
			/**
			 *
			 */
			void setOdometerStdDev(double odometerStdDev);
			/**
			 *
			 */
			void saveConfiguration();
			/**
			 *
			 */
			void loadConfiguration();

		private:
			bool drawOpenSet;
			unsigned long speed;
			double lidarStdDev;
			double compasStdDev;
			double odometerStdDev;
			unsigned long worldNumber;
			bool drawParticleFilter;
			bool drawKalmanFilter;

	};

} /* namespace Application */

#endif /* SRC_MAINSETTINGS_HPP_ */
