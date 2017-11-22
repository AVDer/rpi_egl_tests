#include "omx_port.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>

#include "omx_support.h"
#include "logger.h"

OMXPort::OMXPort(OMX_U32 port_index, const OMX_HANDLETYPE &handle) : handle_(handle)
{
  memset(&port_definition_, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  port_definition_.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
  port_definition_.nVersion.nVersion = OMX_VERSION;
  port_definition_.nPortIndex = port_index;
  get_definition();
  buffers_.resize(port_definition_.nBufferCountActual);
  buffer_headers_.resize(port_definition_.nBufferCountActual);
}

OMXPort::~OMXPort()
{
  for (auto buffer: buffers_) {
    if (buffer)
    {
      delete[] buffer;
    }
  }
}

void OMXPort::enable(bool state)
{
  if (!state)
  {
    OMX_SendCommand(handle_, OMX_CommandPortDisable, port_definition_.nPortIndex, nullptr);
  }
  else
  {
    OMX_SendCommand(handle_, OMX_CommandPortEnable, port_definition_.nPortIndex, nullptr);
  }
}

bool OMXPort::enabled()
{
  get_definition();
  return port_definition_.bEnabled;
}

void OMXPort::wait_state(bool state)
{
  uint32_t counter{0};
  while (enabled() != state)
  {
    if (counter++ >= PORT_SWITCH_TIME / WAIT_SLICE)
    {
      Logger::warning("OMX Port: Port %d can't change state to %d", port_definition_.nPortIndex, state);
      return;
    }
    usleep(WAIT_SLICE);
  }
  Logger::trace("OMX Port: Port %d changed state to %d", port_definition_.nPortIndex, state);
}

void OMXPort::allocate_buffer()
{
  for (OMX_U32 i {0}; i < port_definition_.nBufferCountActual; ++i)
  {
    buffers_[i] = new uint8_t[port_definition_.nBufferSize];
    OMX_ERRORTYPE error = OMX_UseBuffer(handle_, &buffer_headers_[i], port_definition_.nPortIndex, nullptr, port_definition_.nBufferSize, buffers_[i]);
    //OMX_ERRORTYPE error = OMX_AllocateBuffer(handle_, &buffer_headers_[i], port_definition_.nPortIndex, this, port_definition_.nBufferSize);
    if (error != OMX_ErrorNone)
    {
      Logger::error("OMX Port: Port %d buffer allocation failed: %s", port_definition_.nPortIndex, omx_error_to_string(error).c_str());
      return;
    }
  }
  Logger::trace("OMX Port: Port %d: %d x buffer(s) for %d bytes allocated.", port_definition_.nPortIndex,
  port_definition_.nBufferCountActual, port_definition_.nBufferSize);
}

void OMXPort::set_video_format(OMX_VIDEO_CODINGTYPE codec)
{
  OMX_VIDEO_PARAM_PORTFORMATTYPE video_port_format;
  memset(&video_port_format, 0, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
  video_port_format.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
  video_port_format.nVersion.nVersion = OMX_VERSION;
  video_port_format.nPortIndex = port_definition_.nPortIndex;
  video_port_format.eCompressionFormat = codec;
  OMX_ERRORTYPE error = OMX_SetParameter(handle_, OMX_IndexParamVideoPortFormat, &video_port_format);
  if (error == OMX_ErrorNone)
  {
    Logger::trace("OMX Port: Port %d video format changed to %s", port_definition_.nPortIndex, omx_vcodec_to_string(codec).c_str());
  }
  else
  {
    Logger::error("OMX Port: Port %d codec setup failed: %s", port_definition_.nPortIndex, omx_error_to_string(error).c_str());
  }
}

void OMXPort::print_info()
{
  std::string domain;
  Logger::debug("OMX Port: -----------------------");
  Logger::debug("OMX Port: Port %d %s", port_definition_.nPortIndex, ((port_definition_.eDir == OMX_DirInput) ? " is input port" : " is output port"));

  switch (port_definition_.eDomain)
  {
  case OMX_PortDomainAudio:
    domain = "Audio";
    break;
  case OMX_PortDomainVideo:
    domain = "Video";
    get_supported_video_formats();
    break;
  case OMX_PortDomainImage:
    domain = "Image";
    break;
  case OMX_PortDomainOther:
    domain = "Other";
    break;
  default:
    break;
  }

  Logger::debug("OMX Port: Domain is %s", domain.c_str());
  Logger::debug("OMX Port: Buffer count %d", port_definition_.nBufferCountActual);
  Logger::debug("OMX Port: Buffer minimum count %d", port_definition_.nBufferCountMin);
  Logger::debug("OMX Port: Buffer size %d bytes", port_definition_.nBufferSize);
  Logger::debug("OMX Port: -----------------------");
}

void OMXPort::get_supported_video_formats()
{
  OMX_VIDEO_PARAM_PORTFORMATTYPE sVideoPortFormat;
  OMX_INIT_STRUCTURE(sVideoPortFormat);
  sVideoPortFormat.nIndex = 0;
  sVideoPortFormat.nPortIndex = port_definition_.nPortIndex;
  Logger::debug("OMX Port: Supported video formats are:");
  while (1)
  {
    OMX_ERRORTYPE err = OMX_GetParameter(handle_, OMX_IndexParamVideoPortFormat, &sVideoPortFormat);
    if (err == OMX_ErrorNoMore)
    {
      // Logger::debug("OMX Port: No more formats supported");
      return;
    }
    /* This shouldn't occur, but does with Broadcom library */
    if (sVideoPortFormat.eColorFormat == OMX_COLOR_FormatUnused)
    {
      Logger::debug("OMX Port: No coding format returned");
      return;
    }
    Logger::debug("OMX Port: Video format encoding %s; color format 0x%X",
                  omx_vcodec_to_string(sVideoPortFormat.eCompressionFormat).c_str(), sVideoPortFormat.eColorFormat);
    sVideoPortFormat.nIndex++;
  }
}

void OMXPort::get_definition()
{
  if (OMX_GetParameter(handle_, OMX_IndexParamPortDefinition, &port_definition_) != OMX_ErrorNone)
  {
    Logger::error("OMX Port: %d: Failed to get definition", port_definition_.nPortIndex);
  }
}