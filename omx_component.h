#ifndef RPI3_OMX_COMPONENT_H
#define RPI3_OMX_COMPONENT_H

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

#include "omx_port.h"

using fill_cb_func_t = std::function<void(OMX_BUFFERHEADERTYPE*)>;

class OMXComponent {
public:
  OMXComponent(const std::string& component_name);
  OMXComponent(const OMXComponent&) = delete;

  OMXComponent operator=(const OMXComponent&) = delete;

  void setup_ports(std::vector<OMX_U32> port_indexes = {});
  void print_state();
  void change_state(OMX_STATETYPE new_state);
  void wait_state(OMX_STATETYPE state);
  std::shared_ptr<OMXPort> port(OMX_U32 port_index);
  void enable_ports(bool state, std::vector<OMX_U32> port_indexes = {});
  void allocate_buffers(std::vector<OMX_U32> port_indexes = {});
  void add_fill_cb(fill_cb_func_t func) {
    fill_cb_function_ = func;
  }

  OMX_HANDLETYPE& handle() { return handle_; }

  friend OMX_ERRORTYPE omx_event_handler(OMX_HANDLETYPE hComponent, OMX_PTR pAppData, OMX_EVENTTYPE eEvent,
    OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData);
  friend OMX_ERRORTYPE omx_fill_buffer_done(OMX_HANDLETYPE, OMX_PTR pAppData, OMX_BUFFERHEADERTYPE * pBuffer);

private:

  void add_defined_ports(OMX_INDEXTYPE index_type);

  OMX_HANDLETYPE handle_ {nullptr};
  OMX_STATETYPE state_ {OMX_StateInvalid};
  OMX_STATETYPE required_state_ {OMX_StateInvalid};

  OMX_VERSIONTYPE spec_version_;
  OMX_VERSIONTYPE comp_version_;
  OMX_UUIDTYPE uid_ {0};

  std::map<OMX_U32, std::shared_ptr<OMXPort>> ports_;
  std::mutex state_mutex_;

  fill_cb_func_t fill_cb_function_ = nullptr;
};

#endif