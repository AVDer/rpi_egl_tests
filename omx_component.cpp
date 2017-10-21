#include "omx_component.h"

#include <cstring>
#include <iostream>
#include <string>

OMXComponent::OMXComponent(const std::string &component_sname, std::vector<uint32_t> port_indexes)
{
  OMX_CALLBACKTYPE callbacks = {.EventHandler = nullptr,
                                .EmptyBufferDone = nullptr,
                                .FillBufferDone = nullptr};

  char component_name[128]{0};
  strcpy(component_name, component_sname.c_str());

  // Ask the core for a handle to the component
  OMX_ERRORTYPE err = OMX_GetHandle(&handle_, component_name, NULL, &callbacks);
  if (err != OMX_ErrorNone)
  {
    std::cerr << component_name << ": OMX_GetHandle faile" << std::endl;
  }
  // Get some version info
  err = OMX_GetComponentVersion(handle_, component_name,
                                &comp_version_, &spec_version_,
                                &uid_);
  if (err != OMX_ErrorNone)
  {
    std::cerr << component_name << ": OMX_GetComponentVersion failed" << std::endl;
  }
  std::cout << "OMX component handler obtained. Component name: " << component_name
            << " version " << static_cast<uint16_t>(comp_version_.s.nVersionMajor) << '.' << static_cast<uint16_t>(comp_version_.s.nVersionMinor)
            << ", Spec version: " << static_cast<uint16_t>(spec_version_.s.nVersionMajor) << '.' << static_cast<uint16_t>(spec_version_.s.nVersionMinor)
            << std::endl;

  for (auto port : port_indexes) {
    ports_.emplace_back(OMXPort(port, handle_));
  }

  for (auto port: ports_) {
    port.print_info();
  }
}