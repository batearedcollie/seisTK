set(classes
  vtkHyperCubeAlgorithm
  vtkHeaderTableAlgorithm
  vtkTracePanelAlgorithm
  )

vtk_module_add_module(STK::Algorithms
  CLASSES ${classes})
