#ifndef songs_h
#define songs_h

const char* songs[]={
"\nMerry Go Round\n  of Life\0",
"\nGreen Sleeves\0",
"\nTchaicovsky\n\n  Swan Lake\0",
"\nElfen Lied\n\n  Lilium\0",
"\nStrauss\n\n  Lullaby\0",
"\nCarnevale ai bagni\0",
"\nBallata a Viareggio\0",
"\nCanto del Carnevale\0",
"\nCarnevale di baci\0",
"\nCarnevale a\n  Viareggio\0",
"\nChe Grande Idea\0",
"\nSu la coppa\n  di Champagne\0",
"\nMaschereide\0",
"\nSpiaggia d\'or\0",
"\nRisveglio \n  dopo l'orgia\0",
"\nUn\'onda scivola\0",
"\nCome un Coriandolo\0",
"\nPolvere di Sole\0"};


class SongGetter {

  public:
    SongGetter(){};
    const char* getTrack(int i){
      return songs[i];
    }
};

#endif