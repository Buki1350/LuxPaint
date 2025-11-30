#pragma once
#include "Utils.h"

Vec2i Utils::View::GetCurrentMonitorSize() { return _monitorSize; }
Vec2i Utils::View::GetWindowSize() { return _windowSize; }
float Utils::View::GetSmallerMonitorEdge() { return _smallerMonitorEdge; }
float Utils::Time::GetDeltaTime() { return _deltaTime; }
