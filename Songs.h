#ifndef songs_h
#define songs_h

const char* songs[]={
"\nTchaicovsky\n\nSwan Lake",
"\nShostakovich\n\nWaltz No. 2",
"\nElfen Lied\n\nLilium",
"\nGreen Sleeves",
"Strauss\n\nSul bel\nDanubio Blu",
"\nMerry Go Round\nof Life",
"Brahms\n\nRadetzky\nMarch",
"Brahms\nHungarian\nDance No.5",
"\nCarnevale ai bagni",
"\nBallata a Viareggio",
"\nCanto del Carnevale",
"\nCarnevale di baci",
"\nCarnevale a\n  Viareggio",
"\nChe Grande Idea",
"\nSu la coppa\n  di Champagne",
"\nMaschereide",
"\nSpiaggia d\'or",
"\nRisveglio\ndopo l'orgia",
"\nUn\'onda scivola",
"\nCome un Coriandolo",
"\nPolvere di Sole",
// The lullaby is always the last song. 
// Dont' move it
"\nStrauss\n\nLullaby"
};


class SongGetter {

  public:
    SongGetter(){};
    const char* getTrack(int i){
      return songs[i];
    }
};

#endif