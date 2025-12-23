#pragma once

class Snapshot {
public:
  virtual ~Snapshot() = default;
  virtual void jumpTo() = 0;
};
