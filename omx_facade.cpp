
#include "omx_facade.h"

#include "logger.h"
#include "omx_component.h"
#include "support_functions.h"

OMXFacade::OMXFacade()
{
  bcm_host_init();

  OMX_ERRORTYPE err{OMX_Init()};
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX: OMX_Init() failed");
  }
}

void OMXFacade::list_components()
{
  char name[OMX_MAX_STRINGNAME_SIZE];
  OMX_ERRORTYPE err{OMX_ErrorNone};
  for (uint32_t i = 0; OMX_ErrorNoMore != err; ++i)
  {
    err = OMX_ComponentNameEnum(name, OMX_MAX_STRINGNAME_SIZE, i);
    if (OMX_ErrorNone == err)
    {
      Logger::debug("OMX: Component is: %s", name);
      list_roles(name);
    }
  }
  Logger::debug("OMX: No more components", name);
}

void OMXFacade::list_roles(char *name)
{
  OMX_U32 roles_number;
  OMX_U8 *roles[MAX_ROLES_NUMBER];
  /* get the number of roles by passing in a NULL roles param */
  OMX_ERRORTYPE err = OMX_GetRolesOfComponent(name, &roles_number, nullptr);
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX: Getting roles failed");
    return;
  }

  Logger::debug("OMX: Number of roles is: %d", roles_number);
  
  if (roles_number > MAX_ROLES_NUMBER)
  {
    Logger::error("OMX: Too many roles to list");
    return;
  }

  /* now get the roles */
  for (OMX_U32 n = 0; n < roles_number; ++n)
  {
    roles[n] = (OMX_U8 *)malloc(OMX_MAX_STRINGNAME_SIZE);
  }
  err = OMX_GetRolesOfComponent(name, &roles_number, roles);
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX: Getting roles failed");
    return;
  }
  for (OMX_U32 n = 0; n < roles_number; ++n)
  {
    Logger::debug("OMX: Role: %s", roles[n]);
    free(roles[n]);
  }
}

void OMXFacade::decode_file(const std::string& filename)
{
  get_file_size(filename);
  OMXComponent component("OMX.broadcom.image_encode");
  component.setup_ports();
  component.print_state();
  component.enable_ports(false);
  component.change_state(OMX_StateIdle);
  component.print_state();
  component.enable_ports(true, {340});
  component.allocate_buffers({340});
  component.enable_ports(true, {341});
  component.allocate_buffers({341});
  component.change_state(OMX_StateExecuting);
  component.print_state();
  //component.set_video_format(130, OMX_VIDEO_CodingAVC);
}