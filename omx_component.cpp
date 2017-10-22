#include "omx_component.h"

#include <cstring>
#include <string>

#include "logger.h"
#include "omx_support.h"

OMXComponent::OMXComponent(const std::string &component_sname)
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
    Logger::error("OMX Component: %s: OMX_GetHandle failed", component_name);
  }
  // Get some version info
  err = OMX_GetComponentVersion(handle_, component_name,
                                &comp_version_, &spec_version_,
                                &uid_);
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX Component: %s: OMX_GetComponentVersion failed", component_name);
  }

  Logger::info("OMX Component: handler obtained. Component name: %s version %d.%d, Spec version: %d.%d",
               component_name, comp_version_.s.nVersionMajor, comp_version_.s.nVersionMinor, spec_version_.s.nVersionMajor, spec_version_.s.nVersionMinor);

}

void OMXComponent::setup_ports(std::vector<uint32_t> port_indexes)
{
  if (!port_indexes.empty())
  {
    for (auto port : port_indexes)
    {
      ports_.emplace_back(OMXPort(port, handle_));
    }
  }
  else
  {
    add_defined_ports(OMX_IndexParamAudioInit);
    add_defined_ports(OMX_IndexParamImageInit);
    add_defined_ports(OMX_IndexParamVideoInit);
    add_defined_ports(OMX_IndexParamOtherInit);
  }

  for (auto port : ports_)
  {
    port.print_info();
  }
}

void OMXComponent::add_defined_ports(OMX_INDEXTYPE index_type)
{
  OMX_PORT_PARAM_TYPE param;
  OMX_INIT_STRUCTURE(param);
  OMX_ERRORTYPE err = OMX_GetParameter(handle_, index_type, &param);
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX Component: Can't get ports for index %d", index_type);
    return;
  }
  for (auto i = param.nStartPortNumber; i < param.nStartPortNumber + param.nPorts; ++i)
  {
    ports_.emplace_back(OMXPort(i, handle_));
  }
}