#include "Solar.h"

//--------------------------------------------------------------------------------------------------

/*
    I am running this class myself on two locations: "Zevenaar, Netherlands" and "Emmerich am Rhein, Germany"
    Find your exact location and calculate the timezone for accurate solar time like the two examples below:

    Zevenaar is located at approximately 6.08°E.
    The Earth rotates 360/24 = 15° per hour, so 1° = 4 minutes = 1/15 hour.
    Solar time at 6.08°E is: 6.0565/15 = 0.4053 hours ahead of UTC.
    Thus, the solar time offset for Zevenaar is approximately UTC+0.4038
    class Solar m_Solar( 0.4053, 51.9291, 6.0565 );

    Emmerich am Rhein is located at approximately 6.35°E.
    The Earth rotates 15° per hour, so 1° = 4 minutes = 1/15 hour.
    Solar time at 6.35°E is: 6.35/15=0.4233 hours ahead of UTC.
    Thus, the solar time offset for Emmerich am Rhein is approximately UTC+0.4233
    class Solar m_Solar( 0.4233, 51.8520, 6.2598 );

*/

#ifdef ARDUINO // tested in Arduino 1.8.19
//--------------------------------------------------------------------------------------------------
// parameters: Time zone, Latitude, Longitude
Solar m_Solar( 0.4038, 51.9291, 6.0565 ); // Zevenaar

void setup( void ) {
    // parameters: yyyy, mm, dd, hh, min
    // Use solar time, NOT daylight saving time, for obvious reasons :-)
    m_Solar.setDateTime( 2025, 6, 21, 12, 01 );
}

void loop() {
    while( 1 ) {
    } // end while
}
//--------------------------------------------------------------------------------------------------
#else // test environment, Code::Blocks on Linux
//--------------------------------------------------------------------------------------------------
int main() {
    // parameters: Time zone, Latitude, Longitude
    Solar m_Solar( 0.4038, 51.9291, 6.0565 ); // Zevenaar

    // parameters: yyyy, mm, dd, hh, min
    // Use solar time, NOT daylight saving time, for obvious reasons :-)
    m_Solar.setDateTime( 2025, 6, 21, 12, 01 );

    printf( "Leap years marked with an asterisk:\n\n" );
    for( int i = 0; i < 50; i++ ) {
        m_Solar.DateCalc( 'Y', -25 );
        printf( "  DateCalc( 'Y', -25 );\t" );
        m_Solar.DebugPrint();
    }

    printf( "\nDay time on: " );
    m_Solar.setDateTime( 2025, 6, 21, 0, 0 );
    for( int i = 0; i < 144; i++ ) {
        m_Solar.DateCalc( 'm', +10 );
        printf( "  DateCalc( 'm', 10 );\t" );
        printf( "%02d:%02d - %s\n", m_Solar.GetHours(), m_Solar.GetMinutes(), m_Solar.DayTime() ? "true" : "false" );
    }
    printf( "\n" );

    printf( "\nDayTime(%02d:%02d):\t \t%s", m_Solar.GetHours(), m_Solar.GetMinutes(), m_Solar.DayTime() ? "true" : "false" );
    printf( "\nSunriseDegrees:\t\t%.5f", m_Solar.SunriseDegrees() );
    printf( "\nSunriseTime:\t%02d:%02d\t%.5f\t", ( int )( m_Solar.SunriseTime() * 24.0 ), ( int )( m_Solar.SunriseTime() * 1440 ) % 60, m_Solar.SunriseTime() );
    printf( "\nSolarNoon:\t%02d:%02d\t%.5f\t", ( int )( m_Solar.SolarNoon() * 24.0 ), ( int )( m_Solar.SolarNoon() * 1440 ) % 60, m_Solar.SolarNoon() );
    printf( "\nSunsetTime:\t%02d:%02d\t%.5f\t", ( int )( m_Solar.SunsetTime() * 24.0 ), ( int )( m_Solar.SunsetTime() * 1440 ) % 60, m_Solar.SunsetTime() );
    printf( "\nSunsetDegrees:\t\t%.5f", m_Solar.SunsetDegrees() );
    printf( "\nSolarElevation:\t\t%.5f", m_Solar.SolarElevation() );
    printf( "\nSolarAzimuthAngle:\t%.5f", m_Solar.SolarAzimuthAngle() );
    printf( "\n" );

    return 0;
}
//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
