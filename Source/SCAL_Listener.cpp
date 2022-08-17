#include "../include/SCAL_Listener.h"
#include <x3daudio.h>

using namespace DirectX;

namespace scal
{
	using namespace xm_operator;

	class Listener::Listener_Impl
	{
	public:
		Listener_Impl();
		~Listener_Impl();

		void UpdateVelocity(void);

		void SetPosition(const Vector3& pos);

		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		void SetRotate(const Vector3& axis, float rot);
		void SetRotate(float x, float y, float z);
		void SetRotateAsQuaternion(const Quaternion& quat);
		void AddRotate(const Vector3& axis, float rot);
		void AddRotateAsQuaternion(const Quaternion& quat);

		Quaternion& GetRotateQuaternionRef(void);

		void EnableVelocity(bool enable);

		X3DAUDIO_LISTENER& GetRawListener(void);

		X3DAUDIO_LISTENER listener_;

		Vector3 defaultFrontVector_;
		Vector3 defaultUpVector_;

		Vector3 lastpos_;

		Quaternion rotate_;

		bool isVelocityEnable_ = false;
	private:
		void Apply(void);

		bool isPositionUpdated_ = false;
	};

	Listener::Listener_Impl::Listener_Impl() : rotate_()
	{
		defaultFrontVector_ = { -1.0f, 0.0f, 0.0f };
		defaultUpVector_ = { 0.0f, 1.0f, 0.0f };

		listener_ = { };

		listener_.OrientFront = { defaultFrontVector_.x, defaultFrontVector_.y, defaultFrontVector_.z };
		listener_.OrientTop = { defaultUpVector_.x, defaultUpVector_.y, defaultUpVector_.z };

		listener_.Position = { 0.0f, 0.0f, 0.0f };

		listener_.Velocity = { 0.0f, 0.0f, 0.0f };
		listener_.pCone = nullptr;
	}

	Listener::Listener_Impl::~Listener_Impl()
	{
		
	}

	void Listener::Listener_Impl::UpdateVelocity(void)
	{
		if (!isVelocityEnable_) { return; }

		listener_.Velocity = GetFloat3(listener_.Position - lastpos_);

		lastpos_ = listener_.Position;
	}

	void Listener::Listener_Impl::SetPosition(const Vector3& pos)
	{
		listener_.Position = { pos.x, pos.y, pos.z };

		if (!isPositionUpdated_)
		{
			lastpos_ = pos;
			isPositionUpdated_ = true;
		}
	}

	void Listener::Listener_Impl::SetDefaultDirection(const Vector3& front, const Vector3& up)
	{
		defaultFrontVector_ = front;
		defaultUpVector_ = up;

		Apply();
	}

	void Listener::Listener_Impl::SetRotate(const Vector3& axis, float rot)
	{
		rotate_ = Quaternion::RotateAxis(axis, rot);

		Apply();
	}

	void Listener::Listener_Impl::SetRotate(float x, float y, float z)
	{
		rotate_.SetRotationEulerAngle(x, y, z);

		Apply();
	}

	void Listener::Listener_Impl::SetRotateAsQuaternion(const Quaternion& quat)
	{
		rotate_ = quat;
		Apply();
	}

	void Listener::Listener_Impl::AddRotate(const Vector3& axis, float rot)
	{
		auto&& q = Quaternion::RotateAxis(axis, rot);

		rotate_ = rotate_ * q;

		Apply();
	}

	void Listener::Listener_Impl::AddRotateAsQuaternion(const Quaternion& quat)
	{
		rotate_ = rotate_ * quat;
		
		Apply();
	}

	Quaternion& Listener::Listener_Impl::GetRotateQuaternionRef(void)
	{
		return rotate_;
	}

	void Listener::Listener_Impl::EnableVelocity(bool enable)
	{
		isVelocityEnable_ = enable;
	}

	X3DAUDIO_LISTENER& Listener::Listener_Impl::GetRawListener(void)
	{
		return listener_;
	}

	void Listener::Listener_Impl::Apply(void)
	{
		auto&& m = rotate_.GetRotationMatrix();

		auto&& f = TranslationMatrix(defaultFrontVector_);
		auto&& u = TranslationMatrix(defaultUpVector_);


		listener_.OrientFront = { f.matrix_[0][3], f.matrix_[1][3], f.matrix_[2][3] };
		listener_.OrientTop = { u.matrix_[0][3], u.matrix_[1][3], u.matrix_[2][3] };
	}


	Listener::Listener()
	{
		impl_ = std::make_unique<Listener_Impl>();
	}

	Listener::~Listener()
	{
	}

	void Listener::SetPosition(const Vector3& pos)
	{
		impl_->SetPosition(pos);
	}

	void Listener::SetDefaultDirection(const Vector3& front, const Vector3& up)
	{
		impl_->SetDefaultDirection(front, up);
	}

	void Listener::SetRotate(const Vector3& axis, float rot)
	{
		impl_->SetRotate(axis, rot);
	}

	void Listener::SetRotate(float x, float y, float z)
	{
		impl_->SetRotate(x, y, z);
	}

	void Listener::SetRotateAsQuaternion(const Quaternion& quat)
	{
		impl_->SetRotateAsQuaternion(quat);
	}

	void Listener::AddRotate(const Vector3& axis, float rot)
	{
		impl_->AddRotate(axis, rot);
	}
	void Listener::AddRotateAsQuaternion(const Quaternion& quat)
	{
		impl_->AddRotateAsQuaternion(quat);
	}
	Quaternion& Listener::GetRotateQuaternionRef(void)
	{
		return impl_->GetRotateQuaternionRef();
	}
	void Listener::EnableVelocity(bool enable)
	{
		impl_->EnableVelocity(enable);
	}
	void Listener::UpdateVelocity(void)
	{
		impl_->UpdateVelocity();
	}
	X3DAUDIO_LISTENER& Listener::GetRawListener(void)
	{
		return impl_->GetRawListener();
	}
}
