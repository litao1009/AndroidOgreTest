#include "MayaCamera.h"

#include "Ogre.h"

class	MayaCamera::Imp
{
public:

	Ogre::Camera*		Camera_{};
	Ogre::SceneManager*	Smgr_{};
	Ogre::SceneNode*	PositionNode_{};
	Ogre::SceneNode*	TargetNode_{};
	Ogre::SceneNode*	Yaw_{};
	Ogre::SceneNode*	Pitch_{};
	float 				RotationSpeed_{.25f};
	bool				PointerMoving_{false};
	PointerState		LastPointerState_;

public:

	void	UpdateData(const Ogre::Vector3& target, const Ogre::Vector3& pos)
	{
		//视线方向Z
		auto rotateTo = pos - target;

		TargetNode_->setPosition(target);
		PositionNode_->setPosition(0, 0, rotateTo.normalise());

		auto pitch = Ogre::Quaternion::IDENTITY;
		auto yaw = Ogre::Quaternion::IDENTITY;

		auto step1 = rotateTo;
		step1.y = 0;
		auto length = step1.normalise();

		if ( length > 0.f )
		{
			yaw = step1.getRotationTo(Ogre::Vector3::UNIT_Z);
		}

		auto step2 =  yaw * rotateTo;

		pitch = step2.getRotationTo(Ogre::Vector3::UNIT_Z);

		Pitch_->setOrientation(pitch.Inverse());
		Yaw_->setOrientation(yaw.Inverse());
	}
};

MayaCamera::MayaCamera( Ogre::Camera *camera ):ICameraFrameListener(camera), ImpUPtr_(new Imp)
{
	auto& imp_ = *ImpUPtr_;

	imp_.Camera_ = camera;
	imp_.Smgr_ = camera->getSceneManager();

	imp_.TargetNode_ = imp_.Smgr_->getRootSceneNode()->createChildSceneNode();
	imp_.TargetNode_->setName("MayaCamera::TargetNode_");
	imp_.Yaw_ = imp_.TargetNode_->createChildSceneNode();
	imp_.Yaw_->setName("MayaCamera::Yaw_");
	imp_.Pitch_ = imp_.Yaw_->createChildSceneNode();
	imp_.Pitch_->setName("MayaCamera::Pitch_");
	imp_.PositionNode_ = imp_.Pitch_->createChildSceneNode();
	imp_.PositionNode_->setName("MayaCamera::PositionNode_");
	imp_.PositionNode_->setPosition(0, 0, 150.f);

	imp_.Camera_->detachFromParent();
	imp_.PositionNode_->attachObject(imp_.Camera_);

	imp_.Camera_->setNearClipDistance(20);
	imp_.Camera_->setAutoAspectRatio(true);
	imp_.Camera_->setFOVy(Ogre::Degree(60));
	imp_.Camera_->setProjectionType(Ogre::PT_PERSPECTIVE);
	imp_.Camera_->setFixedYawAxis(true);
	imp_.Camera_->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
}

MayaCamera::~MayaCamera()
{ }

void MayaCamera::SetPosAndTarget( const Ogre::Vector3 &pos, const Ogre::Vector3 &target )
{
	auto& imp_ = *ImpUPtr_;

	imp_.UpdateData(target, pos);
}

void MayaCamera::SetPosition( const Ogre::Vector3 &pos )
{
	auto& imp_ = *ImpUPtr_;

	imp_.UpdateData(imp_.TargetNode_->getPosition(), pos);
}

Ogre::Vector3 MayaCamera::GetPosition() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.PositionNode_->_getDerivedPosition();
}

void MayaCamera::SetTarget( const Ogre::Vector3 &target )
{
	auto& imp_ = *ImpUPtr_;

	imp_.UpdateData(target, imp_.PositionNode_->_getDerivedPosition());
}

Ogre::Vector3 MayaCamera::GetTarget() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.TargetNode_->getPosition();
}

void MayaCamera::SetFocusLength( float length )
{
	auto& imp_ = *ImpUPtr_;

	imp_.PositionNode_->setPosition(0, 0, length);
}

float MayaCamera::GetFocusLength() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.PositionNode_->getPosition().z;
}

void MayaCamera::SetRotationSpeed( float rSpeed )
{
	auto& imp_ = *ImpUPtr_;

	imp_.RotationSpeed_ = rSpeed;
}

float MayaCamera::GetRotationSpeed() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.RotationSpeed_;
}

void MayaCamera::_FrameStart( const Ogre::FrameEvent &fevt )
{
	auto& imp_ = *ImpUPtr_;

	auto& evtRecoder = GetEventRecorder();

	if ( !imp_.PointerMoving_ && evtRecoder.GetPointerRecorder().HasPressed() )
	{
		imp_.LastPointerState_ = EventRecorder::GetStaticPointerState();

		imp_.PointerMoving_ = true;
	}
	else
	if ( imp_.PointerMoving_ && evtRecoder.GetPointerRecorder().HasReleased() )
	{
		imp_.PointerMoving_ = false;
	}

	if ( imp_.PointerMoving_ )
	{
		auto offset = EventRecorder::GetStaticPointerState() - imp_.LastPointerState_;

		if ( 0 != offset.X.rel || 0 != offset.Y.rel )
		{
			auto yaw = Ogre::Degree(-offset.X.rel * imp_.RotationSpeed_) + imp_.Yaw_->getOrientation().getYaw();
			Ogre::Quaternion quaYaw;
			quaYaw.FromAngleAxis(yaw, Ogre::Vector3::UNIT_Y);
			imp_.Yaw_->setOrientation(quaYaw);

			auto pitch = Ogre::Degree(-offset.Y.rel * imp_.RotationSpeed_) + imp_.Pitch_->getOrientation().getPitch();
			Ogre::Quaternion quaPitch;
			quaPitch.FromAngleAxis(pitch, Ogre::Vector3::UNIT_X);

			auto pitchAngle = quaPitch.getPitch();
			auto tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

			if ( pitchAngle > Ogre::Radian(Ogre::Degree(90 - tooSmall)) )
			{
				quaPitch.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_X);
			}
			else if ( pitchAngle < Ogre::Radian(Ogre::Degree(-90 + tooSmall)) )
			{
				quaPitch.FromAngleAxis(Ogre::Degree(-90), Ogre::Vector3::UNIT_X);
			}

			imp_.Pitch_->setOrientation(quaPitch);

			imp_.LastPointerState_ = EventRecorder::GetStaticPointerState();
		}
	}
}

void MayaCamera::_Attach()
{ }