#ifndef AUDIO_CUBE_H
#define AUDIO_CUBE_H

#define LITERAL_CONCAT(x, y) x##y
#define CONCAT(x, y) LITERAL_CONCAT(x, y)
#define AFFIX_VERSION(identifier) CONCAT(identifier, VERSION)

#include <thirdparty/miniaudio/miniaudio.h>

#undef AFFIX_VERSION
#undef CONCAT
#undef LITERAL_CONCAT

#endif // AUDIO_CUBE_H