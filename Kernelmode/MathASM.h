extern "C" {
	float __sinf(const float);
	float __cosf(const float);
	float __sqrtf(const float);
	float __tanf(const float a);
	float __atan2f(const float, const float);
	__forceinline float __atanf(const float a) {
		return __atan2f(a, 1.f);
	}
	__forceinline float __asinf(const float a) {
		return __atan2f(a, __sqrtf(1.f - a * a));
	}
	__forceinline float __acosf(const float a) {
		return __atan2f(__sqrtf(1.f - a * a), a);
	}
}


