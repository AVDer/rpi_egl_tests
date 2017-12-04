#ifndef RPI3_OMX_PORT_H
#define RPI3_OMX_PORT_H

#include <vector>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

enum PortFlag : uint32_t {
  changed
};

using buffer_address_t = std::pair<OMX_U32, OMX_U32>;

struct Buffer {
  OMX_BUFFERHEADERTYPE* buffer_header {nullptr};
  uint8_t* data {nullptr};
  bool ready {true};
  buffer_address_t  buffer_address {0, 0};
};

class OMXPort {
public:
  OMXPort(OMX_U32 port_index, const OMX_HANDLETYPE& handle);
  ~OMXPort();
  void print_info();
  void enable(bool state = true);
  bool enabled();
  void allocate_buffer();
  OMX_BUFFERHEADERTYPE* get_buffer(bool blocking = true);
  void set_buffer_ready(OMX_U32 buffer_index, bool ready = true) { buffers_[buffer_index].ready = ready; }
  void set_video_format(OMX_VIDEO_CODINGTYPE codec);
  void wait_state(bool state);
  OMX_VIDEO_PARAM_PORTFORMATTYPE video_port_format();
  void set_video_port_format(OMX_VIDEO_PARAM_PORTFORMATTYPE port_format);
  void set_flag(PortFlag flag, bool state = true);
  bool get_flag(PortFlag flag);

  void print_video_settings();

private:
  void get_supported_video_formats();
  void get_definition();
  uint32_t flags_;

  OMX_PARAM_PORTDEFINITIONTYPE port_definition_;
  std::vector<Buffer> buffers_;
  
  const OMX_HANDLETYPE& handle_;
};

#endif