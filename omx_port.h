#ifndef RPI3_OMX_PORT_H
#define RPI3_OMX_PORT_H

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

class OMXPort {
public:
  OMXPort(OMX_U32 port_index, const OMX_HANDLETYPE& handle);
  void print_info();
  void enable(bool state = true);
  void set_enabled(bool state = true) {enabled_ = state;};
  bool enabled() const {return enabled_;}
  void allocate_buffer();
  void set_video_format(OMX_VIDEO_CODINGTYPE codec);

private:

  void get_supported_video_formats();

  OMX_PARAM_PORTDEFINITIONTYPE port_definition_;
  OMX_BUFFERHEADERTYPE* buffer_header_;
  bool enabled_ {true};
  
  const OMX_HANDLETYPE& handle_;
};

#endif