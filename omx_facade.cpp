
#include "omx_facade.h"

#include <iostream>

void OMXFacade::check_roles()
{
  bcm_host_init();

  OMX_ERRORTYPE err = OMX_Init();
  if (err != OMX_ErrorNone)
  {
    std::cout << "OMX_Init() failed" << std::endl;
  }
  
  char name[OMX_MAX_STRINGNAME_SIZE];
  for (uint32_t i = 0; OMX_ErrorNoMore != err; ++i)
  {
    err = OMX_ComponentNameEnum(name, OMX_MAX_STRINGNAME_SIZE, i);
    if (OMX_ErrorNone == err)
    {
      std::cout << "Component is: " << name << std::endl;
      //list_roles(name);
    }
  }
  std::cout << "No more components" << std::endl;
}

void OMXFacade::list_roles(char *name)
{
  OMX_U32 roles_number;
  OMX_U8 *roles[MAX_ROLES_NUMBER];
  /* get the number of roles by passing in a NULL roles param */
  OMX_ERRORTYPE err = OMX_GetRolesOfComponent(name, &roles_number, nullptr);
  if (err != OMX_ErrorNone)
  {
    std::cout << "Getting roles failed" << std::endl;
    return;
  }

  std::cout << "Number of roles is: " << roles_number << std::endl;

  if (roles_number > MAX_ROLES_NUMBER)
  {
    std::cout << "Too many roles to list" << std::endl;
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
    std::cout << "Getting roles failed" << std::endl;
    return;
  }
  for (OMX_U32 n = 0; n < roles_number; ++n)
  {
    std::cout << "Role: " << roles[n] << std::endl;
    free(roles[n]);
  }
}