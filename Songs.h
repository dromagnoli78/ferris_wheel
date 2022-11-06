#ifndef songs_h
#define songs_h

const char* songs[]={"prima lungalunga\0","seconda lungalunga lunga\0","terza\0","quarta\0","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A","A",};


class SongGetter {

  public:
    SongGetter(){};
    const char* getTrack(int i){
      return songs[i];
    }
};

#endif