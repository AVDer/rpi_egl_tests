#include "omx_port.h"

#include <cstring>
#include <string>

#include "omx_support.h"
#include "logger.h"

OMXPort::OMXPort(uint32_t port_index, const OMX_HANDLETYPE &handle) : handle_(handle)
{
  memset(&port_definition_, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  port_definition_.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
  port_definition_.nVersion.nVersion = OMX_VERSION;
  port_definition_.nPortIndex = port_index;
  if (OMX_GetParameter(handle_, OMX_IndexParamPortDefinition, &port_definition_) != OMX_ErrorNone)
  {
    Logger::error("OMX Port: %d: Failed to get definition", port_index);
  }
}

void OMXPort::enable(bool state) {
  if (!state) {
    OMX_SendCommand(handle_, OMX_CommandPortDisable, port_definition_.nPortIndex, nullptr);
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
    Logger::debug("OMX Port: Video format encoding 0x%X; color format 0x%X", sVideoPortFormat.eCompressionFormat, sVideoPortFormat.eColorFormat);
    sVideoPortFormat.nIndex++;
  }
}
