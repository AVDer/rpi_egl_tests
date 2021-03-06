#ifndef RPI3_OMX_FACADE_H
#define RPI3_OMX_FACADE_H

#include <string>
#include <vector>

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>
#include <bcm_host.h>

#include "omx_component.h"

using egl_image_t = void*;

class OMXFacade
{
public:
  OMXFacade();
  ~OMXFacade();
  void list_components();
  void decode_file(const std::string& filename);
  void render_file(const std::string& filename);
  void decode_to_egl(const std::string& filename, egl_image_t egl_image);

private:
  static const uint32_t MAX_ROLES_NUMBER {32};

  void list_roles(char *name);
};

#endif