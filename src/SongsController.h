#ifndef songsController_h
#define songsController_h

const char* songs[]={
"Tchaikovsky\nSwan Lake",
"Dmitri Shostakovich\nWaltz N2",
"Elfen Lied\nLilium",
"Green Sleeves",
"Johann Strauss\nSul bel danubio blu",
"Howl's Moving Castle",
"Johann Strauss\nRadetzky march",
"Johannes Brahms\nHungarian dance N5",
"Johannes Brahms\nLullaby",
"Carnevale 1921\nSu la coppa di Champagne",
"Carnevale 1922\nMaschereide",
"Carnevale 1924\nCarnevale a Viareggio",
"Carnevale 1924\nRisveglio dopo l'orgia",
"Carnevale 1928\nCarnevale ai Bagni",
"Carnevale 1929\nSpiaggia D'Or",
"Carnevale 1932\nCarnevale di Baci",
"Carnevale 1933\nCanto del Carnevale",
"Carnevale 1935\nBallata a Viareggio",
"Carnevale 1982\nUn'Onda Scivola",
"Carnevale 1983\nChe grande Idea",
"Carnevale 1983\nPolvere di sole",
"Carnevale 1987\nCome un Coriandolo",
"Facciamo un Pupazzo Insieme",
"Oggi per la Prima Volta",
"All'Alba sorgerò",
"Il fiume del passato",
"Nell'ignoto",
"Mostrati",
"Fai ciò che è giusto",
"Qualche cosa non cambia mai",
"La famiglia Madrigal",
"La pressione sale",
"Un Miracolo",
"Non si nomina Bruno",
"Cos'Altro Farò",
"Oltre l'orizzonte",
"Tranquilla",
"Lo splendente Tamatoa",
"Aspettando una nuova vita",
"Resta con me",
"Ho un sogno anch'io",
"Il mio nuovo sogno",
"La Bella e la Bestia",
"In fondo al mar",
"Dargen D'Amico\nDove Si Balla",
"Johannes Brahms\nLullaby",
};

const char* folders[]={
"Classiche",
"Carnevale",
"Frozen",
"Encanto",
"Oceania",
"Rapunzel",
"Disney",
"Dance",
"Lullaby",
};

const int tracksPerFolder[9]= {9,13,8,5,3,4,2,1,1};
class SongsController {
public:
  SongsController(){};
  const char* getTrack(int i) {return songs[i];};
  const int getNumFolders(){return 9;};
  const int* getTracksPerFolder(){ return tracksPerFolder;};
  const int getTracksPerFolder(int i){ return tracksPerFolder[i];};
  const int getLastTrack(){ return 46;};
  int computeNextFolder(int currentTrack);
  int computePreviousFolder(int currentTrack);
};

int SongsController::computeNextFolder(int currentTrack) {
  int runningTrack = currentTrack;
  int incrementalTrack = 0;
  int folder = 0;
  int jump = 0;
  for (;folder < getNumFolders(); folder++) {
    incrementalTrack += tracksPerFolder[folder];
    if (runningTrack <= incrementalTrack){
      jump = (incrementalTrack - runningTrack) +1;
      if (runningTrack + jump > getLastTrack()) {
        jump = -(runningTrack-1);
      }
      break;
    }
  }
  dbg ("Jumping tracks:", jump);
  return jump;
};

int SongsController::computePreviousFolder(int currentTrack) {
  int runningTrack = currentTrack;
  int incrementalTrack = 0;
  int previousIncrementalTrack = 0;
  int folder = 0;
  int jump = 0;
  for (;folder < getNumFolders(); folder++) {
     incrementalTrack += tracksPerFolder[folder];
     if ((runningTrack == incrementalTrack + 1) || runningTrack < incrementalTrack) {
       jump = (runningTrack - previousIncrementalTrack) - 1;
       break;
     } else {
       previousIncrementalTrack = incrementalTrack;
     }
  }
  if (folder == getNumFolders()) {
     jump = (runningTrack - previousIncrementalTrack) -1;
  }
  if (runningTrack - jump < 0) {
     jump = -(runningTrack-1);
  }
  dbg ("Jumping tracks:", -jump);
  return -jump;
};

#endif
