#ifndef TracksController_h
#define TracksController_h

const char* tracks[]={
"Tchaikovsky - Swan Lake",
"Dmitri Shostakovich - Waltz N2",
"Elfen Lied - Lilium",
"Green Sleeves",
"Johann Strauss - Sul bel danubio blu",
"Howl's Moving Castle",
"Johann Strauss - Radetzky march",
"Johannes Brahms - Hungarian dance N5",
"Johannes Brahms - Lullaby",
"Beck Bennet Scarlett Johansson - I dont wanna",
"Katy Perry - Firework",
"Taylor Swift - Shake It Off",
"Scarlett Johansson - Set It All Free",
"Shawn Mendes - There s Nothing Holding Me Back",
"Taron Egerton - I m Still Standing",
"Stevie Wonder Feat. Ariana Grande - Faith",
"Facciamo un Pupazzo Insieme",
"Oggi per la Prima Volta",
"All'Alba sorgero'",
"Il fiume del passato",
"Nell'ignoto",
"Mostrati",
"Fai cio' che e' giusto",
"Qualche cosa non cambia mai",
"La famiglia Madrigal",
"La Pressione sale",
"Un Miracolo",
"Non si nomina Bruno",
"Cos'Altro Faro'",
"Oltre l'orizzonte",
"Tranquilla",
"Lo splendente Tamatoa",
"Aspettando una nuova vita",
"Resta con me",
"Ho un sogno anch'io",
"Il mio nuovo sogno",
"La Bella e la Bestia",
"In fondo al mar",
"Dargen D'Amico - Dove Si Balla",
"David Guetta ft Zara Larsson - This One is For You",
"Annalisa - Bellissima",
"Annalisa - Mon amour",
"Ariana Grande - Break Free",
"Gigi D'Agostino - L'amour Toujours",
"Jingle Bells",
"White Christmas",
"Dean Martin - Let It Snow! Let It Snow! Let It Snow!",
"Mariah Carey - All I Want For Christmas Is You",
"Carnevale 1921 - Su la coppa di Champagne",
"Carnevale 1922 - Maschereide",
"Carnevale 1924 - Carnevale a Viareggio",
"Carnevale 1924 - Risveglio dopo l'orgia",
"Carnevale 1928 - Carnevale ai Bagni",
"Carnevale 1929 - Spiaggia D'Or",
"Carnevale 1932 - Carnevale di Baci",
"Carnevale 1933 - Canto del Carnevale",
"Carnevale 1935 - Ballata a Viareggio",
"Carnevale 1982 - Un'Onda Scivola",
"Carnevale 1983 - Che grande Idea",
"Carnevale 1983 - Polvere di sole",
"Carnevale 1987 - Come un Coriandolo",
"Johannes Brahms - Lullaby",
};

const char* folders[]={
"Classiche",
"Sing",
"Frozen",
"Encanto",
"Oceania",
"Rapunzel",
"Disney",
"Dance",
"Natale",
"Carnevale",
"Lullaby",
};

const int tracksPerFolder[11]= {9,7,8,5,3,4,2,6,4,13,1};
class TracksController {
public:
  TracksController(){};
  const char* getTrack(int i) {return tracks[i];};
  const char* getFolder(int i) {return folders[i];};
  const int getNumFolders(){return 11;};
  const int* getTracksPerFolder(){ return tracksPerFolder;};
  const int getTracksPerFolder(int i){ return tracksPerFolder[i];};
  const int getLastTrack(){ return 62;};
  int computeNextFolder(int currentTrack);
  int computePreviousFolder(int currentTrack);
  int computeFolderByTrackNumber(int currentTrack);
  const char* getLongestTrackName() {return "Dean Martin - Let It Snow! Let It Snow! Let It Snow!";}
};

int TracksController::computeNextFolder(int currentTrack) {
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

int TracksController::computePreviousFolder(int currentTrack) {
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

int TracksController::computeFolderByTrackNumber(int currentTrack) {
  int arraySize = getNumFolders();
  int currentSum = 0;
  int elementIndex = -1;
  for (int i = 0; i < arraySize; i++) {
      currentSum += tracksPerFolder[i];
      if (currentSum >= currentTrack) {
         elementIndex = i;
         break;  // Exit the loop as soon as the target is reached or exceeded
      }
  }
  return elementIndex;
};

#endif
