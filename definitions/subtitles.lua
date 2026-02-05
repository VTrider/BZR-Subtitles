--- @meta subtitles
--- This file provides the lua definitions for the subtitles dll module

error([[This is a definition file, this should not be distributed with your mod]])

--- @class subtitles
local subtitles = {}

--- Submits a subtitle to the render queue. Displays the given text
--- for `duration` seconds. You can optionally provide a color
--- in RGB from 0.0-1.0
--- @param text string subtitle text
--- @param duration number duration in seconds
--- @param r number | nil
--- @param g number | nil
--- @param b number | nil
function subtitles.submit(text, duration, r, g, b) end

--- Sets the opacity of the subtitle background
--- Default: `0.0`
--- @param opacity number
function subtitles.set_opaacity(opacity) end

--- Clears the render queue
function subtitles.clear_queue() end
