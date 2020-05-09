//
// created by 狩野 亮 on 2020/03/29.
// Copyright (c) 2020年 狩野 亮. All rights reserved.
//

#ifndef __MIX_ACTOR_H__
#define __MIX_ACTOR_H__

namespace Mix
{

  template<class Renderer>
  class Actor
  {
  public:
    virtual ~Actor() {}
    virtual void Update() = 0;
    virtual void Draw(Renderer&) = 0;
  };

} // namespace Mix

#endif
