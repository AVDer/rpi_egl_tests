#ifndef RPI3_OMX_COMPONENT_H
#define RPI3_OMX_COMPONENT_H

#include <map>
#include <string>
#include <vector>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

#include "omx_port.h"

class OMXComponent {
public:
  OMXComponent(const std::string& component_name);
  void setup_ports(std::vector<uint32_t> port_indexes = {});
  void print_state();
  void change_state(OMX_STATETYPE new_state);
  void disable_ports() { for (auto& port : ports_) port.second->enable(false); }

  friend OMX_ERRORTYPE omx_event_handler(OMX_HANDLETYPE hComponent, OMX_PTR pAppData, OMX_EVENTTYPE eEvent,
    OMX_U32 Data1, OMX_U32 Data2, OMX_PTR pEventData);

private:

  static const uint32_t WAIT_SLICE {100'000}; // 100ms
  static const uint32_t STATE_CHANGE_TIME {5'000'000}; // 1s

  void add_defined_ports(OMX_INDEXTYPE index_type);

  OMX_HANDLETYPE handle_ {nullptr};
  OMX_STATETYPE state_;
  OMX_STATETYPE required_state_;

  OMX_VERSIONTYPE spec_version_;
  OMX_VERSIONTYPE comp_version_;
  OMX_UUIDTYPE uid_;

  std::map<uint32_t, OMXPort*> ports_;
};

#endif