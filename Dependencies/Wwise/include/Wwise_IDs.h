/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Audiokinetic Wwise generated include file. Do not edit.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WWISE_IDS_H__
#define __WWISE_IDS_H__

#include <AK/SoundEngine/Common/AkTypes.h>

namespace AK
{
    namespace EVENTS
    {
        static const AkUniqueID DISABLEBYPASS = 2190380339U;
        static const AkUniqueID ENABLEBYPASS = 3559920456U;
        static const AkUniqueID PLAY = 1256202815U;
        static const AkUniqueID PLAYBACKGROUNDMUSIC = 3115765196U;
    } // namespace EVENTS

    namespace STATES
    {
        namespace MUSICBACKGROUND
        {
            static const AkUniqueID GROUP = 2073497030U;

            namespace STATE
            {
                static const AkUniqueID MUSIC1 = 1730564819U;
                static const AkUniqueID MUSIC2 = 1730564816U;
                static const AkUniqueID NONE = 748895195U;
            } // namespace STATE
        } // namespace MUSICBACKGROUND

    } // namespace STATES

    namespace SWITCHES
    {
        namespace MAIN
        {
            static const AkUniqueID GROUP = 3161908922U;

            namespace SWITCH
            {
                static const AkUniqueID BIRD = 1544377644U;
                static const AkUniqueID CAR = 983016381U;
            } // namespace SWITCH
        } // namespace MAIN

    } // namespace SWITCHES

    namespace GAME_PARAMETERS
    {
        static const AkUniqueID PLAYBACK_RATE = 1524500807U;
        static const AkUniqueID RPM = 796049864U;
        static const AkUniqueID SS_AIR_FEAR = 1351367891U;
        static const AkUniqueID SS_AIR_FREEFALL = 3002758120U;
        static const AkUniqueID SS_AIR_FURY = 1029930033U;
        static const AkUniqueID SS_AIR_MONTH = 2648548617U;
        static const AkUniqueID SS_AIR_PRESENCE = 3847924954U;
        static const AkUniqueID SS_AIR_RPM = 822163944U;
        static const AkUniqueID SS_AIR_SIZE = 3074696722U;
        static const AkUniqueID SS_AIR_STORM = 3715662592U;
        static const AkUniqueID SS_AIR_TIMEOFDAY = 3203397129U;
        static const AkUniqueID SS_AIR_TURBULENCE = 4160247818U;
    } // namespace GAME_PARAMETERS

    namespace BANKS
    {
        static const AkUniqueID INIT = 1355168291U;
        static const AkUniqueID MAIN = 3161908922U;
    } // namespace BANKS

    namespace BUSSES
    {
        static const AkUniqueID MASTER_AUDIO_BUS = 3803692087U;
        static const AkUniqueID MOTION_FACTORY_BUS = 985987111U;
    } // namespace BUSSES

    namespace AUX_BUSSES
    {
        static const AkUniqueID NORMAL = 1160234136U;
        static const AkUniqueID REVERB = 348963605U;
    } // namespace AUX_BUSSES

    namespace AUDIO_DEVICES
    {
        static const AkUniqueID DEFAULT_MOTION_DEVICE = 4230635974U;
        static const AkUniqueID NO_OUTPUT = 2317455096U;
        static const AkUniqueID SYSTEM = 3859886410U;
    } // namespace AUDIO_DEVICES

}// namespace AK

#endif // __WWISE_IDS_H__
