#ifndef RPI3_OMX_FACADE_H
#define RPI3_OMX_FACADE_H

#include <IL/OMX_Core.h>
#include <bcm_host.h>

class OMXFacade
{
public:
  void check_roles();

private:
  static const uint32_t MAX_ROLES_NUMBER {32};

  void list_roles(char *name);
};

#endif