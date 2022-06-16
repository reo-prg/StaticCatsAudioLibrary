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
		void SetRotateAsQuaternion(const XMVECTOR& quat);
		void AddRotate(const Vector3& axis, float rot);
		void AddRotateAsQuaternion(const XMVECTOR& quat);

		void EnableVelocity(bool enable);

		X3DAUDIO_LISTENER listener_;

		XMFLOAT4 defaultFrontVector_;
		XMFLOAT4 defaultUpVector_;

		XMFLOAT3 lastpos_;

		XMVECTOR rotate_;

		bool isVelocityEnable_ = false;
	private:
		void Apply(void);

		bool isPositionUpdated_ = false;
	};

	Listener::Listener_Impl::Listener_Impl()
	{
		rotate_ = XMQuaternionIdentity();

		defaultFrontVector_ = { -1.0f, 0.0f, 0.0f, 1.0f };
		defaultUpVector_ = { 0.0f, 1.0f, 0.0f, 1.0f };

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

		listener_.Velocity = listener_.Position - lastpos_;

		lastpos_ = listener_.Position;
	}

	void Listener::Listener_Impl::SetPosition(const Vector3& pos)
	{
		listener_.Position = { pos.x_, pos.y_, pos.z_ };

		if (!isPositionUpdated_)
		{
			lastpos_ = { pos.x_, pos.y_, pos.z_ };
			isPositionUpdated_ = true;
		}
	}

	void Listener::Listener_Impl::SetDefaultDirection(const Vector3& front, const Vector3& up)
	{
		defaultFrontVector_ = { front.x_, front.y_, front.z_, 1.0f };
		defaultUpVector_ = { up.x_, up.y_, up.z_, 1.0f };

		Apply();
	}

	void Listener::Listener_Impl::SetRotate(const Vector3& axis, float rot)
	{
		rotate_ = XMQuaternionRotationAxis({ axis.x_, axis.y_, axis.z_ }, rot);

		Apply();
	}

	void Listener::Listener_Impl::SetRotate(float x, float y, float z)
	{
		rotate_ = XMQuaternionRotationRollPitchYaw(y, z, x);

		Apply();
	}

	void Listener::Listener_Impl::SetRotateAsQuaternion(const XMVECTOR& quat)
	{
		rotate_ = quat;
		Apply();
	}

	void Listener::Listener_Impl::AddRotate(const Vector3& axis, float rot)
	{
		auto&& q = XMQuaternionRotationAxis({ axis.x_, axis.y_, axis.z_ }, rot);

		rotate_ = XMQuaternionMultiply(rotate_, q);

		Apply();
	}

	void Listener::Listener_Impl::AddRotateAsQuaternion(const XMVECTOR& quat)
	{
		rotate_ = XMQuaternionMultiply(rotate_, quat);
		
		Apply();
	}

	void Listener::Listener_Impl::EnableVelocity(bool enable)
	{
		isVelocityEnable_ = enable;
	}

	void Listener::Listener_Impl::Apply(void)
	{
		auto&& m = XMMatrixRotationQuaternion(rotate_);

		auto&& f = XMVector4Transform(XMLoadFloat4(&defaultFrontVector_), m);
		auto&& u = XMVector4Transform(XMLoadFloat4(&defaultUpVector_), m);


		listener_.OrientFront = { f.m128_f32[0], f.m128_f32[1], f.m128_f32[2] };
		listener_.OrientTop = { u.m128_f32[0], u.m128_f32[1], u.m128_f32[2] };
	}


	Listener::Listener()
	{
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

	void Listener::SetRotateAsQuaternion(const XMVECTOR& quat)
	{
		impl_->SetRotateAsQuaternion(quat);
	}

	void Listener::AddRotate(const Vector3& axis, float rot)
	{
		impl_->AddRotate(axis, rot);
	}
	void Listener::AddRotateAsQuaternion(const XMVECTOR& quat)
	{
		impl_->AddRotateAsQuaternion(quat);
	}
	void Listener::EnableVelocity(bool enable)
	{
		impl_->EnableVelocity(enable);
	}
	void Listener::UpdateVelocity(void)
	{
		impl_->UpdateVelocity();
	}
}
