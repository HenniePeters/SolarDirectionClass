#include "Solar.h"

// The functions used in the Solar class were copied from spreadsheets published on the website of the Global Monitoring Laboratory:
// https://gml.noaa.gov/grad/solcalc/calcdetails.html

// I did noticed this text on the noaa site so leave the comment below intact:

// The information on government servers are in the public domain, unless specifically annotated otherwise, and may be used freely by the public
// so long as you do not:
// 1) claim it is your own (e.g. by claiming copyright for NOAA information – see next paragraph),
// 2) use it in a manner that implies an endorsement or affiliation with NOAA,
//or
// 3) modify it in content and then present it as official government material.
// You also cannot present information of your own in a way that makes it appear to be official government information.

// Please note that calculations are only valid for dates between 1901 and 2099, due to an approximation used in the Julian Day calculation.

// The purpose of the functions main and debugprint is for testing only, I developed this class in a Code::Blocks C++ console project
// Once you've imported this class into a micro controller project, the functions "main", "debugprint" and the include line for stdio.h can be discarded.

// Good luck,
// Hennie Peters
// Zevenaar, Netherlands

//--------------------------------------------------------------------------------------------------
#ifndef ARDUINO
double Solar::radians( double degrees ) {
    return degrees * 3.141592654 / 180.0;
}
#endif
//--------------------------------------------------------------------------------------------------
#ifndef ARDUINO
double Solar::degrees( double radians ) {
    return radians * 180.0 / 3.141592654;
}
#endif
//--------------------------------------------------------------------------------------------------
uint8_t Solar::GetHours( void ) {
    return m_Hour;
}
//--------------------------------------------------------------------------------------------------
uint8_t Solar::GetMinutes( void ) {
    return m_Minute;
}
//--------------------------------------------------------------------------------------------------
Solar::Solar( double TimeZone, double Latitude, double Longitude ) {
    m_TimeZone  = TimeZone;
    m_Latitude  = Latitude;
    m_Longitude = Longitude;
}
//--------------------------------------------------------------------------------------------------
double Solar::DecimalDate( void ) {
    return ( double ) GregorianToJulian( m_Year, m_Month, m_Day );
}
//--------------------------------------------------------------------------------------------------
long Solar::GregorianToJulian( int Year, uint8_t Month, uint8_t Day ) { // Dublin JD, epoch Dec 31, 1899
    double Y = ( double ) Year;
    double M = ( double ) Month;
    double D = ( double ) Day;
    return ( long )( 1461.0 * ( Y + 4800.0 + ( M - 14.0 ) / 12.0 ) ) / 4.0 + ( 367.0 * ( M - 2.0 - 12.0 * ( ( M - 14.0 ) / 12.0 ) ) ) / 12.0 - ( 3.0 * ( ( Y + 4900.0 + ( M - 14.0 ) / 12.0 ) / 100.0 ) ) / 4.0 + D - 32075.0 - 2415020.0;
}
//--------------------------------------------------------------------------------------------------
double Solar::DecimalTime( void ) { // DecimalTime (time past local midnight, 0 = midnight, 0.5 = noon, 1.0 = midnight again)
    return ( double )( m_Hour + ( m_Minute / 60.0 ) ) / 24.0;
}
//--------------------------------------------------------------------------------------------------
void Solar::setDateTime( int Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute ) {
    setDate( Year, Month, Day );
    setTime( Hour, Minute );
}
//--------------------------------------------------------------------------------------------------
void Solar::setDate( int Year, uint8_t Month, uint8_t Day ) {
    m_Year   = Year;
    m_Month  = Month;
    m_Day    = Day;
}
//--------------------------------------------------------------------------------------------------
void Solar::setTime( uint8_t Hour, uint8_t Minute ) {
    m_Hour   = Hour;
    m_Minute = Minute;
#ifdef DEBUG_PRINT
    DebugPrint();
#ifdef ARDUINO
    Serial.print( "\n" );
#else
    printf( "\n" );
#endif
#endif
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
    return fmod( 280.46646 + JulianCentury() * ( 36000.76983 + JulianCentury() * 0.0003032 ), 360.0 );
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
double Solar::SolarElevation( void ) { // Corrected for atm. refraction
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
bool Solar::LeapYear( void ) {
    return ( m_Year % 4 == 0 ) ? ( m_Year % 100 == 0 ) ? ( m_Year % 400 == 0 ) ? true : false : true : false;
}
//--------------------------------------------------------------------------------------------------
void Solar::DateCalc( char part, int offset ) {
    int32_t tmp;
    int8_t feb = LeapYear() ? 29 :  28;
    const int8_t months[] = { 0, 31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    switch( part ) {
        case 'Y':
            m_Year += offset;
            break;
        case 'M':
            tmp = m_Month;
            tmp += offset;
            while( tmp > 12 ) {
                tmp -= 12;
                DateCalc( 'Y', 1 );
            }
            while( tmp < 0 ) {
                tmp += 12;
                DateCalc( 'Y', -1 );
            }
            m_Month = tmp;
            break;
        case 'D':
            tmp = m_Day;
            tmp += offset;
            while( tmp > months[m_Month] ) {
                tmp -= months[m_Month];
                DateCalc( 'M', 1 );
            }
            while( tmp < 0 ) {
                tmp += months[m_Month];
                DateCalc( 'M', -1 );
            }
            m_Day = tmp;
            break;
        case 'h':
            tmp = m_Hour;
            tmp += offset;
            while( tmp > 23 ) {
                tmp -= 24;
                DateCalc( 'D', 1 );
            }
            while( tmp < 0 ) {
                tmp += 24;
                DateCalc( 'D', -1 );
            }
            m_Hour = tmp;
            break;
        case 'm':
            tmp = m_Minute;
            tmp += offset;
            while( tmp > 59 ) {
                tmp -= 60;
                DateCalc( 'h', 1 );
            }
            while( tmp < 0 ) {
                tmp += 60;
                DateCalc( 'h', -1 );
            }
            m_Minute = tmp;
            break;
    }
}
//--------------------------------------------------------------------------------------------------
#ifdef DEBUG_PRINT
void Solar::DebugPrint( void ) {
#ifdef ARDUINO
    char buf[32];
    sprintf( buf, "%c %04d-%02d-%02d %02d:%02d\n", ( LeapYear() == true ) ? '*' : '-', m_Year, m_Month, m_Day, m_Hour, m_Minute );
    Serial.print( buf );
#else
    printf( "%c %04d-%02d-%02d %02d:%02d\n", ( LeapYear() == true ) ? '*' : '-', m_Year, m_Month, m_Day, m_Hour, m_Minute );
#endif
}
#endif
//--------------------------------------------------------------------------------------------------
