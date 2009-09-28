define patch model
(
  color "orange"
  size [ 2 2 0.02 ]

  gui_nose 0
  gui_grid 0
  gui_move 1
  gui_outline 0

  # insensible to collision and range sensors
  obstacle_return 0
  laser_return 0
  ranger_return 0
  ctrl "patchCtrl"
)