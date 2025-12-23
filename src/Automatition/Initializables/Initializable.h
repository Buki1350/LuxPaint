#pragma once

class Initializable {
public:
  Initializable();
  virtual void init() = 0;
};
