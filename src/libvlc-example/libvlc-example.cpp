#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <chrono>
#include <thread>

#ifdef _MSC_VER
using ssize_t = long int;
#endif

#include <vlc/vlc.h>

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    return 1;
  }

  libvlc_instance_t* inst;
  libvlc_media_player_t* mp;
  libvlc_media_t* m;

  /* Load the VLC engine */
  inst = libvlc_new(0, NULL);

  /* Create a new item */
  //m = libvlc_media_new_location(inst, "http://mycool.movie.com/test.mov");
  m = libvlc_media_new_path(inst, argv[1]);

  /* Create a media player playing environement */
  mp = libvlc_media_player_new_from_media(m);

  /* No need to keep the media now */
  libvlc_media_release(m);

  /* play the media_player */
  libvlc_media_player_play(mp);

  std::this_thread::sleep_for(std::chrono::seconds(1));

  while (libvlc_media_player_is_playing(mp))
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    int64_t milliseconds = libvlc_media_player_get_time(mp);
    int64_t seconds = milliseconds / 1000;
    int64_t minutes = seconds / 60;
    milliseconds -= seconds * 1000;
    seconds -= minutes * 60;

    printf("Current time: %" PRId64 ":%" PRId64 ":%" PRId64 "\n",
      minutes, seconds, milliseconds);
  }

  /* Stop playing */
  libvlc_media_player_stop(mp);

  /* Free the media_player */
  libvlc_media_player_release(mp);

  libvlc_release(inst);

  return 0;
}