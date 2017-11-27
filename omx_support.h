#ifndef RPI3_OMX_SUPPORT_H
#define RPI3_OMX_SUPPORT_H

#include <string>

#include <IL/OMX_Core.h>
#include <IL/OMX_Video.h>

static const uint32_t WAIT_SLICE {100'000}; // 100ms
static const uint32_t STATE_CHANGE_TIME {5'000'000}; // 5s
static const uint32_t PORT_SWITCH_TIME {5'000'000}; // 5s


#define OMX_INIT_STRUCTURE(a)                       \
  memset(&(a), 0, sizeof(a));                       \
  (a).nSize = sizeof(a);                            \
  (a).nVersion.nVersion = OMX_VERSION;              \
  (a).nVersion.s.nVersionMajor = OMX_VERSION_MAJOR; \
  (a).nVersion.s.nVersionMinor = OMX_VERSION_MINOR; \
  (a).nVersion.s.nRevision = OMX_VERSION_REVISION;  \
  (a).nVersion.s.nStep = OMX_VERSION_STEP;

const std::string omx_error_to_string(OMX_ERRORTYPE err);
const std::string omx_state_to_string(OMX_STATETYPE state);
const std::string omx_vcodec_to_string(OMX_VIDEO_CODINGTYPE codec);
const std::string omx_event_to_string(OMX_EVENTTYPE event);
const std::string omx_command_to_string(OMX_COMMANDTYPE command);
const std::string omx_color_format_to_string(OMX_COLOR_FORMATTYPE format);

#endif