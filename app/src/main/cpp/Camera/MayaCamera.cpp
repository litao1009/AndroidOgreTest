#include "MayaCamera.h"

#include "Ogre.h"

class	MayaCamera::Imp
{
public:

	Ogre::Camera*		Camera_{};
	Ogre::SceneManager*	Smgr_{};
	Ogre::SceneNode*	CameraNode_{};
	Ogre::Quaternion	Yaw_ = Ogre::Quaternion::IDENTITY;
	Ogre::Quaternion	Pitch_ = Ogre::Quaternion::IDENTITY;
	Ogre::Quaternion	Roll_ = Ogre::Quaternion::IDENTITY;
	Ogre::Quaternion	Rotation_ = Ogre::Quaternion::IDENTITY;
	Ogre::Vector3		Target_{};
	Ogre::Vector3		Position_{};
	float 				FocusLen_{150.f};
	float 				RotationSpeed_{.25f};
	bool				PointerMoving_{false};
	PointerState		LastPointerState_;

public:

	void	UpdateNode()
	{
		Rotation_ = Roll_ * Yaw_ * Pitch_;
		Position_ = (Rotation_ * Ogre::Vector3::UNIT_Z) * FocusLen_ + Target_;

		CameraNode_->setPosition(Position_);
		CameraNode_->setOrientation(Rotation_);
	}

	void	UpdateData(const Ogre::Vector3& target, const Ogre::Vector3& pos)
	{
		//视线方向Z
		auto rotateTo = pos - target;

		Target_ = target;
		Position_ = pos;
		FocusLen_ = rotateTo.normalise();

		Pitch_ = Ogre::Quaternion::IDENTITY;
		Yaw_ = Ogre::Quaternion::IDENTITY;

		auto step1 = rotateTo;
		step1.y = 0;
		auto length = step1.normalise();

		if ( length > 0.f )
		{
			Yaw_ = step1.getRotationTo(Ogre::Vector3::UNIT_Z);
		}

		auto step2 =  Yaw_ * rotateTo;

		Pitch_ = step2.getRotationTo(Ogre::Vector3::UNIT_Z);

		Pitch_ = Pitch_.Inverse();
		Yaw_ = Yaw_.Inverse();

		UpdateNode();
	}

	void	Init(Ogre::Camera* camera)
	{
		Camera_ = camera;
		Smgr_ = camera->getSceneManager();

		CameraNode_ = Smgr_->getRootSceneNode()->createChildSceneNode();
		CameraNode_->setName("MayaCamera::CameraNode_");
		CameraNode_->setPosition(0, 0, FocusLen_);

		Camera_->detachFromParent();
		CameraNode_->attachObject(Camera_);

		Camera_->setNearClipDistance(20);
		Camera_->setAutoAspectRatio(true);
		Camera_->setFOVy(Ogre::Degree(60));
		Camera_->setProjectionType(Ogre::PT_PERSPECTIVE);
		Camera_->setFixedYawAxis(true);
		Camera_->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
	}
};

MayaCamera::MayaCamera( Ogre::Camera *camera ):ICameraFrameListener(camera), ImpUPtr_(new Imp)
{
	ImpUPtr_->Init(camera);
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

	imp_.UpdateData(imp_.Target_, pos);
}

const Ogre::Vector3& MayaCamera::GetPosition() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.Position_;
}

void MayaCamera::SetTarget( const Ogre::Vector3 &target )
{
	auto& imp_ = *ImpUPtr_;

	imp_.UpdateData(target, imp_.Position_);
}

const Ogre::Vector3& MayaCamera::GetTarget() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.Target_;
}

void MayaCamera::SetFocusLength( float length )
{
	auto& imp_ = *ImpUPtr_;

	imp_.FocusLen_ = length;
	imp_.Position_ = imp_.Target_ + (imp_.Position_ - imp_.Target_).normalisedCopy() * imp_.FocusLen_;

	imp_.UpdateData(imp_.Target_, imp_.Position_);
}

float MayaCamera::GetFocusLength() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.FocusLen_;
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

	auto needUpdate = false;

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
			auto yaw = Ogre::Degree(-offset.X.rel * imp_.RotationSpeed_) + imp_.Yaw_.getYaw();
			imp_.Yaw_.FromAngleAxis(yaw, Ogre::Vector3::UNIT_Y);

			auto pitch = Ogre::Degree(-offset.Y.rel * imp_.RotationSpeed_) + imp_.Pitch_.getPitch();
			imp_.Pitch_.FromAngleAxis(pitch, Ogre::Vector3::UNIT_X);

			auto pitchAngle = imp_.Pitch_.getPitch();
			auto tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

			if ( pitchAngle > Ogre::Radian(Ogre::Degree(90 - tooSmall)) )
			{
				imp_.Pitch_.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_X);
			}
			else if ( pitchAngle < Ogre::Radian(Ogre::Degree(-90 + tooSmall)) )
			{
				imp_.Pitch_.FromAngleAxis(Ogre::Degree(-90), Ogre::Vector3::UNIT_X);
			}

			imp_.LastPointerState_ = EventRecorder::GetStaticPointerState();

			needUpdate = true;
		}
	}

	if ( needUpdate )
	{
		imp_.UpdateNode();
	}
}