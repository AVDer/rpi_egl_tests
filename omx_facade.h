#ifndef RPI3_OMX_FACADE_H
#define RPI3_OMX_FACADE_H

#include <string>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>
#include <bcm_host.h>

class OMXFacade
{
public:
  OMXFacade();
  void check_roles();
  void get_port_info(const std::string& component_sname, uint32_t port_index);

private:
  static const uint32_t MAX_ROLES_NUMBER {32};

  void list_roles(char *name);
  OMX_ERRORTYPE get_port_info(OMX_HANDLETYPE handle, OMX_PARAM_PORTDEFINITIONTYPE *portdef);
  void print_port_info(OMX_PARAM_PORTDEFINITIONTYPE *portdef);
};

#endif