#ifndef RPI3_OMX_FACADE_H
#define RPI3_OMX_FACADE_H

#include <string>
#include <vector>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>
#include <bcm_host.h>

class OMXFacade
{
public:
  OMXFacade();
  void list_components();
  void print_component_info(const std::string& component_sname, std::vector<uint32_t> port_indexes = {});

private:
  static const uint32_t MAX_ROLES_NUMBER {32};

  void list_roles(char *name);
};

#endif