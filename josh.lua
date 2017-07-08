function init()
  window.width = 1080
  window.height = 900
  window.size = vec2(window.width, window.height)
  window.title = "JELLYMOON TEST"
end

local curTime = 0
local state = false

local endWidth = 100
local endHeight = 100
local curWidth = 100
local curHeight = 100

function update()
  curTime = math.min(curTime + (time.dt / .4), 1)
  local width = curWidth + easeInOutQuad(curTime) * (endWidth - curWidth)
  local height = curHeight + easeInOutQuad(curTime) * (endHeight - curHeight)

  if state then 
    endWidth = 100
    endHeight = 100
  else 
    endWidth = window.size.x - 40
    endHeight = window.size.y - 40
  end

  if mouse.left.pressed then
    curTime = 0
    curWidth = width
    curHeight = height
    if state then 
      state = false
    else 
      state = true
    end
  end

  set_color(1, 1, 1, 1)
  draw_rect(20, 20, width, height)
end

function easeInOutQuad(t)
  if t < .5 then 
    return 2*t*t
  else 
    return -1+(4-2*t)*t
  end
end