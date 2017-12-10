#ifndef RPI3_OMX_TUNNEL_H
#define RPI3_OMX_TUNNEL_H

#include "omx_component.h"

class OMXTunnel {
public:
  OMXTunnel(OMXComponent& out_component, OMX_U32 out_port, OMXComponent& in_component, OMX_U32 in_port):
  out_component_(out_component),
  in_component_(in_component),
  out_port_(out_port),
  in_port_(in_port) {}

  void activate();
  void deactivate();


private:
  OMXComponent& out_component_;
  OMXComponent& in_component_;
  OMX_U32 out_port_;
  OMX_U32 in_port_;
};

#endif
