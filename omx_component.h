#ifndef RPI3_OMX_COMPONENT_H
#define RPI3_OMX_COMPONENT_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

#include "omx_port.h"

class OMXComponent {
public:
  OMXComponent(const std::string& component_name);
  void setup_ports(std::vector<OMX_U32> port_indexes = {});
  void print_state();
  void change_state(OMX_STATETYPE new_state);
  void wait_state(OMX_STATETYPE state);
  std::shared_ptr<OMXPort> port(OMX_U32 port_index);
  void enable_ports(bool state, std::vector<OMX_U32> port_indexes = {});
  void allocate_buffers(std::vector<OMX_U32> port_indexes = {});

  OMX_HANDLETYPE& handle() { return handle_; }

  friend OMX_ERRORTYPE omx_event_handler(OMX_HANDLETYPE hComponent, OMX_PTR pAppData, OMX_EVENTTYPE eEvent,
    OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData);

private:

  void add_defined_ports(OMX_INDEXTYPE index_type);

  OMX_HANDLETYPE handle_ {nullptr};
  OMX_STATETYPE state_;
  OMX_STATETYPE required_state_;

  OMX_VERSIONTYPE spec_version_;
  OMX_VERSIONTYPE comp_version_;
  OMX_UUIDTYPE uid_;

  std::map<OMX_U32, std::shared_ptr<OMXPort>> ports_;
  std::mutex state_mutex_;
};

#endif