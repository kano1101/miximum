#pragma once

#include <mix/Utility.h>

namespace Mix {

  class GraphicsInterface
  {
  public:
    virtual ~GraphicsInterface() {}
    virtual void Begin() const = 0;
    virtual void End() const = 0;
  };
  
} // namespace Mix

