#include <audio_cube.h>
#include <assert.h>

#define miniaudio_c // miniaudio implementation is already implemented.

// The author assumes the implementation is in the source file,
// so they uses macros only available with the implementation.
// This is not the case, thus we define the macros ourselves.
#define MA_ASSERT(condition) assert(condition)

#include <thirdparty/miniaudio/examples/simple_playback_sine.c>