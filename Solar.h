#ifndef SOLAR_H
#define SOLAR_H

#include <math.h>
#include <stdint.h>
#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#include <stdio.h>
#endif

//--------------------------------------------------------------------------------------------------
class Solar {
    // A class to calculate the direction and elevation to the position of the sun
    // It can also be used to prove that the earth is NOT flat :-)
public:
    Solar( double TimeZone, double Latitude, double Longitude );
    void setDateTime( int Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute );
    void setDate( int Year, uint8_t Month, uint8_t Day );
    void setTime( uint8_t Hour, uint8_t Minute );
    double SolarElevation( void );
    double SolarAzimuthAngle( void );
    double SunriseDegrees( void );
    double SunriseTime( void );
    double SunsetTime( void );
    double SolarNoon( void );
    double SunsetDegrees( void );
    bool DayTime( void );
    void DateCalc( char part, int offset );
    uint8_t GetHours( void );
    uint8_t GetMinutes( void );
    bool LeapYear( void );
#ifdef DEBUG_PRINT
    void DebugPrint(void);
#endif
private:
    long GregorianToJulian( int Year, uint8_t Month, uint8_t Day );
    double DecimalDate( void );
    double DecimalTime( void );
    double JulianDay( void );
    double JulianCentury( void );
    double GeomMeanLongSun( void );
    double GeomMeanAnomSun( void );
    double EccentEarthOrbit( void );
    double MeanObliqEcliptic( void );
    double ObliqCorr( void );
    double SunDeclin( void );
    double VarY( void );
    double SunAppLong( void );
    double SunEqOfCtr( void );
    double SunTrueLong( void );
    double TrueSolarTime( void );
    double HourAngle( void );
    double SolarZenithAngle( void );
    double EqOfTime( void );
    double SolarElevationAngle( void );
    double ApproxAtmosphericRefraction( void );
#ifndef ARDUINO
    double radians( double );
    double degrees( double );
#endif
    double m_TimeZone;
    double m_Latitude;
    double m_Longitude;
    int m_Year;
    uint8_t m_Month;
    uint8_t m_Day;
    uint8_t m_Hour;
    uint8_t m_Minute;
};
//--------------------------------------------------------------------------------------------------

#endif // SOLAR_H
