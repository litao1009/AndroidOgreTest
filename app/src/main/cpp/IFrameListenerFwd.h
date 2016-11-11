#pragma once

#include <memory>

class	IFrameListener;
class	ICameraFrameListener;
class	ISceneFrameListener;

using	IFrameListenerSPtr = std::shared_ptr<IFrameListener>;
using	ICameraFrameListenerSPtr = std::shared_ptr<ICameraFrameListener>;
using	ISceneFrameListenerSPtr = std::shared_ptr<ISceneFrameListener>;
using	IFrameListenerWPtr = std::weak_ptr<IFrameListener>;
using	ICameraFrameListenerWPtr = std::weak_ptr<ICameraFrameListener>;
using	ISceneFrameListenerWPtr = std::weak_ptr<ISceneFrameListener>;