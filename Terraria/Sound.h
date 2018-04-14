#ifndef _SOUND_INCLUDE
#define _SOUND_INCLUDE

#include "fmod.hpp"
#include "fmod_errors.h"

using namespace std;

//Sound array size
#define NUM_SOUNDS 23

//Sound identifiers
#define	SOUND_AMBIENT1      0
#define	SOUND_AMBIENT2      1
#define SOUND_TITLESCREEN   2
#define	SOUND_JUNGLE        3
#define	SOUND_JUMP          4
#define	SOUND_PLAYERHIT     5
#define SOUND_PLAYERHIT2	6
#define SOUND_PLAYERHIT3	7
#define SOUND_PLAYERKILLED	8
#define	SOUND_BOSS          9
#define	SOUND_RUN           10
#define	SOUND_MINING        11
#define SOUND_ZOMBIE		12
#define SOUND_ZOMBIE2		13
#define SOUND_ZOMBIE3		14
#define SOUND_MONSTERKILLED	15
#define SOUND_GRAB			16
#define SOUND_DIG			17
#define SOUND_SWORD			18
#define SOUND_PUT			19
#define SOUND_MENUOPEN		20
#define SOUND_MENUCLOSE		21
#define SOUND_NPCHIT		22

class Sound
{
public:
	Sound(void);
	virtual ~Sound(void);

	bool Load();
	void Play(int sound_id);
	void StopAll();
	void Update();

	FMOD::System* system; //handle to FMOD engine
	FMOD::Sound* sounds[NUM_SOUNDS]; //sound that will be loaded and played
	FMOD::Channel* ambient1Channel;
	FMOD::Channel* ambient2Channel;
	FMOD::Channel* titlescreenChannel;
	FMOD::Channel* jungleChannel;
	FMOD::DSP* dspSmoothStop;
};

#endif // _SOUND_INCLUDE