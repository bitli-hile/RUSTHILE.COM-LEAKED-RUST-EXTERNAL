
float sqrtn(const float& x)
{
	union
	{
		int i;
		float x;
	} u;
	u.x = x;
	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

	u.x = u.x + x / u.x;
	u.x = 0.25f * u.x + x / u.x;

	return u.x;
}


class vector3
{
public:
	float x;
	float y;
	float z;
	vector3()
	{}
	vector3(float x, float y, float z)
		: x(x), y(y), z(z)
	{}
#define M_PI_2 1.57079632679489661923
#define M_PI 3.14159265358979323846
#define RAD_TO_DEG 180 / 3.141592654f
#define abs(a) ((a) >= 0) ? (a) : -(a)
	bool is_empty() { return x == 0 && y == 0 && z == 0; }



	static vector3 Zero() {
		return vector3(0.0f, 0.0f, 0.0f);
	}
	static float my_sqrt(float number)
	{
		long i;
		float x2, y;
		const float threehalfs = 1.5F;
		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y; // floating point bit level hacking [sic]
		i = 0x5f3759df - (i >> 1); // Newton's approximation
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y)); // 1st iteration
		y = y * (threehalfs - (x2 * y * y)); // 2nd iteration
		return 1 / y;
	}
	inline float Magnitude2D()
	{
		return my_sqrt(this->x * this->x + this->z * this->z);
	}
	vector3 midPoint(vector3 v2)
	{
		return vector3((x + v2.x) / 2, (y + v2.y) / 2, (z + v2.z) / 2);
	}

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}


	inline float Length()
	{
		return my_sqrt((x * x) + (y * y) + (z * z));
	}
	static double my_atan(double x)
	{
		// Return arctangent(x) given that 5/3 < x <= 2, with the same properties as atan.
		auto atani5 = [](double x) {
			constexpr double p00 = +0x1.124A85750FB5Cp+00;
			constexpr double p01 = +0x1.D59AE78C11C49p-03;
			constexpr double p02 = -0x1.8AD3C44F10DC3p-04;
			constexpr double p03 = +0x1.2B090AAD5F9DCp-05;
			constexpr double p04 = -0x1.881EC3D15241Fp-07;
			constexpr double p05 = +0x1.8CB82A74E0699p-09;
			constexpr double p06 = -0x1.3182219E21362p-12;
			constexpr double p07 = -0x1.2B9AD13DB35A8p-12;
			constexpr double p08 = +0x1.10F884EAC0E0Ap-12;
			constexpr double p09 = -0x1.3045B70E93129p-13;
			constexpr double p10 = +0x1.00B6A460AC05Dp-14;
			double y = x - 0x1.d555555461337p0;
			return ((((((((((+p10) * y + p09) * y + p08) * y + p07) * y + p06) * y +
				p05) *
				y +
				p04) *
				y +
				p03) *
				y +
				p02) *
				y +
				p01) *
				y +
				p00;
		};
		// Return arctangent(x) given that 4/3 < x <= 5/3, with the same properties as
		// atan.
		auto atani4 = [](double x) {
			constexpr double p00 = +0x1730BD281F69Dp-01;
			constexpr double p01 = +0x1.3B13B13B13B0Cp-02;
			constexpr double p02 = -0x1.22D719C06115Ep-03;
			constexpr double p03 = +0x1.C963C83985742p-05;
			constexpr double p04 = -0x1.135A0938EC462p-06;
			constexpr double p05 = +0x1.13A254D6E5B7Cp-09;
			constexpr double p06 = +0x1.DFAA5E77B7375p-10;
			constexpr double p07 = -0x14AC1342182D2p-10;
			constexpr double p08 = +0x1.25BAD4D85CBE1p-10;
			constexpr double p09 = -0x1.E4EEF429EB680p-12;
			constexpr double p10 = +0x1.B4E30D1BA3819p-14;
			constexpr double p11 = +0x1.0280537F097F3p-15;
			double y = x - 0x1.8000000000003p0;
			return (((((((((((+p11) * y + p10) * y + p09) * y + p08) * y + p07) * y +
				p06) *
				y +
				p05) *
				y +
				p04) *
				y +
				p03) *
				y +
				p02) *
				y +
				p01) *
				y +
				p00;
		};
		// Return arctangent(x) given that 1 < x <= 4 / 3, with the same properties as
		// atan.
		auto atani3 = [](double x) {
			constexpr double p00 = +0x1.B96E5A78C5C40p-01;
			constexpr double p01 = +0x1.B1B1B1B1B1B3Dp-02;
			constexpr double p02 = -0x1.AC97826D58470p-03;
			constexpr double p03 = +0x1.3FD2B9F586A67p-04;
			constexpr double p04 = -0x1.BC317394714B7p-07;
			constexpr double p05 = -0x1.2B01FC60CC37Ap-07;
			constexpr double p06 = +0x1.73A9328786665p-07;
			constexpr double p07 = -0x1.C0B993A09CE31p-08;
			constexpr double p08 = +0x1.2FCDACDD6E5B5p-09;
			constexpr double p09 = +0x1.CBD49DA316282p-13;
			constexpr double p10 = -0x1.0120E602F6336p-10;
			constexpr double p11 = +0x1.A89224FF69018p-11;
			constexpr double p12 = -0x1.883D8959134B3p-12;

			double y = x - 0x1.2aaaaaaaaaa96p0;

			return ((((((((((((+p12) * y + p11) * y + p10) * y + p09) * y + p08) * y +
				p07) *
				y +
				p06) *
				y +
				p05) *
				y +
				p04) *
				y +
				p03) *
				y +
				p02) *
				y +
				p01) *
				y +
				p00;
		};
		// Return arctangent(x) given that 3 / 4 < x <= 1, with the same properties as
		// atan.
		auto atani2 = [](double x) {
			constexpr double p00 = +0x1.700A7C580EA7Ep-01;
			constexpr double p01 = +0x1.21FB781196AC3p-01;
			constexpr double p02 = -0x1.1F6A8499714A2p-02;
			constexpr double p03 = +0x1.41B15E5E8DCD0p-04;
			constexpr double p04 = +0x1.59BC93F81895Ap-06;
			constexpr double p05 = -0x1.63B543EFFA4EFp-05;
			constexpr double p06 = +0x1.C90E92AC8D86Cp-06;
			constexpr double p07 = -0x1.91F7E2A7A338Fp-08;
			constexpr double p08 = -0x1.AC1645739E676p-08;
			constexpr double p09 = +0x1.152311B180E6Cp-07;
			constexpr double p10 = -0x1.265EF51B17DB7p-08;
			constexpr double p11 = +0x1.CA7CDE5DE9BD7p-14;

			double y = x - 0x1.c0000000f4213p-1;

			return (((((((((((+p11) * y + p10) * y + p09) * y + p08) * y + p07) * y +
				p06) *
				y +
				p05) *
				y +
				p04) *
				y +
				p03) *
				y +
				p02) *
				y +
				p01) *
				y +
				p00;
		};
		// Return arctangent(x) given that 1/2 < x <= 3/4, with the same properties as
		// atan.
		auto atani1 = [](double x) {
			constexpr double p00 = +0x1.1E00BABDEFED0p-1;
			constexpr double p01 = +0x1.702E05C0B8155p-1;
			constexpr double p02 = -0x1.4AF2B78215A1Bp-2;
			constexpr double p03 = +0x1.5D0B7E9E69054p-6;
			constexpr double p04 = +0x1.A1247CA5D9475p-4;
			constexpr double p05 = -0x1.519E110F61B54p-4;
			constexpr double p06 = +0x1.A759263F377F2p-7;
			constexpr double p07 = +0x1.094966BE2B531p-5;
			constexpr double p08 = -0x1.09BC0AB7F914Cp-5;
			constexpr double p09 = +0x1F3B7C531AA4Ap-8;
			constexpr double p10 = +0x1.950E69DCDD967p-7;
			constexpr double p11 = -0x1.D88D31ABC3AE5p-7;
			constexpr double p12 = +0x1.10F3E20F6A2E2p-8;

			double y = x - 0x1.4000000000027p-1;

			return ((((((((((((+p12) * y + p11) * y + p10) * y + p09) * y + p08) * y +
				p07) *
				y +
				p06) *
				y +
				p05) *
				y +
				p04) *
				y +
				p03) *
				y +
				p02) *
				y +
				p01) *
				y +
				p00;
		};
		// Return arctangent(x) given that 0x1p-27 < |x| <= 1/2, with the same properties
		// as atan.
		auto atani0 = [](double x) {
			constexpr double p03 = -0x1.555555555551Bp-2;
			constexpr double p05 = +0x1.99999999918D8p-3;
			constexpr double p07 = -0x1.2492492179CA3p-3;
			constexpr double p09 = +0x1.C71C7096C2725p-4;
			constexpr double p11 = -0x1.745CF51795B21p-4;
			constexpr double p13 = +0x1.3B113F18AC049p-4;
			constexpr double p15 = -0x1.10F31279EC05Dp-4;
			constexpr double p17 = +0x1.DFE7B9674AE37p-5;
			constexpr double p19 = -0x1.A38CF590469ECp-5;
			constexpr double p21 = +0x1.56CDB5D887934p-5;
			constexpr double p23 = -0x1.C0EB85F543412p-6;
			constexpr double p25 = +0x1.4A9F5C4724056p-7;

			// Square x.
			double x2 = x * x;

			return ((((((((((((+p25) * x2 + p23) * x2 + p21) * x2 + p19) * x2 + p17) *
				x2 +
				p15) *
				x2 +
				p13) *
				x2 +
				p11) *
				x2 +
				p09) *
				x2 +
				p07) *
				x2 +
				p05) *
				x2 +
				p03) *
				x2 * x +
				x;
		};
		// Return arctangent(x) given that 2 < x, with the same properties as atan.
		auto Tail = [](double x) {
			{
				constexpr double HalfPi = 0x3.243f6a8885a308d313198a2e037ap-1;

				// For large x, generate inexact and return pi/2.
				if (0x1p53 <= x)
					return HalfPi + DBL_EPSILON;
				if (x != x) // isnan
					return x - x;
			}

			constexpr double p03 = -0x1.5555555554A51p-2;
			constexpr double p05 = +0x1.999999989EBCAp-3;
			constexpr double p07 = -0x1.249248E1422E3p-3;
			constexpr double p09 = +0x1.C71C5EDFED480p-4;
			constexpr double p11 = -0x1.745B7F2D72663p-4;
			constexpr double p13 = +0x1.3AFD7A0E6EB75p-4;
			constexpr double p15 = -0x1.104146B1A1AE8p-4;
			constexpr double p17 = +0x1.D78252FA69C1Cp-5;
			constexpr double p19 = -0x1.81D33E401836Dp-5;
			constexpr double p21 = +0x1.007733E06CEB3p-5;
			constexpr double p23 = -0x1.83DAFDA7BD3FDp-7;

			constexpr double p000 = +0x1.921FB54442D18p0;
			constexpr double p001 = +0x1.1A62633145C07p-54;

			double y = 1 / x;

			// Square y.
			double y2 = y * y;

			return p001 -
				((((((((((((+p23) * y2 + p21) * y2 + p19) * y2 + p17) * y2 + p15) *
					y2 +
					p13) *
					y2 +
					p11) *
					y2 +
					p09) *
					y2 +
					p07) *
					y2 +
					p05) *
					y2 +
					p03) *
					y2 * y +
					y) +
				p000;
		};

		if (x < 0)
			if (x < -1)
				if (x < -5 / 3.)
					if (x < -2)
						return -Tail(-x);
					else
						return -atani5(-x);
				else if (x < -4 / 3.)
					return -atani4(-x);
				else
					return -atani3(-x);
			else if (x < -.5)
				if (x < -.75)
					return -atani2(-x);
				else
					return -atani1(-x);
			else if (x < -0x1.d12ed0af1a27fp-27)
				return atani0(x);
			else if (x <= -0x1p-1022)
				// Generate inexact and return x.
				return (DBL_EPSILON + 1) * x;
			else if (x == 0)
				return x;
			else
				// Generate underflow and return x.
				return x * DBL_EPSILON + x;
		else if (x <= +1)
			if (x <= +.5)
				if (x <= +0x1.d12ed0af1a27fp-27)
					if (x < +0x1p-1022)
						if (x == 0)
							return x;
						else
							// Generate underflow and return x.
							return x * DBL_EPSILON + x;
					else
						// Generate inexact and return x.
						return (DBL_EPSILON + 1) * x;
				else
					return atani0(x);
			else if (x <= +.75)
				return +atani1(+x);
			else
				return +atani2(+x);
		else if (x <= +5 / 3.)
			if (x <= +4 / 3.)
				return +atani3(+x);
			else
				return +atani4(+x);
		else if (x <= +2)
			return +atani5(+x);
		else
			return +Tail(+x);
	}

	static float my_sin(float x)
	{
		// useful to pre-calculate
		double x2 = x * x;
		double x4 = x2 * x2;

		// Calculate the terms
		// As long as abs(x) < sqrt(6), which is 2.45, all terms will be positive.
		// Values outside this range should be reduced to [-pi/2, pi/2] anyway for
		// accuracy. Some care has to be given to the factorials. They can be
		// pre-calculated by the compiler, but the value for the higher ones will exceed
		// the storage capacity of int. so force the compiler to use unsigned long longs
		// (if available) or doubles.
		double t1 = x * (1.0 - x2 / (2 * 3));
		double x5 = x * x4;
		double t2 = x5 * (1.0 - x2 / (6 * 7)) / (1.0 * 2 * 3 * 4 * 5);
		double x9 = x5 * x4;
		double t3 = x9 * (1.0 - x2 / (10 * 11)) / (1.0 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9);
		double x13 = x9 * x4;
		double t4 = x13 * (1.0 - x2 / (14 * 15)) /
			(1.0 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13);
		double x14 = x13 * x4;
		double t5 =
			x14 * (1.0 - x2 / (18 * 19)) /
			(1.0 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16 * 17);
		double x15 = x14 * x4;
		double t6 = x15 * (1.0 - x2 / (22 * 23)) /
			(1.0 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 *
				16 * 17 * 18 * 19 * 20 * 21);
		// add some more if your accuracy requires them.
		// But remember that x is smaller than 2, and the factorial grows very fast
		// so I doubt that 2^17 / 17! will add anything.
		// Even t4 might already be too small to matter when compared with t1.

		// Sum backwards
		double result = t6;
		result += t5;
		result += t4;
		result += t3;
		result += t2;
		result += t1;

		return result;
	}

	float length_2d() { return my_sqrt((x * x) + (z * z)); }

	static float my_cos(float x) { return my_sin(x + M_PI_2); }

	static float my_asin(float x) { return my_atan2(x, my_sqrt(1.0 - (x * x))); }

	
	vector3 lerp(vector3 v, float x, bool clamp = true)
	{
		auto delta = (v - *this);
		if (clamp)
			delta.clamp();

		auto result = (*this + delta * x);
		if (clamp)
			result.clamp();

		return result;
	}

		static float UnityClamp(float value)
		{
			float result;
			if (value < 0.f)
			{
				result = 0.f;
			}
			else if (value > 1.f)
			{
				result = 1.f;
			}
			else
			{
				result = value;
			}
			return result;
		}

		float Lerp()
		{
			return x + (y - x) * vector3::UnityClamp(z);
		}
	vector3 forward()
	{
		float sp, sy, cp, cy;

		constexpr auto PI = 3.14159265358979323846;
		sy = my_sin(y / 180 * (float)PI);
		cy = my_cos(y / 180 * (float)PI);

		sp = my_sin(x / 180 * (float)PI);
		cp = my_cos(x / 180 * (float)PI);

		return { cp * cy, -sp, cp * sy };
	}

	template<typename T>
	T get_remainder(T val, T min, T max)
	{
		while (val > max)
			val -= max * 2;
		while (val < min)
			val += max * 2;
		return val;
	}

	vector3 clamp()
	{
		constexpr auto yaw_limit = static_cast<float>(180);
		constexpr auto pitch_limit = static_cast<float>(90);

		y = get_remainder(y, -yaw_limit, yaw_limit);
		x = get_remainder(x, -pitch_limit, pitch_limit);

		/*if ( x > 180 )
			x -= 360;
		else if ( x < -180 )
			x += 360;

		if ( y > 180 )
			y -= 360;
		else if ( y < -180 )
			y += 360;

		if ( x < -89 )
			x = -89;

		if ( x > 89 )
			x = 89;

		while ( y < -180 )
			y += 360;

		while ( y > 180 )
			y -= 360;

		z = 0;*/

		if (x != x)
			x = 0;
		if (y != y)
			y = 0;
		if (z != z)
			z = 0;

		return *this;
	}


	static float my_atan2(float y, float x)
	{
		// https://en.wikipedia.org/wiki/Atan2#Definition (A compact expression with four
		// overlapping half-planes is: ...)
		if (x == 0) {
			if (y > 0.0f)
				return M_PI_2;
			else if (y < 0.0f)
				return -M_PI_2;
			return 0;
		}
		else if (x > 0)
			return my_atan(y / x);
		else if (x < 0)
			return M_PI + my_atan(y / x);
		else if (y > 0)
			return M_PI_2 - my_atan(y / x);
		else if (y < 0)
			return -M_PI_2 - my_atan(y / x);
		return 0;
	}

	vector3 rotation()
	{
		float pitch = -my_atan2(y, my_sqrt(x * x + z * z));
		float yaw = -my_atan2(-x, z);

		yaw *= RAD_TO_DEG;
		pitch *= RAD_TO_DEG;

		return { pitch, yaw, 0 };
	}

	inline float Distance(const vector3& vector)
	{
		return my_sqrt(
			(x - vector.x) * (x - vector.x) +
			(y - vector.y) * (y - vector.y) +
			(z - vector.z) * (z - vector.z));
	}

	float dot(const vector3& vector)
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	float length()
	{
		return my_sqrt(dot(*this));
	}

	


#define powFFFFFFFFFFFFFFFFFFFFFF(n) (n)*(n)

	float get_3d_dist(const vector3& Dst) {
		return my_sqrt(powFFFFFFFFFFFFFFFFFFFFFF(x - Dst.x) + powFFFFFFFFFFFFFFFFFFFFFF(y - Dst.y) + powFFFFFFFFFFFFFFFFFFFFFF(z - Dst.z));
	}



	float distance(const vector3& vector)
	{
		return my_sqrt(
			(x - vector.x) * (x - vector.x) +
			(y - vector.y) * (y - vector.y) +
			(z - vector.z) * (z - vector.z));
	}




	vector3 normalize(vector3 out)
	{
		auto l = length();
		if (l == 0)
			return *this;
		;
		out.x /= l;
		out.y /= l;
		out.z /= l;
		return out;
	}

	float world_distance(const vector3& vector)
	{
		return float(sqrtf(powf(vector.x - x, 2.0) + powf(vector.y - y, 2.0) + powf(vector.z - z, 2.0)));
	}

	void rotate(float deg)
	{
		float theta = deg / 180.0f * 3.14159265358979323846f;
		float c = cos(theta);
		float s = sin(theta);
		float tx = x * c - y * s;
		float ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	vector3& operator+=(const vector3& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;

		return *this;
	}

	vector3& operator-=(const vector3& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;

		return *this;
	}

	vector3& operator*=(float number)
	{
		x *= number;
		y *= number;
		z *= number;

		return *this;
	}

	vector3& operator/=(float number)
	{
		x /= number;
		y /= number;
		z /= number;

		return *this;
	}

	bool operator==(const vector3& vector) const
	{
		return x == vector.x && y == vector.y && z == vector.z;
	}

	bool operator!=(const vector3& vector) const
	{
		return x != vector.x || y != vector.y || z != vector.z;
	}

	vector3 operator+(const vector3& vector) const
	{
		return vector3(x + vector.x, y + vector.y, z + vector.z);
	}

	vector3 operator-(const vector3& vector) const
	{
		return vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	vector3 operator-() const
	{
		return vector3(-x, -y, -z);
	}

	vector3 operator*(float number) const
	{
		return vector3(x * number, y * number, z * number);
	}

	vector3 operator/(float number) const
	{
		return vector3(x / number, y / number, z / number);
	}

	void angle_vectors(vector3* forward, vector3* right, vector3* up)
	{
		float sp, sy, cp, cy, sr, cr;

		constexpr auto PI = 3.14159265358979323846;
		sy = my_sin(y / 180.f * (float)PI);
		cy = my_cos(y / 180.f * (float)PI);

		sp = my_sin(x / 180.f * (float)PI);
		cp = my_cos(x / 180.f * (float)PI);

		sr = 0.f;
		cr = 1.f;

		if (forward) {
			*forward = { cp * cy, -sp, cp * sy };
		}

		if (right) {
			right->x = cp * sy;
			right->y = (cr * sp * sy + -sr * cy);
			right->z = (sr * sp * sy + cr * cy);
		}

		if (up) {
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = cr * cp;
			up->z = (cr * sp * sy + -sr * cy);
		}
	}

	float Magnitude() { return sqrtn(this->x * this->x + this->y * this->y + this->z * this->z); }
	vector3 Normalized() { float num = this->Magnitude(); if (num > 9.99999974737875E-06) { return { this->x / num, this->y / num, this->z / num }; } return vector3(); }

	vector3 Normalized2() {
		float len = Length();
		return vector3(x / len, y / len, z / len);
	}
};

 
class vector2
{
public:
	float x;
	float y;
	vector2()
	{}
	vector2(float x, float y)
		: x(x), y(y)
	{}

	inline vector2 operator/(float v) const {
		return vector2(x / v, y / v);
	}
	inline vector2 operator-(const vector2& v) const {
		return vector2(x - v.x, y - v.y);
	}

	inline vector2 operator+(const vector2& v) const {
		return vector2(x + v.x, y + v.y);
	}
	float distance(vector2 b)
	{
		return sqrt(pow(b.x - x, 2) + pow(b.y - y, 2));
	}
};





int __fpclassifyf(float f)
{
	union IEEEf2bits {
		float	f;
		struct {
			unsigned int	man : 23;
			unsigned int	exp : 8;
			unsigned int	sign : 1;
		} bits;
	};

	union IEEEf2bits u;
	u.f = f;
	if (u.bits.exp == 0) {
		if (u.bits.man == 0)
			return (FP_ZERO);
		return (FP_SUBNORMAL);
	}
	if (u.bits.exp == 255) {
		if (u.bits.man == 0)
			return (FP_INFINITE);
		return (FP_NAN);
	}
	return (FP_NORMAL);
}
bool isnanf(float a) {
	return bool(__fpclassifyf(a) == FP_NAN);
}
float to_degree(float radian)
{
	return radian * 180.f / M_PI;
}
float Length(vector3 v) {
	return sqrtn(v.x * v.x + v.y * v.y + v.z * v.z);
}	

class vector4
{
public:
	vector4()
	{
		x = y = z = w = 0.f;
	}

	vector4(float fx, float fy, float fz, float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	float x, y, z, w;


};


double ToRad(double degree)
{
	double pi = M_PI;
	return (degree * (pi / 180));
}
vector4 ToQuat(vector3 Euler) {

	double heading = ToRad(Euler.x);
	double attitude = ToRad(Euler.y);
	double bank = ToRad(Euler.z);

	double c1 = vector3::my_cos(heading / 2);
	double s1 = vector3::my_sin(heading / 2);
	double c2 = vector3::my_cos(attitude / 2);
	double s2 = vector3::my_sin(attitude / 2);
	double c3 = vector3::my_cos(bank / 2);
	double s3 = vector3::my_sin(bank / 2);
	double c1c2 = c1 * c2;
	double s1s2 = s1 * s2;
	vector4 Quat;
	Quat.w = c1c2 * c3 - s1s2 * s3;
	Quat.x = c1c2 * s3 + s1s2 * c3;
	Quat.y = s1 * c2 * c3 + c1 * s2 * s3;
	Quat.z = c1 * s2 * c3 - s1 * c2 * s3;
	vector4 Quat2;
	Quat2 = { Quat.y, Quat.z, (Quat.x * -1), Quat.w };
	return Quat2;
}
static vector3 CalcAngle(vector3 LocalPos, vector3 EnemyPos)
{
	vector3 dir;

	dir.x = LocalPos.x - EnemyPos.x;
	dir.y = LocalPos.y - EnemyPos.y;
	dir.z = LocalPos.z - EnemyPos.z;
		
	float Pitch = to_degree(__asinf(dir.y / Length(dir)));
	float Yaw = to_degree(-__atan2f(dir.x, -dir.z));


	vector3 Return;
	Return.x = Pitch;
	Return.y = Yaw;


	return Return;
}
static bool Normalizee(float& Yaw, float& Pitch) {
	//Vector2 CurrentViewAngles = LocalPlayer.BasePlayer->GetVA();
	if (Pitch < -89)
		//Pitch = -89;
		return false;

	else if (Pitch > 89)
		//Pitch = 89;
		return false;

	if (Yaw < -360)
		/*Yaw += 360;*/
		return false;

	else if (Yaw > 360)
		//Yaw -= 360;
		return false;
}


__forceinline float Dot(const vector3& Vec1, const vector3& Vec2) {
	return Vec1[0] * Vec2[0] + Vec1[1] * Vec2[1] + Vec1[2] * Vec2[2];
}

struct Matrix4x4 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		}; float m[4][4];
	};
}; inline Matrix4x4 pViewMatrix;


#define pow(n) (n)*(n)

float Calc3D_Dist(const vector3& Src, const vector3& Dst) {
	return vector3::my_sqrt(pow(Src.x - Dst.x) + pow(Src.y - Dst.y) + pow(Src.z - Dst.z));
}

float deg_2_rad(float degrees)
{
	float radians;
	radians = degrees * (M_PI / 180);
	return radians;
}
void angle_vectors(const vector3& angles, vector3* forward)
{
	float    sp, sy, cp, cy;
	sy = __sinf(deg_2_rad(angles.y));
	cy = __cosf(deg_2_rad(angles.y));
	sp = __sinf(deg_2_rad(angles.x));
	cp = __cosf(deg_2_rad(angles.x));
	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}