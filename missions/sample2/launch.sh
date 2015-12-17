#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
VARIATION="0"
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES] [time_warp]   \n" $0
	printf "  --just_make, -j    \n" 
	printf "  --help, -h         \n" 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "v1" ] ; then
	VARIATION="1"
    elif [ "${ARGI}" = "v2" ] ; then
	VARIATION="2"
    elif [ "${ARGI}" = "v3" ] ; then
	VARIATION="3"
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------
VNUM1=0
VNUM2=1
VNAME1="v$VNUM1"           # The first vehicle Community
VNAME2="v$VNUM2"           # The second vehicle Community
VCOLOR1="orange"
VCOLOR2="cyan"
START_POS1="0,0"         
START_POS2="80,0"        
LOITER_POS1="x=0,y=-75"
LOITER_POS2="x=125,y=-50"
SHORE_LISTEN="9300"

nsplug meta_vehicle.moos targ_$VNAME1.moos -f WARP=$TIME_WARP \
    VNUM=$VNUM1            VNAME=$VNAME1          SHARE_LISTEN="9301"              \
    VPORT="9001"           SHORE_LISTEN=$SHORE_LISTEN       \
    START_POS=$START_POS1  VARIATION=$VARIATION             

nsplug meta_vehicle.moos targ_$VNAME2.moos -f WARP=$TIME_WARP \
    VNUM=$VNUM2            VNAME=$VNAME2          SHARE_LISTEN="9302"              \
    VPORT="9002"           SHORE_LISTEN=$SHORE_LISTEN       \
    START_POS=$START_POS2  VARIATION=$VARIATION             

nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
    SNAME="shoreside"  SHARE_LISTEN=$SHORE_LISTEN                 \
    SPORT="9000"       VARIATION=$VARIATION         

nsplug meta_vehicle.bhv targ_$VNAME1.bhv -f VNAME=$VNAME1     \
    VNUM=$VNUM1     VNAME=$VNAME1       VCOLOR=$VCOLOR1       \
    START_POS=$START_POS1 LOITER_POS=$LOITER_POS1       

nsplug meta_vehicle.bhv targ_$VNAME2.bhv -f VNAME=$VNAME2     \
    VNUM=$VNUM2     VNAME=$VNAME2       VCOLOR=$VCOLOR2       \
    START_POS=$START_POS2 LOITER_POS=$LOITER_POS2       

if [ ! -e targ_$VNAME1.moos ]; then echo "no targ_$VNAME1.moos"; exit; fi
if [ ! -e targ_$VNAME1.bhv  ]; then echo "no targ_$VNAME1.bhv "; exit; fi
if [ ! -e targ_$VNAME2.moos ]; then echo "no targ_$VNAME2.moos"; exit; fi
if [ ! -e targ_$VNAME2.bhv  ]; then echo "no targ_$VNAME2.bhv "; exit; fi
if [ ! -e targ_shoreside.moos ]; then echo "no targ_shoreside.moos";  exit; fi

if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi


#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching $SNAME MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler targ_shoreside.moos >& thelog_shoreside.txt & # >& /dev/null &
printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME1.moos >& thelog_$VNAME1.txt & # >& /dev/null &
printf "Launching $VNAME2 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME2.moos >& thelog_$VNAME2.txt & # >& /dev/null &
printf "Done \n"

uMAC targ_shoreside.moos

printf "Killing all processes ... \n"
mykill
pkill xterm
pkill pAuctioneer
pkill pBidder
printf "Done killing processes.   \n"
ps -a
