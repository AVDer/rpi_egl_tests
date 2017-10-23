#ifndef RPI3_OMX_PORT_H
#define RPI3_OMX_PORT_H

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

class OMXPort {
public:
  OMXPort(uint32_t port_index, const OMX_HANDLETYPE& handle);
  void print_info();
  void enable(bool state = true);

private:

  void get_supported_video_formats();

  OMX_PARAM_PORTDEFINITIONTYPE port_definition_;
  const OMX_HANDLETYPE& handle_;
};

#endif