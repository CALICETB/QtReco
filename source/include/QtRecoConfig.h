#ifndef QtRecoConfig_H
#define QtRecoConfig_H

// version macros
#define QtReco_MAJOR_VERSION 
#define QtReco_MINOR_VERSION 
#define QtReco_PATCH_LEVEL 
#define QtReco_VERSION_STR ".."
#define QtReco_VERSION_GE( MAJV , MINV , PLEV )  \
     ( (QtReco_MAJOR_VERSION > MAJV) || ( (QtReco_MAJOR_VERSION == MAJV) && (QtReco_MINOR_VERSION > MINV) ) \
  || ( (QtReco_MAJOR_VERSION == MAJV) && (QtReco_MINOR_VERSION == MINV) && (QtReco_PATCH_LEVEL >= PLEV) ) )

// project source dir
// useful for icons
#define QtReco_DIR "/home/calice/Desktop/AHCAL-Reconstruction"

#endif // QtRecoConfig_H
