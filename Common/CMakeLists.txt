

set(headers 
    stkMacros.hpp
)

vtk_module_add_module(STK::Common
  HEADER_ONLY  
  HEADERS ${headers}
  )
