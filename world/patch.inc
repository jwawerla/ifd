define patch model
(
  color "orange"
  size [ 2 2 0.00 ]

  gui_nose 0
  gui_grid 0
  gui_move 0
  gui_outline 0

  # insensible to collision and range sensors
  obstacle_return 0
  laser_return 0
  ranger_return 0
  ctrl "patchCtrl"
)

define puck model
(
  color "purple"
  name "puck"
  size [0.1 0.1 0.6]
  gui_nose 0
  gui_grid 0
  gui_move 1
  gui_outline 0

  pose [ 0.0 0.0 0.0 0.0 ]
  obstacle_return 1
  laser_return 1
)