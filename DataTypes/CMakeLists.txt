set(classes
  vtkHyperCube
  vtkHeaderTable
  vtkTracePanelData
  )

vtk_module_add_module(STK::DataTypes
  CLASSES ${classes})
