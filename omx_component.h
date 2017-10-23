#ifndef RPI3_OMX_COMPONENT_H
#define RPI3_OMX_COMPONENT_H

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
  void disable_ports() { for (auto& port : ports_) port.enable(false); }

  static OMX_ERRORTYPE cEventHandler(OMX_HANDLETYPE hComponent, OMX_PTR pAppData, OMX_EVENTTYPE eEvent, OMX_U32 Data1, OMX_U32 Data2, OMX_PTR pEventData);

private:

  void add_defined_ports(OMX_INDEXTYPE index_type);

  OMX_HANDLETYPE handle_ {nullptr};
  static OMX_STATETYPE state_; // for now
  static OMX_STATETYPE required_state_; // for now

  OMX_VERSIONTYPE spec_version_;
  OMX_VERSIONTYPE comp_version_;
  OMX_UUIDTYPE uid_;

  std::vector<OMXPort> ports_;
};

#endif