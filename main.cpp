#include <stdio.h>
#include <math.h>

// The functions used in this class were copied from the spreadsheets that were published on this website:
// https://gml.noaa.gov/grad/solcalc/calcdetails.html

// I did not notice any licence details when I copied the code. Let's assume a MIT license, so leave the comments intact.

// Please note that calculations are only valid for dates between 1901 and 2099, due to an approximation used in the Julian Day calculation.

// The purpose of the function main is for testing only, I developed this class in a Code::Blocks console project
// Once you've imported this class into a micro controller project,
// the function "main" and the include line for stdio.h can be discarded.

// Good luck,
// Hennie Peters
// Zevenaar, Netherlands

//--------------------------------------------------------------------------------------------------
class Solar {
    // A class to calculate the direction and elevation to the position of the sun
    // It can also be used to prove that the earth is NOT flat :-)
public:
    Solar( double TimeZone, double Latitude, double Longitude );
    void setDateTime( long Year, long Month, long Day, long Hour, long Minute );
    void setDate( long Year, long Month, long Day );
    void setTime( long Hour, long Minute );
    double SolarElevationCorrectedForAtmRefraction( void );
    double SolarAzimuthAngle( void );
    double SunriseDegrees( void );
    double SunriseTime( void );
    double SunsetTime( void );
    double SolarNoon( void );
    double SunsetDegrees( void );
    bool DayTime( void );
private:
    long GregorianToJulian( long Year, long Month, long Day );
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
    double radians( double );
    double degrees( double );
    double m_TimeZone;
    double m_Latitude;
    double m_Longitude;
    long m_Year;
    long m_Month;
    long m_Day;
    long m_Hour;
    long m_Minute;
};
//--------------------------------------------------------------------------------------------------
double Solar::radians( double degrees) {
    return 3.141592654 * degrees / 180.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::degrees( double radians ) {
    return 180.0 * radians / 3.141592654;
}
//--------------------------------------------------------------------------------------------------
Solar::Solar( double TimeZone, double Latitude, double Longitude ) {
    m_TimeZone  = TimeZone;
    m_Latitude  = Latitude;
    m_Longitude = Longitude;
}
//--------------------------------------------------------------------------------------------------
double Solar::DecimalDate( void ) {
    return (double) GregorianToJulian( m_Year, m_Month, m_Day );
}
//--------------------------------------------------------------------------------------------------
long Solar::GregorianToJulian( long Year, long Month, long Day ) { // Dublin JD, epoch Dec 31, 1899
    double Y = (double) Year;
    double M = (double) Month;
    double D = (double) Day;
    return (long) (1461.0*(Y+4800.0+(M-14.0)/12.0))/4.0+(367.0*(M-2.0-12.0*((M-14.0)/12.0)))/12.0-(3.0*((Y+4900.0+(M-14.0)/12.0)/100.0))/4.0+D-32075.0-2415020.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::DecimalTime( void ) { // DecimalTime (time past local midnight)
    return (double) ( m_Hour + ( m_Minute / 60.0 ) ) / 24.0;
}
//--------------------------------------------------------------------------------------------------
void Solar::setDateTime( long Year, long Month, long Day, long Hour, long Minute ) {
    m_Year   = Year;
    m_Month  = Month;
    m_Day    = Day;
    m_Hour   = Hour;
    m_Minute = Minute;
}
//--------------------------------------------------------------------------------------------------
void Solar::setDate( long Year, long Month, long Day ) {
    m_Year   = Year;
    m_Month  = Month;
    m_Day    = Day;
}
//--------------------------------------------------------------------------------------------------
void Solar::setTime( long Hour, long Minute ) {
    m_Hour   = Hour;
    m_Minute = Minute;
}
//--------------------------------------------------------------------------------------------------
double Solar::JulianDay( void ) {
    return DecimalDate() + 2415018.5 + DecimalTime() - m_TimeZone / 24.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::JulianCentury( void ) {
    return ( JulianDay() - 2451545.0 ) / 36525.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::GeomMeanLongSun( void ) {
    return fmod( 280.46646 + JulianCentury() * (36000.76983 + JulianCentury() * 0.0003032 ), 360.0 );
}
//--------------------------------------------------------------------------------------------------
double Solar::GeomMeanAnomSun( void ) {
    return 357.52911 + JulianCentury() * ( 35999.05029 - 0.0001537 * JulianCentury() );
}
//--------------------------------------------------------------------------------------------------
double Solar::EccentEarthOrbit( void ) {
    return 0.016708634 - JulianCentury() * ( 0.000042037 + 0.0000001267 * JulianCentury() );
}
//--------------------------------------------------------------------------------------------------
double Solar::MeanObliqEcliptic( void ) {
    return 23.0 + ( 26.0 + ( ( 21.448 - JulianCentury() * ( 46.815 + JulianCentury() * ( 0.00059 - JulianCentury() * 0.001813 ) ) ) ) / 60.0 ) / 60.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::ObliqCorr( void ) {
    return MeanObliqEcliptic() + 0.00256 * cos( radians( 125.04 - 1934.136 * JulianCentury() ) );
}
//--------------------------------------------------------------------------------------------------
double Solar::SunDeclin( void ) {
    return degrees( asin( sin( radians( ObliqCorr() ) ) * sin( radians( SunAppLong() ) ) ) );
}
//--------------------------------------------------------------------------------------------------
double Solar::VarY( void ) {
    return tan( radians( ObliqCorr() / 2.0 ) ) * tan( radians( ObliqCorr() / 2 ) );
}
//--------------------------------------------------------------------------------------------------
double Solar::SunAppLong( void ) {
    return SunTrueLong() - 0.00569 - 0.00478 * sin( radians( 125.04 - 1934.136 * JulianCentury() ) );
}
//--------------------------------------------------------------------------------------------------
double Solar::SunEqOfCtr( void ) {
    return sin( radians( GeomMeanAnomSun() ) ) * ( 1.914602 - JulianCentury() * ( 0.004817 + 0.000014 * JulianCentury() ) ) + sin( radians( 2.0 * GeomMeanAnomSun() ) ) * ( 0.019993 - 0.000101 * JulianCentury() ) + sin( radians( 3.0 * GeomMeanAnomSun() ) ) * 0.000289;
}
//--------------------------------------------------------------------------------------------------
double Solar::SunTrueLong( void ) {
    return GeomMeanLongSun() + SunEqOfCtr();
}
//--------------------------------------------------------------------------------------------------
double Solar::SunriseDegrees( void ) {
    return degrees( acos( cos( radians( 90.833 ) ) / ( cos( radians( m_Latitude ) ) * cos( radians( SunDeclin() ) ) ) - tan( radians( m_Latitude ) ) * tan( radians( SunDeclin() ) ) ) );
}
//--------------------------------------------------------------------------------------------------
double Solar::SunsetDegrees( void ) {
    return 360.0 - SunriseDegrees();
}
//--------------------------------------------------------------------------------------------------
double Solar::SunriseTime( void ) {
    return SolarNoon() - SunriseDegrees() * 4.0 / 1440.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::SunsetTime( void ) {
    return SolarNoon() + SunriseDegrees() * 4.0 / 1440.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::SolarNoon( void ) {
    return ( 720.0 - 4.0 * m_Longitude - EqOfTime() + m_TimeZone * 60.0 ) / 1440.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::TrueSolarTime( void ) {
    return fmod( DecimalTime() * 1440.0 + EqOfTime() + 4.0 * m_Longitude - 60.0 * m_TimeZone, 1440.0 );
}
//--------------------------------------------------------------------------------------------------
double Solar::EqOfTime( void ) {
    return 4.0 * degrees( VarY() * sin( 2.0 * radians( GeomMeanLongSun() ) ) - 2.0 * EccentEarthOrbit() * sin( radians( GeomMeanAnomSun() ) ) + 4.0 * EccentEarthOrbit() * VarY() * sin( radians( GeomMeanAnomSun() ) ) * cos( 2.0 * radians( GeomMeanLongSun() ) ) - 0.5 * VarY() * VarY() * sin( 4.0 * radians( GeomMeanLongSun() ) ) - 1.25 * EccentEarthOrbit() * EccentEarthOrbit() * sin( 2.0 * radians( GeomMeanAnomSun() ) ) );
}
//--------------------------------------------------------------------------------------------------
double Solar::HourAngle( void ) {
    double dTemp = TrueSolarTime() / 4.0;
    if( dTemp < 0 ) {
        return dTemp + 180.0;
    } else {
        return dTemp - 180.0;
    }
}
//--------------------------------------------------------------------------------------------------
double Solar::SolarZenithAngle( void ) {
    return degrees( acos( sin( radians( m_Latitude ) ) * sin( radians( SunDeclin() ) ) + cos( radians( m_Latitude ) ) * cos( radians( SunDeclin() ) ) * cos( radians( HourAngle() ) ) ) );
}
//--------------------------------------------------------------------------------------------------
double Solar::SolarElevationAngle( void ) {
    return 90.0 - SolarZenithAngle();
}
//--------------------------------------------------------------------------------------------------
double Solar::ApproxAtmosphericRefraction( void ) {
    if( SolarElevationAngle() > 85.0 ) {
        return 0;
    } else if( SolarElevationAngle() > 5.0 ) {
        return ( 58.1 / tan( radians( SolarElevationAngle() ) ) - 0.07 / pow( tan( radians( SolarElevationAngle() ) ), 3 ) + 0.000086 / pow( tan( radians( SolarElevationAngle() ) ), 5.0 ) ) / 3600.0;
    } else if( SolarElevationAngle() > -0.575 ) {
        return ( 1735.0 + SolarElevationAngle() * ( -518.2 + SolarElevationAngle() * ( 103.4 + SolarElevationAngle() * ( -12.79 + SolarElevationAngle() * 0.711 ) ) ) ) / 3600.0;
    } else {
        return ( -20.772 / tan( radians( SolarElevationAngle() ) ) ) / 3600.0;
    }
}
//--------------------------------------------------------------------------------------------------
double Solar::SolarElevationCorrectedForAtmRefraction( void ) {
    return SolarElevationAngle() + ApproxAtmosphericRefraction();
}
//--------------------------------------------------------------------------------------------------
double Solar::SolarAzimuthAngle( void ) {
    if( HourAngle() > 0.0 ) {
        return fmod( degrees( acos( ( ( sin( radians( m_Latitude ) ) * cos( radians( SolarZenithAngle() ) ) ) - sin( radians( SunDeclin() ) ) ) / ( cos( radians( m_Latitude ) ) * sin( radians( SolarZenithAngle() ) ) ) ) ) + 180.0, 360.0 );
    } else {
        return fmod( 540.0 - degrees( acos( ( ( sin( radians( m_Latitude ) ) * cos( radians( SolarZenithAngle() ) ) ) - sin( radians( SunDeclin() ) ) ) / ( cos( radians( m_Latitude ) ) * sin( radians( SolarZenithAngle() ) ) ) ) ), 360.0 );
    }
}
//--------------------------------------------------------------------------------------------------
bool Solar::DayTime( void ) {
    if( DecimalTime() >= SunriseTime() ) {
        if( DecimalTime() <= SunsetTime() ) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
//--------------------------------------------------------------------------------------------------
int main() {
    // Zevenaar Lat. 51.9374
    // Zevenaar Lon. 6.063
    // Zevenaar timezone GMT+0.48

    // The result for: 2025-06-21, 12:22:
    // SunriseDegrees: 03:44                        0.15597
    // SolarNoon:      12:06                        0.85265
    // SunriseTime:    20:27                        0.50431
    // SunsetTime:                                  0.66293
    // SunsetDegrees:                               234.59719
    // SolarElevationCorrectedForAtmRefraction:     61.34779
    // SolarAzimuthAngle:                           187.57022

    // parameters: Time zone, Latitude, Longitude
    Solar m_Solar( 0.48, 51.9374, 6.063 );

    // Use solar time, NOT daylight saving time, for obvious reasons :-)
    // parameters: yyyy, mm, dd, hh, min
    m_Solar.setDateTime( 2025, 6, 21, 12, 22 );

    printf( "\nDayTime(%02d:%02d):\t \t\t\t\t%s", 12, 22, m_Solar.DayTime() ? "true" : "false" );
    printf( "\nSunriseDegrees:\t\t\t\t\t%.5f", m_Solar.SunriseDegrees() );
    printf( "\nSunriseTime:\t%02d:%02d\t\t\t\t%.5f\t", (int)(m_Solar.SunriseTime() * 24.0), (int)(m_Solar.SunriseTime()* 1440) % 60, m_Solar.SunriseTime() );
    printf( "\nSolarNoon:\t%02d:%02d\t\t\t\t%.5f\t", (int)(m_Solar.SolarNoon() * 24.0), (int)(m_Solar.SolarNoon()* 1440) % 60, m_Solar.SolarNoon() );
    printf( "\nSunsetTime:\t%02d:%02d\t\t\t\t%.5f\t", (int)(m_Solar.SunsetTime() * 24.0), (int)(m_Solar.SunsetTime()* 1440) % 60, m_Solar.SunsetTime() );
    printf( "\nSunsetDegrees:\t\t\t\t\t%.5f", m_Solar.SunsetDegrees() );
    printf( "\nSolarElevationCorrectedForAtmRefraction:\t%.5f", m_Solar.SolarElevationCorrectedForAtmRefraction() );
    printf( "\nSolarAzimuthAngle:\t\t\t\t%.5f", m_Solar.SolarAzimuthAngle() );
    printf( "\n");
    return 0;
}
//--------------------------------------------------------------------------------------------------
