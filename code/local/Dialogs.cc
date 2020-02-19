#include "Dialogs.h"
#include "Singletons.h"
namespace KGB {

	static constexpr float SoundVolume = 100.0f;

	Dialogs::Dialogs()
	: m_dialogHeinz(gResourceManager().getSound("sounds/dialogHeinz.wav"))
    , m_dialogFranchement(gResourceManager().getSound("sounds/dialogFranchement.wav"))
    , m_dialogCramer(gResourceManager().getSound("sounds/dialogCramer.wav"))
    , m_dialogTiensTiens(gResourceManager().getSound("sounds/dialogTiensTiens.wav"))
	{
        m_dialogHeinz.setVolume(SoundVolume);
        m_dialogCramer.setVolume(SoundVolume);
        m_dialogFranchement.setVolume(SoundVolume);
        m_dialogTiensTiens.setVolume(SoundVolume);
	}

    void Dialogs::playCramer(){
        m_dialogCramer.play();
    }

    void Dialogs::playHeinz(){
        m_dialogHeinz.play();
    }

    void Dialogs::playFranchement(){
        m_dialogFranchement.play();
    }

    void Dialogs::stopFranchement(){
        m_dialogFranchement.stop();
    }

    void Dialogs::playTiensTiens(){
        m_dialogTiensTiens.play();
    }

    void Dialogs::muteAll(){
        m_dialogCramer.stop();
        m_dialogHeinz.stop();
        m_dialogFranchement.stop();
        m_dialogTiensTiens.stop();
    }


}
