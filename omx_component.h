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

private:

  void add_defined_ports(OMX_INDEXTYPE index_type);

  OMX_HANDLETYPE handle_;
  OMX_VERSIONTYPE spec_version_;
  OMX_VERSIONTYPE comp_version_;
  OMX_UUIDTYPE uid_;

  std::vector<OMXPort> ports_;
};

#endif