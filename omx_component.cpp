#include "omx_component.h"

#include <cstring>
#include <string>
#include <thread>

#include <unistd.h>

#include "logger.h"
#include "omx_support.h"

OMX_ERRORTYPE omx_event_handler(
    OMX_HANDLETYPE /*hComponent*/,
    OMX_PTR pAppData,
    OMX_EVENTTYPE eEvent,
    OMX_U32 Data1,
    OMX_U32 Data2,
    OMX_PTR /*pEventData*/)
{
  OMXComponent *component = reinterpret_cast<OMXComponent *>(pAppData);
  std::lock_guard<std::mutex> lock(component->state_mutex_);
  Logger::trace("OMX Component: [0x%X]: Event %s received", pAppData, omx_event_to_string(eEvent).c_str());

  switch (eEvent)
  {
  case OMX_EventCmdComplete:
    Logger::trace("OMX Component: Command: %s", omx_command_to_string(static_cast<OMX_COMMANDTYPE>(Data1)).c_str());
    switch (Data1)
    {
    case OMX_CommandStateSet:
      Logger::debug("OMX Component: [0x%X]: State changed to: %s", pAppData, omx_state_to_string(static_cast<OMX_STATETYPE>(Data2)).c_str());
      component->state_ = static_cast<OMX_STATETYPE>(Data2);
      break;
    case OMX_CommandFlush:
      break;
    case OMX_CommandPortDisable:
      Logger::trace("OMX Component: [0x%X]: Port %d disabled", pAppData, Data2);
      break;
    case OMX_CommandPortEnable:
      Logger::trace("OMX Component: [0x%X]: Port %d enabled", pAppData, Data2);
      break;
    case OMX_CommandMarkBuffer:
      break;
    default:
      break;
    }
  default:
    break;
  }
  Logger::trace("Callback return");
  return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_empty_buffer_done(
    OMX_IN OMX_HANDLETYPE /*hComponent*/,
    OMX_IN OMX_PTR pAppData,
    OMX_IN OMX_BUFFERHEADERTYPE * pBuffer)
{
  Logger::warning("OMX Component: [0x%X]: Empty buffer done for 0x%X", pAppData, pBuffer);
  return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_fill_buffer_done(
    OMX_IN OMX_HANDLETYPE /*hComponent*/,
    OMX_IN OMX_PTR pAppData,
    OMX_IN OMX_BUFFERHEADERTYPE * pBuffer)
{
  Logger::warning("OMX Component: [0x%X]: Fill buffer done for 0x%X", pAppData, pBuffer);
  return OMX_ErrorNone;
}

OMXComponent::OMXComponent(const std::string &component_sname)
{
  OMX_CALLBACKTYPE callbacks = {.EventHandler = omx_event_handler,
                                .EmptyBufferDone = omx_empty_buffer_done,
                                .FillBufferDone = omx_fill_buffer_done};

  char component_name[128]{0};
  strcpy(component_name, component_sname.c_str());

  // Ask the core for a handle to the component
  OMX_ERRORTYPE err = OMX_GetHandle(&handle_, component_name, this, &callbacks);
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

  Logger::info("OMX Component: [0x%X] handler obtained. Component name: %s version %d.%d, Spec version: %d.%d",
               this, component_name, comp_version_.s.nVersionMajor, comp_version_.s.nVersionMinor,
               spec_version_.s.nVersionMajor, spec_version_.s.nVersionMinor);
}

void OMXComponent::setup_ports(std::vector<uint32_t> port_indexes)
{
  if (!port_indexes.empty())
  {
    for (auto port : port_indexes)
    {
      ports_[port].reset(new OMXPort(port, handle_));
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
    port.second->print_info();
  }
}

void OMXComponent::print_state()
{
  OMX_STATETYPE state;
  OMX_ERRORTYPE err{OMX_GetState(handle_, &state)};
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX Component: Error on getting state");
    return;
  }
  Logger::info("OMX Component: Component state: %s", omx_state_to_string(state).c_str());
}

void OMXComponent::change_state(OMX_STATETYPE new_state)
{
  Logger::debug("OMX Component: 0x%X changed request to: %s", this, omx_state_to_string(new_state).c_str());
  OMX_SendCommand(handle_, OMX_CommandStateSet, new_state, NULL);
}

void OMXComponent::wait_state(OMX_STATETYPE state)
{
  uint32_t counter{0};
  while (state_ != state)
  {
    if (counter++ >= STATE_CHANGE_TIME / WAIT_SLICE)
    {
      Logger::warning("OMX Component: [0x%X] can't change state to %s", this, omx_state_to_string(state).c_str());
      return;
    }
    //usleep(WAIT_SLICE);
    std::this_thread::sleep_for(std::chrono::microseconds(WAIT_SLICE));
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
    ports_[i].reset(new OMXPort(i, handle_));
  }
}

void OMXComponent::enable_ports(bool state, std::vector<OMX_U32> port_indexes)
{
  if (port_indexes.empty())
  {
    for (auto &port : ports_)
      port.second->enable(state);
  }
  else
  {
    for (OMX_U32 i : port_indexes)
    {
      ports_[i]->enable(state);
    }
  }
}

void OMXComponent::allocate_buffers(std::vector<OMX_U32> port_indexes)
{
  if (port_indexes.empty())
  {
    for (auto &port : ports_)
      port.second->allocate_buffer();
  }
  else
  {
    for (OMX_U32 i : port_indexes)
    {
      ports_[i]->allocate_buffer();
    }
  }
}
