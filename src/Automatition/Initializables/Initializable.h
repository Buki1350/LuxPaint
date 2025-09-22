#pragma once

class Initializable {
public:
  Initializable();
  virtual void Init() = 0;
};
