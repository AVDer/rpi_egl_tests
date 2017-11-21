#ifndef RPI3_OMX_PORT_H
#define RPI3_OMX_PORT_H

#include <vector>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

class OMXPort {
public:
  OMXPort(OMX_U32 port_index, const OMX_HANDLETYPE& handle);
  ~OMXPort();
  void print_info();
  void enable(bool state = true);
  bool enabled();
  void allocate_buffer();
  void set_video_format(OMX_VIDEO_CODINGTYPE codec);
  void wait_state(bool state);

private:

  void get_supported_video_formats();
  void get_definition();


  OMX_PARAM_PORTDEFINITIONTYPE port_definition_;
  std::vector<OMX_BUFFERHEADERTYPE*> buffer_headers_;
  std::vector<uint8_t*> buffers_;
  
  const OMX_HANDLETYPE& handle_;
};

#endif