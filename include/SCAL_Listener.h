#pragma once
#include <memory>
#include <Windows.h>
#include <DirectXMath.h>
#include <x3daudio.h>
#include "../include/SCAL_Math.h"

namespace scal
{
	/// <summary>
	/// <para>リスナークラス</para>
	/// <para>3Dサウンドの聴く側のクラスです</para>
	/// </summary>
	class Listener
	{
	public:
		Listener();
		~Listener();

		/// <summary>
		/// 座標を設定する
		/// </summary>
		/// <param name="pos">座標</param>
		void SetPosition(const Vector3& pos);

		/// <summary>
		/// <para>デフォルトの向きを設定する</para>
		/// <para>正面と上は直角である必要あり</para>
		/// </summary>
		/// <param name="front">正面</param>
		/// <param name="up">上</param>
		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		/// <summary>
		/// 回転を設定する
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="rot">回転角度</param>
		void SetRotate(const Vector3& axis, float rot);

		/// <summary>
		/// 回転を設定する
		/// </summary>
		/// <param name="x">x回転</param>
		/// <param name="y">y回転</param>
		/// <param name="z">z回転</param>
		void SetRotate(float x, float y, float z);

		/// <summary>
		/// 回転を設定する
		/// </summary>
		/// <param name="quat">クォータニオン</param>
		void SetRotateAsQuaternion(const Quaternion& quat);

		/// <summary>
		/// 回転を加算する
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="rot">回転角度</param>
		void AddRotate(const Vector3& axis, float rot);

		/// <summary>
		/// 回転を加算する
		/// </summary>
		/// <param name="quat">クォータニオン</param>
		void AddRotateAsQuaternion(const Quaternion& quat);

		/// <summary>
		/// 回転を取得する
		/// </summary>
		/// <returns>回転用のクォータニオンの参照</returns>
		Quaternion& GetRotateQuaternionRef(void);

		/// <summary>
		/// 速度を有効にするか
		/// </summary>
		/// <param name="enable">有効にするならtrue</param>
		void EnableVelocity(bool enable = true);

		/// <summary>
		/// 速度を更新する
		/// </summary>
		void UpdateVelocity(void);

		/// <summary>
		/// <para>生のリスナーを取得する</para>
		/// <para>通常は使用する必要なし</para>
		/// </summary>
		/// <returns>リスナーオブジェクト</returns>
		X3DAUDIO_LISTENER& GetRawListener(void);
	private:
		class Listener_Impl;
		std::unique_ptr<Listener_Impl> impl_;
	};

}
