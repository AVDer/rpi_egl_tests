#include "omx_tunnel.h"

#include "logger.h"

void OMXTunnel::activate() {
  out_component_.enable_ports(false, {out_port_});
  in_component_.enable_ports(false, {in_port_});
  OMX_ERRORTYPE error = OMX_SetupTunnel(out_component_.handle(), out_port_, in_component_.handle(), in_port_);
  if (error != OMX_ErrorNone)
  {
    Logger::error("OMX: Tunnel seup failed");
    return;
  }
  out_component_.enable_ports(true, {out_port_});
  in_component_.enable_ports(true, {in_port_});
}

void OMXTunnel::deactivate() {

}