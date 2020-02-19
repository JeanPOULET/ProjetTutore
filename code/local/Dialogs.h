#ifndef KGB_DIALOGS_H
#define KGB_DIALOGS_H


#include <SFML/Audio.hpp>
#include "Singletons.h"

namespace KGB {

  class Dialogs {
    public:
        Dialogs();

        void toggleMute();
        void playHeinz();
        void playFranchement();
        void playCramer();
        void playTiensTiens();
        void muteAll();

    private:
        sf::Sound m_dialogHeinz;
        sf::Sound m_dialogFranchement;
        sf::Sound m_dialogCramer;
        sf::Sound m_dialogTiensTiens;

    };

}

#endif // KGB_DIALOGS_H