#include "Sound.h"
#include <iostream>

Sound::Sound(void)
{
	FMOD::System_Create(&system);// create an instance of the game engine
	system->init(32, FMOD_INIT_NORMAL, 0);// initialise the game engine with 32 channels (cantidad de sonido simultaneo que puede haber)
}

Sound::~Sound(void)
{
	//for (int i = 0; i<NUM_SOUNDS; i++) sounds[i]->release();
	//system->release();
}

bool Sound::Load()
{
	system->createStream("music/01-overworld-day.mp3", FMOD_LOOP_NORMAL, 0, &sounds[SOUND_AMBIENT1]);
	system->createStream("music/03-overworld-night.mp3", FMOD_LOOP_NORMAL, 0, &sounds[SOUND_AMBIENT2]);
	system->createStream("music/04-title-screen.mp3", FMOD_LOOP_NORMAL, 0, &sounds[SOUND_TITLESCREEN]);
	system->createStream("music/07-jungle.mp3", FMOD_LOOP_NORMAL, 0, &sounds[SOUND_JUNGLE]);
	system->createSound("music/Jump_1.wav", FMOD_DEFAULT, 0, &sounds[SOUND_JUMP]);
	system->createSound("music/Player_Hit_0.wav", FMOD_DEFAULT, 0, &sounds[SOUND_PLAYERHIT]);
	system->createSound("music/Player_Hit_1.wav", FMOD_DEFAULT, 0, &sounds[SOUND_PLAYERHIT2]);
	system->createSound("music/Player_Hit_2.wav", FMOD_DEFAULT, 0, &sounds[SOUND_PLAYERHIT3]);
	system->createSound("music/Player_Killed.wav", FMOD_DEFAULT, 0, &sounds[SOUND_PLAYERKILLED]);
	system->createSound("music/Boss.wav", FMOD_DEFAULT, 0, &sounds[SOUND_BOSS]);
	system->createSound("music/Run.wav", FMOD_DEFAULT, 0, &sounds[SOUND_RUN]);
	system->createSound("music/Tink_0.wav", FMOD_DEFAULT, 0, &sounds[SOUND_MINING]);
	system->createSound("music/Zombie_0.wav", FMOD_DEFAULT, 0, &sounds[SOUND_ZOMBIE]);
	system->createSound("music/Zombie_1.wav", FMOD_DEFAULT, 0, &sounds[SOUND_ZOMBIE2]);
	system->createSound("music/Zombie_2.wav", FMOD_DEFAULT, 0, &sounds[SOUND_ZOMBIE3]);
	system->createSound("music/NPC_Killed_1.wav", FMOD_DEFAULT, 0, &sounds[SOUND_MONSTERKILLED]);
	system->createSound("music/Grab.wav", FMOD_DEFAULT, 0, &sounds[SOUND_GRAB]);
	system->createSound("music/Dig.wav", FMOD_DEFAULT, 0, &sounds[SOUND_DIG]);
	system->createSound("music/Sword.wav", FMOD_DEFAULT, 0, &sounds[SOUND_SWORD]);
	system->createSound("music/Put.wav", FMOD_DEFAULT, 0, &sounds[SOUND_PUT]);
	system->createSound("music/Menu_Open.wav", FMOD_DEFAULT, 0, &sounds[SOUND_MENUOPEN]);
	system->createSound("music/Menu_Close.wav", FMOD_DEFAULT, 0, &sounds[SOUND_MENUCLOSE]);
	system->createSound("music/NPC_Hit_1.wav", FMOD_DEFAULT, 0, &sounds[SOUND_NPCHIT]);
	return true;
}

void Sound::Play(int sound_id)
{
	if (sound_id == SOUND_AMBIENT1) {
		system->playSound(sounds[SOUND_AMBIENT1], NULL, false, &ambient1Channel);
		ambient1Channel->setVolume(1.f); //between 0 and 1
	}
	else if (sound_id == SOUND_AMBIENT2) {
		system->playSound(sounds[SOUND_AMBIENT2], NULL, false, &ambient2Channel);
		ambient2Channel->setVolume(1.f); //between 0 and 1
	}
	else if (sound_id == SOUND_TITLESCREEN) {
		system->playSound(sounds[SOUND_TITLESCREEN], NULL, false, &titlescreenChannel);
		ambient2Channel->setVolume(1.f); //between 0 and 1
	}
	else if (sound_id == SOUND_JUNGLE) {
		system->playSound(sounds[SOUND_JUNGLE], NULL, false, &jungleChannel);
		ambient2Channel->setVolume(1.f); //between 0 and 1
	}
	else system->playSound(sounds[sound_id], NULL, false, 0);
}

void Sound::StopAll()
{
	ambient1Channel->stop();
	ambient2Channel->stop();
	titlescreenChannel->stop();
	jungleChannel->stop();

}

void Sound::Update()
{
	system->update();
}