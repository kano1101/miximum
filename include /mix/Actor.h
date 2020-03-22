#pragma once

namespace Mix
{

  class Actor
  {
  public:
    virtual ~Actor() {}
    virtual void Update() {}
    virtual void Draw() const {}
  };

} // namespace Mix
